#pragma once
#include "framework/DirectRenderer.h"
#include "scene/QuadLight.h"

namespace RTKit2 {
class AnalyticDirectIntegrator : public DirectRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  glm::vec3 _traceRay(MyRay ray, MyScene::Ptr scene);
  glm::vec3 _shade(glm::vec3 pt, glm::vec3 normal, glm::vec3 Kd,
                   QuadLight* lgt);
};
}  // namespace RTKit2
