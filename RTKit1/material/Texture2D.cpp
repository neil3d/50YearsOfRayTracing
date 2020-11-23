#include "Texture2D.h"

#include "../asset/MyAssetManager.h"

namespace RTKit1 {

void Texture2D::loadImage(const std::string& szFilePath) {
  mImage = MyAssetManager::instance().loadAssetObject<ImageRGBA>(szFilePath);
}

glm::vec3 Texture2D::sample(const glm::vec2& uv, const glm::vec3& p) {
  if (!mImage) return glm::vec3(1, 0, 0);

  float u = uv.x - floor(uv.x);
  float v = uv.y - floor(uv.y);

  int w = mImage->getWidth();
  int h = mImage->getHeight();

  // TODO: filter
  int x = int(u * w);
  int y = int(v * h);

  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x >= w) x = w - 1;
  if (y >= h) y = h - 1;

  ImageRGBA::Pixel pixel = mImage->getPixel(x, y);

  constexpr float s = 1.0f / 256;
  return s * glm::vec3(pixel.R, pixel.G, pixel.B);
}

}  // namespace RTKit1
