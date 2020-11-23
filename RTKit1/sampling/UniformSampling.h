/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <random>
#include <vector>

namespace RTKit1 {

namespace UniformSampling {

std::vector<glm::vec2> generateSamples(int n) {
  std::vector<glm::vec2> ret(n * n);

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  for (auto& sample : ret) {
    sample.x = uniformDist(stdRand);
    sample.y = uniformDist(stdRand);
  }
  return ret;
}

}  // namespace UniformSampling
}  // namespace RTKit1
