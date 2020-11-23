/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <tuple>

#include "geometry/Ray.h"

namespace RTKit1 {

/**
 * abstract area light base class
 */
class AreaLight {
 public:
  AreaLight() = default;
  virtual ~AreaLight() = default;

  virtual std::tuple<Ray, float, glm::vec3> generateShadowRay(
      const glm::vec3& shadingPt, const glm::vec3& normal,
      const glm::vec2& xi) const = 0;
  virtual float getArea() const = 0;

  float getIntensity() const { return intensity; }
  AreaLight& setIntensity(float value) {
    intensity = value;
    return *this;
  }

 protected:
  float intensity = 120;
};

}  // namespace RTKit1
