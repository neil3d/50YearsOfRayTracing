/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>

#include "geometry/Ray.h"

namespace RayTracingHistory {

/**
 * abstract area light base class
 */
class AreaLight {
 public:
  AreaLight() = default;
  virtual ~AreaLight() = default;

  virtual Ray generateShadowRay(const glm::vec3& shadingPt,
                                const glm::vec2& xi) const = 0;
  virtual float getArea() const = 0;

 protected:
  float intensity = 2.0f;
};

}  // namespace RayTracingHistory
