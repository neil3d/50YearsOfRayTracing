#pragma once
#include "framework/MyRenderer.h"

class WhittedRayTracer : public MyRenderer {
 public:
  virtual void _renderThread() override;
};