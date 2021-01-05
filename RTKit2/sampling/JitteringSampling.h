/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <random>
#include <vector>

namespace RTKit2 {

namespace JitteringSampling {

inline std::vector<glm::vec2> generateSamples(int count, bool shuffle) {
  int n = (int)sqrt(count);
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

inline std::vector<glm::vec2> stratifyUniformSamples(
    const std::vector<glm::vec2>& samples) {
  int M = (int)sqrt(samples.size());
  std::vector<glm::vec2> ret(M * M);

  for (int j = 0; j < M; j++) {
    for (int i = 0; i < M; i++) {
      int index = j * M + i;
      const auto& u = samples[index];
      ret[index] = glm::vec2((j + u.x) / (float)M, (i + u.y) / (float)M);
    }
  }
  return ret;
}

}  // namespace JitteringSampling
}  // namespace RTKit2
