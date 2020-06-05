/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

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


 private:
  glm::vec3 _rayGeneration(PinholeCamera* pCamera, float pixelX, float pixelY,
                           MyScene* pScene);

  glm::vec3 _shade(const Ray& wo, const HitRecord& shadingPoint,
                   MyScene* pScene, int depth);
  
};
}  // namespace RayTracingHistory
