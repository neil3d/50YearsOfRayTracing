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

#include "framework/TiledRenderer.h"

namespace RayTracingHistory {

class PinholeCamera;

class MonteCarloPathTracer : public TiledRenderer {
 public:
  virtual void _tileRenderThread(Tile tile, MyScene::Ptr scene,
                                 MyCamera::Ptr camera) override;

  virtual std::string getInfo() const override;
  virtual float getProgress() const override;
  virtual bool isDone() const;

 private:
  virtual void _init(SDL_Window* pWnd) override;

  glm::vec3 _traceRay(const Ray& wo, MyScene* pScene);

 private:
  std::string mInfo;
};
}  // namespace RayTracingHistory
