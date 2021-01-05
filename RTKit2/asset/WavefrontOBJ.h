
/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */
#pragma once

#include <vector>

#include "MyAssetObject.h"

namespace RTKit2 {
class WavefrontOBJ : public MyAssetObject {
 public:
  typedef std::shared_ptr<WavefrontOBJ> Ptr;

  using MyAssetObject::MyAssetObject;

  virtual void loadFromFile(const std::string& szPath) override;

 private:
};
}  // namespace RTKit1