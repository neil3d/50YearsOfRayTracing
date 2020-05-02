#pragma once
#include "MyScene.h"

class SpheresScene : public MyScene {
 public:
  typedef std::shared_ptr<SpheresScene> Ptr;

  virtual void init() override;
};