/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>

#include "AreaLight.h"
#include "scene/MyScene.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class BilliardScene : public MyScene {
  AreaLight mLight;

 public:
  const AreaLight& getMainLight() const { return mLight; }

  virtual void init() override {
    // setup a main light
    mLight
        .setParallelogram(glm::vec3(5, 0, 0), glm::vec3(0, 0, 5),
                          glm::vec3(0, 10, 0))
        .setAmbient(0.2f)
        .setIntensity(0.9f);

    // setup scene objects
    createObject<Plane>("ground");

    createObject<Sphere>("ball1").setCenter(glm::vec3(0, 1, 0)).setRadius(1);
    createObject<Sphere>("ball2").setCenter(glm::vec3(2.5f, 1, 5)).setRadius(1);
    createObject<Sphere>("ball3").setCenter(glm::vec3(-2.5f, 1, 3)).setRadius(1);

  }
};
}  // namespace RayTracingHistory
