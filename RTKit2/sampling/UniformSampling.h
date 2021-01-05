/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>
#include <vector>

#include "utility/ONB.h"

namespace RTKit2 {

namespace UniformSampling {

inline std::vector<glm::vec2> generateSamples(int n) {
  std::vector<glm::vec2> ret(n);

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  for (auto& sample : ret) {
    sample.x = uniformDist(stdRand);
    sample.y = uniformDist(stdRand);
  }
  return ret;
}

inline glm::vec3 sampleHemisphere(const glm::vec3& normal) {
  constexpr float PI = glm::pi<float>();

#if 1
  float r1 = glm::linearRand(0.0f, 1.0f);
  float r2 = glm::linearRand(0.0f, 1.0f);

  float theta = glm::acos(r1);
  float phi = 2 * PI * r2;

  glm::vec3 localSample;
  localSample.x = glm::cos(phi) * glm::sin(theta);
  localSample.y = glm::sin(phi) * glm::sin(theta);
  localSample.z = glm::cos(theta);
#else
  glm::vec3 localSample = glm::sphericalRand(1.0f);
  localSample.z = glm::abs(localSample.z);
#endif

  ONB onb(normal);
  return onb.localToWorld(localSample);
}

}  // namespace UniformSampling
}  // namespace RTKit2
