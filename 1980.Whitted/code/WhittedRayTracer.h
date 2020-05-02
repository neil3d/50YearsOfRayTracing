#pragma once
#include "MyRenderer.h"

class WhittedRayTracer : public MyRenderer {
 public:
  virtual void _renderThread() override;
};