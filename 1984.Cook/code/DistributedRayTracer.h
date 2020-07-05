/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <tuple>

#include "framework/TiledRenderer.h"

namespace RayTracingHistory {

class MySceneWithLight;

class DistributedRayTracer : public TiledRenderer {
 public:
  virtual void _tileRenderThread(Tile tile, MyScene::Ptr scene,
                                 MyCamera::Ptr camera) override;

  virtual std::string getInfo() const override;

 private:
  glm::vec3 _traceRay(const Ray& ray, MySceneWithLight* pScene, int depth,
                      const glm::vec2 xi, float weight);

  std::tuple<float, float> _shade(const glm::vec3& wo,
                                            const HitRecord& shadingPoint,
                                            MySceneWithLight* pScene,
                                            const glm::vec2 xi, float time);

  Ray _jitteredReflectionRay(const glm::vec3& dir, const glm::vec3& point,
                             const glm::vec3& normal, const glm::vec2 xi,
                             float glossy);

 private:
};

}  // namespace RayTracingHistory
