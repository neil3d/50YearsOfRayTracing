/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "framework/DirectRenderer.h"

namespace RayTracingHistory {

/**
 * A simple Monte Carlo solution for direct lighting only
 */
class DirectLightingRenderer : public DirectRenderer {
 public:
  virtual bool isDone() const override;
  virtual float getProgress() const override;
  virtual std::string getInfo() const override;

private:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

  glm::vec3 _traceRay(Ray ray, MyScene* pScene, int depth);

 private:
    std::atomic<uint32_t> mSPP = {0};
  std::vector<glm::vec3> mBackBuffer;
};

}  // namespace RayTracingHistory
