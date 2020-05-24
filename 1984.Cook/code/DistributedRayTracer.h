#pragma once
#include "framework/MyRenderer.h"

class DistributedRayTracer : public MyRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

};