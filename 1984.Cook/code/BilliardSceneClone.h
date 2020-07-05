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

#include "MySceneWithLight.h"

namespace RayTracingHistory {

class BilliardSceneClone : public MySceneWithLight {
 public:
  const AreaLight& getMainLight() const { return mLight; }

  virtual void init() override {
    constexpr float BALL_GLOSS = 0.75f;
    constexpr float BALL_KS = 0.55f;

    // setup a main light
    mLight
        .setParallelogram(glm::vec3(5, 0, 0), glm::vec3(0, 0, 5),
                          glm::vec3(0, 15, 0))
        .setAmbient(0.25f)
        .setIntensity(1.55f);

    // setup scene objects
    createObject<Plane>("table")
        .createMaterial<Material>()
        .setGloss(2.0f)
        .setCoefficient(0.0f)
        .setTexture2D("content/billiard/table.jpg")
        .setTiling(0.1f, 0.1f);

    glm::vec3 wPos(1.55f, 1, 1.55f);
    MySceneObject& whiteSphere = createObject<Sphere>("ball_white")
                                     .setCenter(wPos)
                                     .setRadius(1)
                                     .setRotation(0, 0, 0);

    whiteSphere.createMaterial<Material>()
        .setGloss(0.5f)
        .setCoefficient(BALL_KS)
        .setColor(glm::vec3(0.88f));
    whiteSphere.createAnimator(true).addKey(
        1, wPos + glm::vec3(-0.15f, 0, -0.15f), glm::vec3(0));

    createTexturedBall(glm::vec3(-1.5f, 1, -1.15f), glm::vec3(0, 0, 108), 0.5f,
                       "ball_8", BALL_GLOSS, BALL_KS);
    createTexturedBall(glm::vec3(-2.95f, 1, -2.95f), glm::vec3(10, 35, 55),
                       0.0f, "ball_4", BALL_GLOSS, BALL_KS);
    createTexturedBall(glm::vec3(-1.8f, 1, 0.85f), glm::vec3(90, 0, 33), 0.85f,
                       "ball_9", BALL_GLOSS, BALL_KS);
    createTexturedBall(glm::vec3(-2.8f, 1, 2.8f), glm::vec3(0, 0, 99), 0.1f,
                       "ball_5", BALL_GLOSS, BALL_KS);
  }
};
}  // namespace RayTracingHistory
