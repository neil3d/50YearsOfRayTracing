/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "../asset/ImageRGBA.h"
#include "MyTexture.h"

namespace RayTracingHistory {

class Texture2D : public MyTexture {
 private:
  ImageRGBA::Ptr mImage;

 public:
  void loadImage(const std::string& szFilePath);

  virtual glm::vec3 sample(const glm::vec2& uv, const glm::vec3& p) override;
};

}  // namespace RayTracingHistory
