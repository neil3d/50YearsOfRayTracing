/**
 * A Brief History of Ray tracing
 * 
 * GitHub: https://github.com/neil3d/RayTracingHistory
 * 
 * Created by yanliang.fyl, 2020
 */ 
#pragma once

#include "MyAssetObject.h"

namespace RayTracingHistory {
class Image : public MyAssetObject {
 public:
  typedef std::shared_ptr<Image> Ptr;
};
}  // namespace RayTracingHistory