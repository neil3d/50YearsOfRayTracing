#pragma once
#include <memory>

class MyMaterial {
 public:
  typedef std::shared_ptr<MyMaterial> Ptr;

  MyMaterial() = default;
  virtual ~MyMaterial() = default;
};