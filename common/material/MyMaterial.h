/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>

#include "MyTexture.h"

namespace RayTracingHistory {

class MyMaterial {
 public:
  typedef std::shared_ptr<MyMaterial> Ptr;

  MyMaterial() = default;
  virtual ~MyMaterial() = default;
};
}  // namespace RayTracingHistory
