#pragma once

#include <glm/glm.hpp>

namespace RTKit2 {

// https://en.wikipedia.org/wiki/Phong_reflection_model
inline glm::vec3 myReflect(glm::vec3 Lm, glm::vec3 N) {
  return N * (2.0f * glm::dot(Lm, N)) - Lm;
}

// Modified Phong reflection model, 1994 Eric Lafortune
inline glm::vec3 modifiedPhong(const glm::vec3& Kd, const glm::vec3& Ks,
                               float s, glm::vec3 wo, glm::vec3 normal,
                               glm::vec3 wi) {
  constexpr float PI = glm::pi<float>();

  glm::vec3 r = myReflect(wo, normal);
  float d = glm::max(0.0f, glm::dot(r, wi));

  glm::vec3 diffuse = Kd / PI;
  glm::vec3 specular = Ks * ((s + 2) / (2 * PI) * std::pow(d, s));

  return diffuse + specular;
}

}  // namespace RTKit2