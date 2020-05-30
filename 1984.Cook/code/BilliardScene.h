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

#define SCENE_A false

namespace RayTracingHistory {

constexpr float BALL_GLOSS = 0.75f;
constexpr float BALL_KS = 0.55f;
#ifdef SCENE_A
constexpr float BALL_PS = 1.75f;
#else
constexpr float BALL_PS = 1.0f;
#endif

class BilliardScene : public MyScene {
  AreaLight mLight;

 public:
  const AreaLight& getMainLight() const { return mLight; }

  void createTexturedBall(glm::vec3 pos, glm::vec3 degAngles, float animScale,
                          const std::string& szName) {
    std::string szTexture("content/billiard/");
    szTexture.append(szName);
    szTexture.append(".jpg");

    glm::vec3 R = glm::radians(degAngles);
    glm::vec3 animRot(4, 5, 6);
    glm::vec3 animOffset(0.5f, 0, 0.5f);

    MySceneObject& newSphere =
        createObject<Sphere>(szName).setCenter(pos).setRadius(1).setRotation(
            R.x, R.y, R.z);
    newSphere.createMaterial<Material>()
        .setGloss(BALL_GLOSS)
        .setCoefficient(BALL_KS)
        .setTexture2D(szTexture);

    if (animScale > 0.0f) {
      newSphere.createAnimator(true).addKey(1.0f, pos + animOffset * animScale,
                                            animRot * animScale);
    }
  }

  virtual void init() override {
    // setup a main light
    mLight
        .setParallelogram(glm::vec3(5, 0, 0), glm::vec3(0, 0, 5),
                          glm::vec3(0, 15, 0))
        .setAmbient(0.15f)
        .setIntensity(2.f);

    // setup scene objects
    createObject<Plane>("table")
        .createMaterial<Material>()
#if SCENE_A
        .setGloss(0.2)
        .setCoefficient(0.6f)
        .setCheckerTexture(glm::vec3(0, 0, 0), glm::vec3(0.4f, 0.4f, 0.4f));
#else
        .setGloss(2.0f)
        .setCoefficient(0.0f)
        .setTexture2D("content/billiard/table.jpg")
        .setTiling(0.1f, 0.1f);
#endif

#if SCENE_A
    glm::vec3 wPos(2, 1, 2);
#else
    glm::vec3 wPos(0.5f, 1, 0.5f);
#endif

    MySceneObject& whiteSphere = createObject<Sphere>("ball_white")
                                     .setCenter(wPos)
                                     .setRadius(1)
                                     .setRotation(0, 0, 0);

    whiteSphere.createMaterial<Material>()
        .setGloss(0.5f)
        .setCoefficient(BALL_KS)
        .setColor(glm::vec3(0.88f));
    whiteSphere.createAnimator(true).addKey(1.0f, wPos + glm::vec3(-0.55f, 0, -0.55f),
                                            glm::vec3(0));

    float PS = BALL_PS;
    createTexturedBall(glm::vec3(-1.5f * PS, 1, 1.15f * PS),
                       glm::vec3(0, 0, 108), 0.0f, "ball_8");
    createTexturedBall(glm::vec3(-2.75f * PS, 1, 2.75f), glm::vec3(10, -40, 55),
                       0.0f, "ball_4");
    createTexturedBall(glm::vec3(-1.25f * PS, 1, -0.15f * PS),
                       glm::vec3(90, 0, 33), 1.0f, "ball_9");
    createTexturedBall(glm::vec3(-1.25f * PS, 1, -1.25f * PS),
                       glm::vec3(0, 0, 99), 0.5f, "ball_5");
  }
};
}  // namespace RayTracingHistory
