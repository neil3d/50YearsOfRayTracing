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
#include "Shading.h"
#include "geometry/ONB.h"

namespace RayTracingHistory {

/**
 * Glossy Material - GGX Microsfaced Model
 */
class GlossyMaterial : public MaterialBase {
  float Kd = 2;
  float roughness = 0.5f;

 public:
  virtual float evaluate(const glm::vec3& wi, const glm::vec3& wo,
                         const glm::vec3& normal) const override {
    float diffuse = Kd / glm::pi<float>();

    glm::vec3 H = glm::normalize(wi + wo);
    float NdotH = glm::max(0.0f, glm::dot(normal, H));
    float NdotL = glm::max(0.0f, glm::dot(normal, wi));
    float NdotV = glm::max(0.0f, glm::dot(normal, wo));
    float VdotH = glm::max(0.0f, glm::dot(wo, H));

    float specular = Shading::F_Schlick(VdotH) *
                     Shading::G_SmithJoint(roughness, NdotV, NdotL) *
                     Shading::D_GGX(roughness, NdotH);
    return diffuse + specular;
  }

  virtual SampleResult sample(const glm::vec3& wo,
                              const glm::vec3& normal) const override {
    glm::vec2 diskRand = glm::diskRand(1.0f);
    auto GGX = Shading::importanceSampleVisibleGGX(diskRand,
                                                   roughness, wo);

    SampleResult ret;

    glm::vec3 wi = std::get<0>(GGX);
    ONB onb(normal);
    ret.scattered = onb.localToWorld(wi);
    ret.pdf = std::get<1>(GGX);
    return ret;
  }
};

}  // namespace RayTracingHistory
