/*
 * Copyright (C) 2008 Google Inc. All rights reserved.
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

#ifndef ImageBufferDataCairo_h
#define ImageBufferDataCairo_h

#if USE(CAIRO)

#include "PlatformContextCairo.h"
#include "RefPtrCairo.h"
#include <wtf/MallocPtr.h>

#if ENABLE(ACCELERATED_2D_CANVAS)
#include "PlatformLayer.h"
#include "TextureMapper.h"
#include "TextureMapperPlatformLayer.h"
#include "TextureMapperPlatformLayerProxy.h"
#endif

namespace WebCore {

class IntSize;

class ImageBufferData
#if ENABLE(ACCELERATED_2D_CANVAS)
    : public PlatformLayer
#endif
{
public:
    ImageBufferData(const IntSize&, RenderingMode);
    virtual ~ImageBufferData();

    MallocPtr<unsigned char> m_surfaceData;
    RefPtr<cairo_surface_t> m_surface;
    PlatformContextCairo m_platformContext;
    std::unique_ptr<GraphicsContext> m_context;
    IntSize m_size;
    RenderingMode m_renderingMode;

#if ENABLE(ACCELERATED_2D_CANVAS)
    void createCairoGLSurface();

#if USE(COORDINATED_GRAPHICS_THREADED)
    RefPtr<TextureMapperPlatformLayerProxy> proxy() const override { return m_platformLayerProxy.copyRef(); }
    void swapBuffersIfNeeded() override;
    void markBufferChanged();
    void createCompositorBuffer();

    RefPtr<TextureMapperPlatformLayerProxy> m_platformLayerProxy;
    bool m_bufferChanged;
    RefPtr<cairo_surface_t> m_compositorSurface;
    uint32_t m_compositorTexture;
    RefPtr<cairo_t> m_compositorCr;
#else
    virtual void paintToTextureMapper(TextureMapper&, const FloatRect& target, const TransformationMatrix&, float opacity);
#endif
    uint32_t m_texture;
#endif
};

} // namespace WebCore

#endif // USE(CAIRO)

#endif // ImageBufferDataCairo_h
