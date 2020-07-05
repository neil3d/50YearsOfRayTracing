/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include "AreaLight.h"
#include "Material.h"
#include "scene/MyScene.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class MySceneWithLight : public MyScene {
 public:
  const AreaLight& getMainLight() const { return mLight; }

  void createTexturedBall(glm::vec3 pos, glm::vec3 degAngles, float animRatio,
                          const std::string& szName, float gloss, float Ks) {
    std::string szTexture("content/billiard/");
    szTexture.append(szName);
    szTexture.append(".jpg");

    constexpr float PI = glm::pi<float>();
    glm::vec3 rot = glm::radians(degAngles);
    glm::vec3 animRot = glm::radians(glm::vec3(8, 6, 9));
    glm::vec3 animOffset(0.35f, 0, 0.35f);

    MySceneObject& newSphere =
        createObject<Sphere>(szName).setCenter(pos).setRadius(1).setRotation(
            rot.x, rot.y, rot.z);
    newSphere.createMaterial<Material>()
        .setGloss(gloss)
        .setCoefficient(Ks)
        .setTexture2D(szTexture);
    if (animRatio > 0.0f) {
      newSphere.createAnimator(true).addKey(1.0f, pos + animOffset * animRatio,
                                            rot + animRot * animRatio);
    }
  }

 protected:
  AreaLight mLight;
};
}  // namespace RayTracingHistory
