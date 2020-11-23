/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/random.hpp>
#include <memory>

#include "MySceneWithLight.h"

namespace RTKit1 {

class BilliardSceneDemo : public MySceneWithLight {

 public:
  virtual void init() override {
    constexpr float BALL_GLOSS = 0.75f;
    constexpr float BALL_KS = 0.55f;
    constexpr float BALL_PS = 2.5f;
    // setup a main light
    mLight
        .setParallelogram(glm::vec3(5, 0, 0), glm::vec3(0, 0, 5),
                          glm::vec3(0, 15, 0))
        .setAmbient(0.25f)
        .setIntensity(2.f);

    // setup scene objects
    createObject<Plane>("table")
        .createMaterial<Material>()
        .setGloss(0.2f)
        .setShininess(4.0f)
        .setCoefficient(0.6f)
        .setCheckerTexture(glm::vec3(0, 0, 0), glm::vec3(0.4f, 0.4f, 0.4f));

    glm::vec3 wPos(2, 1, 2);
    MySceneObject& whiteSphere = createObject<Sphere>("ball_white")
                                     .setCenter(wPos)
                                     .setRadius(1)
                                     .setRotation(0, 0, 0);

    whiteSphere.createMaterial<Material>()
        .setGloss(0.5f)
        .setCoefficient(BALL_KS)
        .setColor(glm::vec3(0.88f));

    float PS = BALL_PS;
    createTexturedBall(glm::vec3(-1.5f * PS, 1, 1.15f * PS),
                       glm::vec3(0, 0, 108), 0.0f, "ball_8", BALL_GLOSS,
                       BALL_KS);
    createTexturedBall(glm::vec3(-2.75f * PS, 1, 2.75f), glm::vec3(10, -40, 55),
                       0.0f, "ball_4", BALL_GLOSS, BALL_KS);
    createTexturedBall(glm::vec3(-1.25f * PS, 1, -1.15f * PS),
                       glm::vec3(90, 0, 33), 0.0f, "ball_9", BALL_GLOSS,
                       BALL_KS);
    createTexturedBall(glm::vec3(-1.25f * PS, 1, -2.25f * PS),
                       glm::vec3(0, 0, 99), 0.f, "ball_5", BALL_GLOSS,
                       BALL_KS);
  }
};
}  // namespace RTKit1
