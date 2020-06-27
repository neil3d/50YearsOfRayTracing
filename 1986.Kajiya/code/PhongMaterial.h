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
class PhongMaterial : public MaterialBase {
 public:
  float Kd = 0.5f;
  float Ks = 2;
  float Shiness = 20;

 public:
  virtual float evaluate(const glm::vec3& wi, const glm::vec3& wo,
                         const glm::vec3& normal) const override {
    glm::vec3 H = glm::normalize(wi - wo);
    float NdotL = glm::max(0.0f, glm::dot(normal, wi));
    float NdotH = glm::max(0.0f, glm::dot(normal, H));

    float diffuse = NdotL * Kd;
    float specular = std::powf(NdotH, Shiness) * Ks;
    return diffuse + specular;
  }

  virtual SampleResult sample(const glm::vec3& wo,
                              const glm::vec3& normal) const override {
    float r0 = glm::linearRand(0.0f, 1.0f);
    float r1 = glm::linearRand(0.0f, 1.0f);
    float r2 = glm::linearRand(0.0f, 1.0f);
    float phi = 2 * glm::pi<float>() * r2;
    float theta = 0.0f;

    float diffuse = Kd / (Kd + Ks);
    if (diffuse > r0) {
      // for diffuse
      theta = glm::acos(sqrt(r1));

    } else {
      // for specular
      theta = glm::acos(pow(r1, 1 / (Shiness + 1)));
    }

    glm::vec3 scattered;
    scattered.x = sin(phi) * cos(theta);
    scattered.y = sin(phi) * sin(theta);
    scattered.z = cos(phi);

    glm::vec3 R = glm::reflect(glm::normalize(-wo), normal);
    ONB onb(R);

    SampleResult ret;
    ret.scattered = onb.localToWorld(scattered);

    // TODO
    ret.pdf = 1.0f / (2.0f * glm::pi<float>());
    return ret;
  }
};

}  // namespace RayTracingHistory
