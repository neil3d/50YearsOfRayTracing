/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "framework/TiledRenderer.h"

namespace RayTracingHistory {

/**
 * A simple Monte Carlo solution for direct lighting only
 */
class SimpleMonteCarloApproach : public TiledRenderer {
 private:
  virtual void _tileRenderThread(Tile tile, MyScene::Ptr scene,
                                 MyCamera::Ptr camera) override;

  glm::vec3 _shade(const Ray& wo, const HitRecord& shadingPoint,
                   MyScene* pScene, int depth);
};

}  // namespace RayTracingHistory
