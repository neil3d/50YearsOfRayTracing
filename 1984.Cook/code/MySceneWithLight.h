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

  void createTexturedBall(glm::vec3 pos, glm::vec3 degAngles, float animScale,
                          const std::string& szName, float gloss, float Ks) {
    std::string szBasePath("./");
#ifdef WIN32
    // TODO£ºFIX ME
    szBasePath = "../../../../../";
#endif  // WIN32

    std::string szTexture;
    szTexture.append(szBasePath);
    szTexture.append("content/billiard/");
    szTexture.append(szName);
    szTexture.append(".jpg");

    constexpr float PI = glm::pi<float>();
    glm::vec3 R = glm::radians(degAngles);
    glm::vec3 animRot(PI/100, PI/80, PI/120);
    glm::vec3 animOffset(0.5f, 0, 0.5f);

    MySceneObject& newSphere =
        createObject<Sphere>(szName).setCenter(pos).setRadius(1).setRotation(
            R.x, R.y, R.z);
    newSphere.createMaterial<Material>()
        .setGloss(gloss)
        .setCoefficient(Ks)
        .setTexture2D(szTexture);
    if (animScale > 0.0f) {
      newSphere.createAnimator(true).addKey(1.0f, pos + animOffset * animScale,
                                            animRot * animScale);
    }
  }

 protected:
  AreaLight mLight;
};
}  // namespace RayTracingHistory
