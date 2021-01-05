/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>
#include <glm/gtc/constants.hpp>
#include "MyTexture.h"

namespace RTKit2 {

class MyMaterial {
 public:
  typedef std::shared_ptr<MyMaterial> Ptr;

  MyMaterial() = default;
  virtual ~MyMaterial() = default;
};

}  // namespace RTKit2
