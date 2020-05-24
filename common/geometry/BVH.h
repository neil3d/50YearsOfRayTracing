/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "AABBox.h"

namespace RayTracingHistory {

template <typename T>
struct BVHNode {
  T data;

  AABBox boundingBox;
  BVHNode leftChild;
  BVHNode rightChild;
};

template <typename T>
class BVHBuilder {};

}  // namespace RayTracingHistory
