
/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>

#include "material/CheckerTexture.h"
#include "material/MyMaterial.h"

namespace RTKit1 {

/**
 * Material for "Improved Model" (Whitted 1980)
 */
class Material : public MyMaterial {
  std::shared_ptr<MyTexture> baseColor;

 public:
  Material() {}

  Material& setColor(const glm::vec3& color) {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(color);
    baseColor = ct;
    return *this;
  }

  Material& setCheckerTexture() {
    baseColor = std::make_shared<CheckerTexture>();
    return *this;
  }

  Material& setCheckerTexture(const glm::vec3& colorOdd,
                              const glm::vec3& colorEven) {
    auto checker = std::make_shared<CheckerTexture>();
    checker->setColor(colorOdd, colorEven);
    baseColor = checker;
    return *this;
  }

  Material& setTiling(float u, float v) {
    uvTiling = glm::vec2(u, v);
    return *this;
  }

  Material& setParam(float inKd, float inKs, float inKt, float inN,
                     float inEta) {
    Kd = inKd;
    Ks = inKs;
    Kt = inKt;
    n = inN;
    eta = inEta;
    return *this;
  }

  glm::vec3 sampleBaseColor(const glm::vec2& uv, const glm::vec3& p) {
    if (baseColor)
      return baseColor->sample(uvTiling * uv, uvTiling.x * p);
    else
      return glm::vec3(1);
  }

 public:
  glm::vec2 uvTiling = {1, 1};

  float Kd = 0.5f;  // diffuse reflection constant
  float Ks = 0.4f;  // the specular reflection coefficient
  float Kt = 0.1f;  // the transmission coefficient
  float n = 99;     // an exponent that depends on the glossiness of the surface
  float eta = 1.34f;  // the index of refraction,
};

}  // namespace RTKit1
