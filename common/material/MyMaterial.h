#pragma once
#include <memory>

#include "MyTexture.h"

class MyMaterial {
 public:
  typedef std::shared_ptr<MyMaterial> Ptr;

  MyMaterial() = default;
  virtual ~MyMaterial() = default;
};