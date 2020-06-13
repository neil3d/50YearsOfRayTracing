/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <random>
#include <vector>

namespace RayTracingHistory {

namespace JitteringSampling {

std::vector<glm::vec2> generateSamples(int n, bool shuffle) {
  std::vector<glm::vec2> ret(n * n);
  float invN = 1.0f / n;

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  // jittering/stratified sampling
  for (int p = 0; p < n; p++) {
    for (int q = 0; q < n; q++) {
      int index = p * n + q;
      ret[index] =
          invN * (glm::vec2(uniformDist(stdRand), uniformDist(stdRand)) +
                  glm::vec2(p, q));
    }  // end of for(q)
  }    // end of for(p)

  if (shuffle) std::shuffle(ret.begin(), ret.end(), stdRand);

  return ret;
}

}  // namespace JitteringSampling
}  // namespace RayTracingHistory
