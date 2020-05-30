/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/random.hpp>
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

  void createTexturedBall(glm::vec3 pos, glm::vec3 degAngles,
                          const std::string& szName) {
    std::string szTexture("content/billiard/");
    szTexture.append(szName);
    szTexture.append(".jpg");

    glm::vec3 R = glm::radians(degAngles);

    createObject<Sphere>(szName)
        .setCenter(pos)
        .setRadius(1)
        .setRotation(R.x, R.y, R.z)
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
    createObject<Plane>("table")
        .createMaterial<Material>()
#if 0
        .setGloss(0.2)
        .setCoefficient(0.6f)
        .setCheckerTexture(glm::vec3(0, 0, 0), glm::vec3(0.4f, 0.4f, 0.4f));
#else
        .setGloss(2.0f)
        .setCoefficient(0.0f)
        .setTexture2D("content/billiard/table.jpg")
        .setTiling(0.1f, 0.1f);
#endif

            createObject<Sphere>("ball_white")
        .setCenter(glm::vec3(0, 1, 0))
        .setRadius(1)
        .setRotation(0, 0, 0)
        .createMaterial<Material>()
        .setGloss(BALL_GLOSS)
        .setCoefficient(BALL_KS)
        .setColor(glm::vec3(0.88f));

    createTexturedBall(glm::vec3(-2.5f, 1, 1.5f), glm::vec3(0, 0, 78),
                       "ball_8");
    createTexturedBall(glm::vec3(-3.75f, 1, 3.5f), glm::vec3(0, 0, 88),
                       "ball_4");
    createTexturedBall(glm::vec3(-2.25f, 1, -1.15f), glm::vec3(0, 0, 99),
                       "ball_9");
    createTexturedBall(glm::vec3(-2.5f, 1, -3.5f), glm::vec3(0, 0, 55),
                       "ball_5");
  }
};
}  // namespace RayTracingHistory
