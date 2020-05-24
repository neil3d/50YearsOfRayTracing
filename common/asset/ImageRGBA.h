/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */
#pragma once

#include <vector>

#include "MyAssetObject.h"

namespace RayTracingHistory {
class ImageRGBA : public MyAssetObject {
 public:
  struct Pixel {
    uint8_t R, G, B, A;
  };

  typedef std::shared_ptr<ImageRGBA> Ptr;

  using MyAssetObject::MyAssetObject;

  virtual void loadFromFile(const std::string& szPath) override;

  uint32_t getWidth() const { return mWidth; }
  uint32_t getHeight() const { return mHeight; }

  Pixel getPixel(const uint32_t x, const uint32_t y) const {
    if (x < mWidth && y < mHeight) return mData[y * mWidth + x];

    return Pixel{0};
  }

 private:
  uint32_t mWidth = 0;
  uint32_t mHeight = 0;
  std::vector<Pixel> mData;
};
}  // namespace RayTracingHistory