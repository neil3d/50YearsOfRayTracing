/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include "DiffuseMaterial.h"
#include "scene/CornellBox.h"

namespace RayTracingHistory {

class CornellBoxDemo : public CornellBox {
 public:
  virtual void onObjectCreated(MySceneObject* newObject) {
    if (newObject->getName() == "light") {
      newObject->createMaterial<DiffuseMaterial>()
          .setEmission(1.0f)
          .enableLight();
    } else if (newObject->getName() == "left_wall") {
      newObject->createMaterial<DiffuseMaterial>().setColor(glm::vec3(0, 1, 0));

    } else if (newObject->getName() == "right_wall") {
      newObject->createMaterial<DiffuseMaterial>().setColor(glm::vec3(1, 0, 0));
    } else if (newObject->getName() == "tall_box") {
      newObject->createMaterial<DiffuseMaterial>().setColor(glm::vec3(1, 1, 1));
    } else {
      newObject->createMaterial<DiffuseMaterial>();
    }
  }
};

}  // namespace RayTracingHistory
