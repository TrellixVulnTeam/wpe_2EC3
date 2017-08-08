/*
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2005, 2006, 2008, 2015 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "BitmapImage.h"

#include "FloatRect.h"
#include "GraphicsContext.h"
#include "ImageBuffer.h"
#include "ImageObserver.h"
#include "IntRect.h"
#include "Logging.h"
#include "Settings.h"
#include "TextStream.h"
#include "Timer.h"
#include <wtf/CurrentTime.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

#if PLATFORM(IOS)
#include <limits>
#endif

namespace WebCore {

BitmapImage::BitmapImage(ImageObserver* observer)
    : Image(observer)
    , m_source(this)
{
}

BitmapImage::BitmapImage(NativeImagePtr&& image, ImageObserver* observer)
    : Image(observer)
    , m_source(WTFMove(image))
{
}

BitmapImage::~BitmapImage()
{
    invalidatePlatformData();
    clearTimer();
    m_source.stopAsyncDecodingQueue();
}

void BitmapImage::updateFromSettings(const Settings& settings)
{
    m_allowSubsampling = settings.imageSubsamplingEnabled();
    m_allowLargeImageAsyncDecoding = settings.largeImageAsyncDecodingEnabled();
    m_allowAnimatedImageAsyncDecoding = settings.animatedImageAsyncDecodingEnabled();
    m_showDebugBackground = settings.showDebugBorders();
}

void BitmapImage::destroyDecodedData(bool destroyAll)
{
    LOG(Images, "BitmapImage::%s - %p - url: %s", __FUNCTION__, this, sourceURL().string().utf8().data());

    if (!destroyAll)
        m_source.destroyDecodedDataBeforeFrame(m_currentFrame);
    else if (!canDestroyDecodedData()) {
        m_source.destroyAllDecodedDataExcludeFrame(m_currentFrame);
        destroyAll = false;
    } else {
        m_source.destroyAllDecodedData();
        m_currentFrameDecodingStatus = ImageFrame::DecodingStatus::Invalid;
    }

    // There's no need to throw away the decoder unless we're explicitly asked
    // to destroy all of the frames.
    if (!destroyAll)
        m_source.clearFrameBufferCache(m_currentFrame);
    else
        m_source.clear(data());

    invalidatePlatformData();
}

void BitmapImage::destroyDecodedDataIfNecessary(bool destroyAll)
{
    // If we have decoded frames but there is no encoded data, we shouldn't destroy
    // the decoded image since we won't be able to reconstruct it later.
    if (!data() && frameCount())
        return;

    if (m_source.decodedSize() < LargeAnimationCutoff)
        return;

    destroyDecodedData(destroyAll);
}

EncodedDataStatus BitmapImage::dataChanged(bool allDataReceived)
{
    if (!shouldUseAsyncDecodingForLargeImages())
        m_source.destroyIncompleteDecodedData();

    m_currentFrameDecodingStatus = ImageFrame::DecodingStatus::Invalid;
    return m_source.dataChanged(data(), allDataReceived);
}

NativeImagePtr BitmapImage::frameImageAtIndexCacheIfNeeded(size_t index, SubsamplingLevel subsamplingLevel, const GraphicsContext* targetContext)
{
    if (!frameHasFullSizeNativeImageAtIndex(index, subsamplingLevel)) {
        LOG(Images, "BitmapImage::%s - %p - url: %s [subsamplingLevel was %d, resampling]", __FUNCTION__, this, sourceURL().string().utf8().data(), static_cast<int>(frameSubsamplingLevelAtIndex(index)));
        invalidatePlatformData();
    }

    return m_source.frameImageAtIndexCacheIfNeeded(index, subsamplingLevel, targetContext);
}

NativeImagePtr BitmapImage::nativeImage(const GraphicsContext* targetContext)
{
    return frameImageAtIndexCacheIfNeeded(0, SubsamplingLevel::Default, targetContext);
}

NativeImagePtr BitmapImage::nativeImageForCurrentFrame(const GraphicsContext* targetContext)
{
    return frameImageAtIndexCacheIfNeeded(m_currentFrame, SubsamplingLevel::Default, targetContext);
}

#if USE(CG)
NativeImagePtr BitmapImage::nativeImageOfSize(const IntSize& size, const GraphicsContext* targetContext)
{
    size_t count = frameCount();

    for (size_t i = 0; i < count; ++i) {
        auto image = frameImageAtIndexCacheIfNeeded(i, SubsamplingLevel::Default, targetContext);
        if (image && nativeImageSize(image) == size)
            return image;
    }

    // Fallback to the first frame image if we can't find the right size
    return frameImageAtIndexCacheIfNeeded(0, SubsamplingLevel::Default, targetContext);
}

Vector<NativeImagePtr> BitmapImage::framesNativeImages()
{
    Vector<NativeImagePtr> images;
    size_t count = frameCount();

    for (size_t i = 0; i < count; ++i) {
        if (auto image = frameImageAtIndexCacheIfNeeded(i))
            images.append(image);
    }

    return images;
}
#endif

#if !ASSERT_DISABLED
bool BitmapImage::notSolidColor()
{
    return size().width() != 1 || size().height() != 1 || frameCount() > 1;
}
#endif

void BitmapImage::draw(GraphicsContext& context, const FloatRect& destRect, const FloatRect& srcRect, CompositeOperator op, BlendMode mode, DecodingMode decodingMode, ImageOrientationDescription description)
{
    if (destRect.isEmpty() || srcRect.isEmpty())
        return;

    FloatSize scaleFactorForDrawing = context.scaleFactorForDrawing(destRect, srcRect);
    IntSize sizeForDrawing = expandedIntSize(size() * scaleFactorForDrawing);
    
    m_currentSubsamplingLevel = m_allowSubsampling ? m_source.subsamplingLevelForScaleFactor(context, scaleFactorForDrawing) : SubsamplingLevel::Default;
    LOG(Images, "BitmapImage::%s - %p - url: %s [subsamplingLevel = %d scaleFactorForDrawing = (%.4f, %.4f)]", __FUNCTION__, this, sourceURL().string().utf8().data(), static_cast<int>(m_currentSubsamplingLevel), scaleFactorForDrawing.width(), scaleFactorForDrawing.height());

    NativeImagePtr image;
    if (decodingMode == DecodingMode::Asynchronous && shouldUseAsyncDecodingForLargeImages()) {
        ASSERT(!canAnimate() && !m_currentFrame);

        bool frameIsCompatible = frameHasDecodedNativeImageCompatibleWithOptionsAtIndex(m_currentFrame, m_currentSubsamplingLevel, DecodingOptions(sizeForDrawing));
        bool frameIsBeingDecoded = frameIsBeingDecodedAndIsCompatibleWithOptionsAtIndex(m_currentFrame, DecodingOptions(sizeForDrawing));

        // If the current frame is incomplete, a new request for decoding this frame has to be made even if
        // it is currently being decoded. New data may have been received since the previous request was made.
        if ((!frameIsCompatible && !frameIsBeingDecoded) || m_currentFrameDecodingStatus == ImageFrame::DecodingStatus::Invalid) {
            LOG(Images, "BitmapImage::%s - %p - url: %s [requesting large async decoding]", __FUNCTION__, this, sourceURL().string().utf8().data());
            m_source.requestFrameAsyncDecodingAtIndex(0, m_currentSubsamplingLevel, sizeForDrawing);
            m_currentFrameDecodingStatus = ImageFrame::DecodingStatus::Decoding;
        }

        if (!frameHasDecodedNativeImageCompatibleWithOptionsAtIndex(m_currentFrame, m_currentSubsamplingLevel, DecodingMode::Asynchronous)) {
            if (m_showDebugBackground)
                fillWithSolidColor(context, destRect, Color(Color::yellow).colorWithAlpha(0.5), op);
            return;
        }

        image = frameImageAtIndex(m_currentFrame);
        LOG(Images, "BitmapImage::%s - %p - url: %s [a decoded image frame is available for drawing]", __FUNCTION__, this, sourceURL().string().utf8().data());
    } else {
        StartAnimationStatus status = internalStartAnimation();
        ASSERT_IMPLIES(status == StartAnimationStatus::DecodingActive, frameHasFullSizeNativeImageAtIndex(m_currentFrame, m_currentSubsamplingLevel));

        if (status == StartAnimationStatus::DecodingActive && m_showDebugBackground) {
            fillWithSolidColor(context, destRect, Color(Color::yellow).colorWithAlpha(0.5), op);
            return;
        }

        if (frameIsBeingDecodedAndIsCompatibleWithOptionsAtIndex(m_currentFrame, DecodingMode::Asynchronous)) {
            // FIXME: instead of showing the yellow rectangle and returning we need to wait for this the frame to finish decoding.
            if (m_showDebugBackground) {
                fillWithSolidColor(context, destRect, Color(Color::yellow).colorWithAlpha(0.5), op);
                LOG(Images, "BitmapImage::%s - %p - url: %s [waiting for async decoding to finish]", __FUNCTION__, this, sourceURL().string().utf8().data());
            }
            return;
        }

        image = frameImageAtIndexCacheIfNeeded(m_currentFrame, m_currentSubsamplingLevel, &context);
        if (!image) // If it's too early we won't have an image yet.
            return;
    }

    ASSERT(image);
    Color color = singlePixelSolidColor();
    if (color.isValid()) {
        fillWithSolidColor(context, destRect, color, op);
        return;
    }

    ImageOrientation orientation(description.imageOrientation());
    if (description.respectImageOrientation() == RespectImageOrientation)
        orientation = frameOrientationAtIndex(m_currentFrame);

    drawNativeImage(image, context, destRect, srcRect, IntSize(size()), op, mode, orientation);
    m_currentFrameDecodingStatus = frameDecodingStatusAtIndex(m_currentFrame);

    if (imageObserver())
        imageObserver()->didDraw(*this);
}

void BitmapImage::drawPattern(GraphicsContext& ctxt, const FloatRect& destRect, const FloatRect& tileRect, const AffineTransform& transform, const FloatPoint& phase, const FloatSize& spacing, CompositeOperator op, BlendMode blendMode)
{
    if (tileRect.isEmpty())
        return;

    if (!ctxt.drawLuminanceMask()) {
        Image::drawPattern(ctxt, destRect, tileRect, transform, phase, spacing, op, blendMode);
        return;
    }

    if (!m_cachedImage) {
        auto buffer = ImageBuffer::createCompatibleBuffer(expandedIntSize(tileRect.size()), ColorSpaceSRGB, ctxt);
        if (!buffer)
            return;

        ImageObserver* observer = imageObserver();

        // Temporarily reset image observer, we don't want to receive any changeInRect() calls due to this relayout.
        setImageObserver(nullptr);

        draw(buffer->context(), tileRect, tileRect, op, blendMode, DecodingMode::Synchronous, ImageOrientationDescription());

        setImageObserver(observer);
        buffer->convertToLuminanceMask();

        m_cachedImage = buffer->copyImage(DontCopyBackingStore, Unscaled);
        if (!m_cachedImage)
            return;
    }

    ctxt.setDrawLuminanceMask(false);
    m_cachedImage->drawPattern(ctxt, destRect, tileRect, transform, phase, spacing, op, blendMode);
}

bool BitmapImage::shouldAnimate()
{
    return repetitionCount() && !m_animationFinished && imageObserver();
}

bool BitmapImage::canAnimate()
{
    return shouldAnimate() && frameCount() > 1;
}

bool BitmapImage::shouldUseAsyncDecodingForLargeImages()
{
    return !canAnimate() && m_allowLargeImageAsyncDecoding && m_source.shouldUseAsyncDecoding();
}

bool BitmapImage::shouldUseAsyncDecodingForAnimatedImages()
{
    return canAnimate() && m_allowAnimatedImageAsyncDecoding && (shouldUseAsyncDecodingForAnimatedImagesForTesting() || m_source.shouldUseAsyncDecoding());
}

void BitmapImage::clearTimer()
{
    m_frameTimer = nullptr;
}

void BitmapImage::startTimer(Seconds delay)
{
    ASSERT(!m_frameTimer);
    m_frameTimer = std::make_unique<Timer>(*this, &BitmapImage::advanceAnimation);
    m_frameTimer->startOneShot(delay);
}

bool BitmapImage::canDestroyDecodedData()
{
    // Animated images should preserve the current frame till the next one finishes decoding.
    if (m_source.hasAsyncDecodingQueue())
        return false;

    // Small image should be decoded synchronously. Deleting its decoded frame is fine.
    if (!shouldUseAsyncDecodingForLargeImages())
        return true;

    return !imageObserver() || imageObserver()->canDestroyDecodedData(*this);
}

BitmapImage::StartAnimationStatus BitmapImage::internalStartAnimation()
{
    if (!canAnimate())
        return StartAnimationStatus::CannotStart;

    if (m_frameTimer)
        return StartAnimationStatus::TimerActive;

    // Don't start a new animation until we draw the frame that is currently being decoded.
    size_t nextFrame = (m_currentFrame + 1) % frameCount();
    if (frameIsBeingDecodedAndIsCompatibleWithOptionsAtIndex(nextFrame, DecodingMode::Asynchronous)) {
        LOG(Images, "BitmapImage::%s - %p - url: %s [nextFrame = %ld is being decoded]", __FUNCTION__, this, sourceURL().string().utf8().data(), nextFrame);
        return StartAnimationStatus::DecodingActive;
    }

    if (m_currentFrame >= frameCount() - 1) {
        // Don't advance past the last frame if we haven't decoded the whole image
        // yet and our repetition count is potentially unset. The repetition count
        // in a GIF can potentially come after all the rest of the image data, so
        // wait on it.
        if (!m_source.isAllDataReceived() && repetitionCount() == RepetitionCountOnce)
            return StartAnimationStatus::IncompleteData;

        ++m_repetitionsComplete;

        // Check for the end of animation.
        if (repetitionCount() != RepetitionCountInfinite && m_repetitionsComplete >= repetitionCount()) {
            m_animationFinished = true;
            destroyDecodedDataIfNecessary(false);
            return StartAnimationStatus::CannotStart;
        }
    }

    // When looping on an animation, destroy de decoded data when in the first frame instead of the last one.
    // If it's done in the last one and a redraw happens before the animation advances, we need to decode all
    // the animation frames to get the last one again, which causes a delay in the animation. If this happens
    // while in the first one, we only need to decode a single frame.
    if (m_currentFrame == 0)
        destroyDecodedDataIfNecessary(true);


    // Don't advance the animation to an incomplete frame.
    if (!m_source.isAllDataReceived() && !frameIsCompleteAtIndex(nextFrame))
        return StartAnimationStatus::IncompleteData;

    MonotonicTime time = MonotonicTime::now();

    // Handle initial state.
    if (!m_desiredFrameStartTime)
        m_desiredFrameStartTime = time;

    // Setting 'm_desiredFrameStartTime' to 'time' means we are late; otherwise we are early.
    m_desiredFrameStartTime = std::max(time, m_desiredFrameStartTime + Seconds { frameDurationAtIndex(m_currentFrame) });

    // Request async decoding for nextFrame only if this is required. If nextFrame is not in the frameCache,
    // it will be decoded on a separate work queue. When decoding nextFrame finishes, we will be notified
    // through the callback newFrameNativeImageAvailableAtIndex(). Otherwise, advanceAnimation() will be called
    // when the timer fires and m_currentFrame will be advanced to nextFrame since it is not being decoded.
    if (shouldUseAsyncDecodingForAnimatedImages()) {
        if (frameHasDecodedNativeImageCompatibleWithOptionsAtIndex(nextFrame, m_currentSubsamplingLevel, { }))
            LOG(Images, "BitmapImage::%s - %p - url: %s [cachedFrameCount = %ld nextFrame = %ld]", __FUNCTION__, this, sourceURL().string().utf8().data(), ++m_cachedFrameCount, nextFrame);
        else {
            m_source.requestFrameAsyncDecodingAtIndex(nextFrame, m_currentSubsamplingLevel);
            m_currentFrameDecodingStatus = ImageFrame::DecodingStatus::Decoding;
            LOG(Images, "BitmapImage::%s - %p - url: %s [requesting async decoding for nextFrame = %ld]", __FUNCTION__, this, sourceURL().string().utf8().data(), nextFrame);
        }

        m_desiredFrameDecodeTimeForTesting = time + std::max(m_frameDecodingDurationForTesting, 0_s);
        if (m_clearDecoderAfterAsyncFrameRequestForTesting)
            m_source.clear(data());
    }

    ASSERT(!m_frameTimer);
    startTimer(m_desiredFrameStartTime - time);
    return StartAnimationStatus::Started;
}

void BitmapImage::advanceAnimation()
{
    clearTimer();

    // Pretend as if decoding nextFrame has taken m_frameDecodingDurationForTesting from
    // the time this decoding was requested.
    if (shouldUseAsyncDecodingForAnimatedImagesForTesting()) {
        MonotonicTime time = MonotonicTime::now();
        // Start a timer with the remaining time from now till the m_desiredFrameDecodeTime.
        if (m_desiredFrameDecodeTimeForTesting > std::max(time, m_desiredFrameStartTime)) {
            startTimer(m_desiredFrameDecodeTimeForTesting - time);
            return;
        }
    }

    // Don't advance to nextFrame unless its decoding has finished or was not required.
    size_t nextFrame = (m_currentFrame + 1) % frameCount();
    if (!frameIsBeingDecodedAndIsCompatibleWithOptionsAtIndex(nextFrame, DecodingMode::Asynchronous))
        internalAdvanceAnimation();
    else {
        // Force repaint if showDebugBackground() is on.
        if (m_showDebugBackground)
            imageObserver()->changedInRect(*this);
        LOG(Images, "BitmapImage::%s - %p - url: %s [lateFrameCount = %ld nextFrame = %ld]", __FUNCTION__, this, sourceURL().string().utf8().data(), ++m_lateFrameCount, nextFrame);
    }
}

void BitmapImage::internalAdvanceAnimation()
{
    m_currentFrame = (m_currentFrame + 1) % frameCount();
    ASSERT(!frameIsBeingDecodedAndIsCompatibleWithOptionsAtIndex(m_currentFrame, DecodingMode::Asynchronous));

    destroyDecodedDataIfNecessary(false);

    if (m_currentFrameDecodingStatus == ImageFrame::DecodingStatus::Decoding)
        m_currentFrameDecodingStatus = frameDecodingStatusAtIndex(m_currentFrame);
    if (imageObserver())
        imageObserver()->imageFrameAvailable(*this, ImageAnimatingState::Yes);

    LOG(Images, "BitmapImage::%s - %p - url: %s [m_currentFrame = %ld]", __FUNCTION__, this, sourceURL().string().utf8().data(), m_currentFrame);
}

bool BitmapImage::isAnimating() const
{
    return !!m_frameTimer;
}

void BitmapImage::stopAnimation()
{
    // This timer is used to animate all occurrences of this image. Don't invalidate
    // the timer unless all renderers have stopped drawing.
    clearTimer();
    if (canAnimate())
        m_source.stopAsyncDecodingQueue();
}

void BitmapImage::resetAnimation()
{
    stopAnimation();
    m_currentFrame = 0;
    m_repetitionsComplete = RepetitionCountNone;
    m_desiredFrameStartTime = { };
    m_animationFinished = false;

    // For extremely large animations, when the animation is reset, we just throw everything away.
    destroyDecodedDataIfNecessary(true);
}

void BitmapImage::imageFrameAvailableAtIndex(size_t index)
{
    UNUSED_PARAM(index);
    LOG(Images, "BitmapImage::%s - %p - url: %s [requested frame %ld is now available]", __FUNCTION__, this, sourceURL().string().utf8().data(), index);

    if (canAnimate()) {
        ASSERT(index == (m_currentFrame + 1) % frameCount());
        
        // Don't advance to nextFrame unless the timer was fired before its decoding finishes.
        if (canAnimate() && !m_frameTimer)
            internalAdvanceAnimation();
        else
            LOG(Images, "BitmapImage::%s - %p - url: %s [earlyFrameCount = %ld nextFrame = %ld]", __FUNCTION__, this, sourceURL().string().utf8().data(), ++m_earlyFrameCount, index);
    } else {
        ASSERT(index == m_currentFrame && !m_currentFrame);
        if (m_source.isAsyncDecodingQueueIdle())
            m_source.stopAsyncDecodingQueue();
        if (m_currentFrameDecodingStatus == ImageFrame::DecodingStatus::Decoding)
            m_currentFrameDecodingStatus = frameDecodingStatusAtIndex(m_currentFrame);
        if (imageObserver())
            imageObserver()->imageFrameAvailable(*this, ImageAnimatingState::No);
    }
}

void BitmapImage::dump(TextStream& ts) const
{
    Image::dump(ts);
    
    if (isAnimated())
        ts.dumpProperty("current-frame", m_currentFrame);
    
    m_source.dump(ts);
}

}