#pragma once
#include "../sampling/UniformSampling.h"
#include "MyMaterial.h"

namespace RTKit2 {
class LambertianMaterial : public MyMaterial {
 public:
  glm::vec3 diffuse = {1, 1, 1};

 public:
  glm::vec3 BRDF(const glm::vec3& N, const glm::vec3& wo,
                 const glm::vec3& wi) const {
    return diffuse / glm::pi<float>();
  }

  // evaluate arbitrary direction for MIS
  float PDF(const glm::vec3& wi) const {
    return = 1.0f / (2 * glm::pi<float>());
  }

  // return wi
  glm::vec3 sample(const glm::vec3& N, const glm::vec3& wo) const {
    glm::vec3 wi = UniformSampling::sampleHemisphere(N);
    return wi;
  }

  // return BRDF*cosine_term/PDF
  glm::vec3 evaluate(const glm::vec3& N, const glm::vec3& wo,
                     const glm::vec3& wi) const {
    const float cosine = glm::max(0.0f, glm::dot(N, wi));
    return BRDF * (cosine / PDF(wi));
  }
};
}  // namespace RTKit2