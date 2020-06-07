/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include "Material.h"
#include "scene/CornellBox.h"

namespace RayTracingHistory {

class CornellBoxDemo : public CornellBox {
  MySceneObject* mLight = nullptr;

 public:
  MySceneObject* getLight() const { return mLight; }

  virtual void onObjectCreated(MySceneObject* newObject) {
    if (newObject->getName() == "light") {
      
    } else if (newObject->getName() == "left_wall") {
      
    } else if (newObject->getName() == "right_wall") {
      
    } else {
      
    }
  }
};

}  // namespace RayTracingHistory
