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
#include "Material.h"
#include "scene/MyScene.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

constexpr float BALL_GLOSS = 0.65f;
constexpr float BALL_KS = 0.45f;

class BilliardScene : public MyScene {
  AreaLight mLight;

 public:
  const AreaLight& getMainLight() const { return mLight; }

  void createTexturedBall(glm::vec3 pos, const std::string& szName) {
    std::string szTexture("content/billiard/");
    szTexture.append(szName);
    szTexture.append(".jpg");

    createObject<Sphere>(szName)
        .setCenter(pos)
        .setRadius(1)
        .createMaterial<Material>()
        .setGloss(BALL_GLOSS)
        .setCoefficient(BALL_KS)
        .setTexture2D(szTexture);
  }

  virtual void init() override {
    // setup a main light
    mLight
        .setParallelogram(glm::vec3(5, 0, 0), glm::vec3(0, 0, 5),
                          glm::vec3(0, 15, 0))
        .setAmbient(0.05f)
        .setIntensity(2.f);

    // setup scene objects
    createObject<Plane>("ground")
        .createMaterial<Material>()
        .setGloss(0.2)
        .setCoefficient(0.6f)
        .setCheckerTexture(glm::vec3(0, 0, 0), glm::vec3(0.4f, 0.4f, 0.4f));

    createObject<Sphere>("ball_white")
        .setCenter(glm::vec3(0, 1, 0))
        .setRadius(1)
        .createMaterial<Material>()
        .setGloss(BALL_GLOSS)
        .setCoefficient(BALL_KS)
        .setColor(glm::vec3(0.88f));

    createTexturedBall(glm::vec3(-2.5f, 1, 1.5f), "ball_8");
    createTexturedBall(glm::vec3(-3.75f, 1, 3.5f), "ball_4");
    createTexturedBall(glm::vec3(-2.25f, 1, -1.15f), "ball_1");
    createTexturedBall(glm::vec3(-2.5f, 1, -3.5f), "ball_5");
  }
};
}  // namespace RayTracingHistory
