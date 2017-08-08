/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_API_VIDEO_I420_BUFFER_H_
#define WEBRTC_API_VIDEO_I420_BUFFER_H_

#include <memory>

#include "webrtc/api/video/video_rotation.h"
#include "webrtc/api/video/video_frame_buffer.h"
#include "webrtc/base/export.h"
#include "webrtc/system_wrappers/include/aligned_malloc.h"

namespace webrtc {

// Plain I420 buffer in standard memory.
class I420Buffer : public VideoFrameBuffer {
 public:
  static rtc::scoped_refptr<I420Buffer> Create(int width, int height);
  static rtc::scoped_refptr<I420Buffer> Create(int width,
                                               int height,
                                               int stride_y,
                                               int stride_u,
                                               int stride_v);

  // Create a new buffer and copy the pixel data.
  static rtc::scoped_refptr<I420Buffer> Copy(const VideoFrameBuffer& buffer);

  static rtc::scoped_refptr<I420Buffer> Copy(
      int width, int height,
      const uint8_t* data_y, int stride_y,
      const uint8_t* data_u, int stride_u,
      const uint8_t* data_v, int stride_v);

  // Returns a rotated copy of |src|.
  WEBRTC_DYLIB_EXPORT static rtc::scoped_refptr<I420Buffer> Rotate(const VideoFrameBuffer& src,
                                               VideoRotation rotation);

  // Sets the buffer to all black.
  WEBRTC_DYLIB_EXPORT static void SetBlack(I420Buffer* buffer);

  // Sets all three planes to all zeros. Used to work around for
  // quirks in memory checkers
  // (https://bugs.chromium.org/p/libyuv/issues/detail?id=377) and
  // ffmpeg (http://crbug.com/390941).
  // TODO(nisse): Deprecated. Should be deleted if/when those issues
  // are resolved in a better way. Or in the mean time, use SetBlack.
  void InitializeData();

  // TODO(nisse): Deprecated, use static method instead.
  void SetToBlack() { SetBlack(this); }

  int width() const override;
  int height() const override;
  const uint8_t* DataY() const override;
  const uint8_t* DataU() const override;
  const uint8_t* DataV() const override;

  int StrideY() const override;
  int StrideU() const override;
  int StrideV() const override;

  void* native_handle() const override;
  rtc::scoped_refptr<VideoFrameBuffer> NativeToI420Buffer() override;

  uint8_t* MutableDataY();
  uint8_t* MutableDataU();
  uint8_t* MutableDataV();

  // Scale the cropped area of |src| to the size of |this| buffer, and
  // write the result into |this|.
  void CropAndScaleFrom(const VideoFrameBuffer& src,
                        int offset_x,
                        int offset_y,
                        int crop_width,
                        int crop_height);

  // The common case of a center crop, when needed to adjust the
  // aspect ratio without distorting the image.
  void CropAndScaleFrom(const VideoFrameBuffer& src);

  // Scale all of |src| to the size of |this| buffer, with no cropping.
  void ScaleFrom(const VideoFrameBuffer& src);

  // TODO(nisse): Deprecated, delete once downstream applications are updated.
  // Returns a rotated versions of |src|. Native buffers are not
  // supported. The reason this function doesn't return an I420Buffer,
  // is that it returns |src| unchanged in case |rotation| is zero.
  static rtc::scoped_refptr<VideoFrameBuffer> Rotate(
      rtc::scoped_refptr<VideoFrameBuffer> src,
      VideoRotation rotation);

 protected:
  I420Buffer(int width, int height);
  I420Buffer(int width, int height, int stride_y, int stride_u, int stride_v);

  ~I420Buffer() override;

 private:
  const int width_;
  const int height_;
  const int stride_y_;
  const int stride_u_;
  const int stride_v_;
  const std::unique_ptr<uint8_t, AlignedFreeDeleter> data_;
};

}  // namespace webrtc

#endif  // WEBRTC_API_VIDEO_I420_BUFFER_H_
