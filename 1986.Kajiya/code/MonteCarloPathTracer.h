/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include <tuple>
#include <vector>

#include "MySceneWithLight.h"
#include "framework/TiledRenderer.h"

namespace RayTracingHistory {

class PinholeCamera;

class MonteCarloPathTracer : public TiledRenderer {
 public:
  virtual void _tileRenderThread(MyScene::Ptr scene,
                                 MyCamera::Ptr camera) override;

  virtual std::string getInfo() const override;

 private:
  virtual void _init(SDL_Window* pWnd) override;

  glm::vec3 _traceRay(const Ray& wo, MySceneWithLight* pScene,
                      const glm::vec2& xi, float weight, int depth);

 private:
  std::string mInfo;
  std::atomic<int> mMaxDepth = {-1};
};
}  // namespace RayTracingHistory
