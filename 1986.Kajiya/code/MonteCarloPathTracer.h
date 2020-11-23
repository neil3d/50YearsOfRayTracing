/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include <tuple>
#include <vector>

#include "MySceneWithLights.h"
#include "framework/TiledRenderer.h"

namespace RTKit1 {

class PinholeCamera;

class MonteCarloPathTracer : public TiledRenderer {
 public:
  virtual void _tileRenderThread(MyScene::Ptr scene,
                                 MyCamera::Ptr camera) override;

  virtual std::string getInfo() const override;

 private:
  virtual void _init(SDL_Window* pWnd) override;

  glm::vec3 _traceRay(const Ray& wo, MySceneWithLights* pScene,
                      const glm::vec2& xi, float weight, int depth);

 private:
  std::string mInfo;
  std::atomic<int> mMaxDepth = {-1};
};
}  // namespace RTKit1
