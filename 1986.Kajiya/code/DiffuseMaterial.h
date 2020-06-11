/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

#include "MaterialBase.h"
#include "geometry/ONB.h"

namespace RayTracingHistory {

/**
 * diffuse : uniform sampling
 */
class DiffuseMaterial : public MaterialBase {
  float Kd = 0.5f;

 public:

};

}  // namespace RayTracingHistory
