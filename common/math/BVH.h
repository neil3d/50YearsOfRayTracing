#pragma once
#include "AABBox.h"

template <typename T>
struct BVHNode {
  T data;

  AABBox boundingBox;
  BVHNode leftChild;
  BVHNode rightChild;
};

template <typename T>
class BVHBuilder {};
