/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>

#include "material/CheckerTexture.h"
#include "material/MyMaterial.h"
#include "material/Texture2D.h"

namespace RayTracingHistory {

enum class ESamplingType { Uniform };

class MaterialBase : public MyMaterial {
  std::shared_ptr<MyTexture> mAlbedo;
  ESamplingType mSampling = ESamplingType::Uniform;

 public:
  MaterialBase() {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(color);
    mAlbedo = ct;
  }
};
}  // namespace RayTracingHistory
