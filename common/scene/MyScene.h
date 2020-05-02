#pragma once
#include <list>
#include <memory>

#include "MySceneObject.h"

class MyScene {
 public:
  typedef std::shared_ptr<MyScene> Ptr;

 protected:
  std::list<MySceneObject::Ptr> mObjects;
};