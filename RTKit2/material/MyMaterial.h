/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>

#include "MyTexture.h"

namespace RTKit2 {

class MyMaterial {
 public:
  typedef std::shared_ptr<MyMaterial> Ptr;

  MyMaterial() = default;
  virtual ~MyMaterial() = default;

 public:
  glm::vec3 diffuse = {1, 1, 1};
  glm::vec3 specular = {0, 0, 0};
  float shininess = 1;
};

}  // namespace RTKit2
