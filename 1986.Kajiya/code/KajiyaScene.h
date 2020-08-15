/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>

#include "DiffuseMaterial.h"
#include "MySceneWithLights.h"
#include "ParallelogramLight.h"
#include "geometry/ONB.h"
#include "scene/Box.h"
#include "scene/MeshInstance.h"
#include "scene/Parallelogram.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class KajiyaScene : public MySceneWithLights {
 public:
  virtual float systemUnit() const override { return 1000; }

  virtual void init() override {
    // create objects
    constexpr float W = 1500;
    constexpr float D = 1800;
    constexpr float H = 1500;
    constexpr float LS = 500;  // light size

    glm::vec3 lightPos(-W / 2, H * 1.5f, 0);
    glm::vec3 lightDir = glm::normalize(lightPos);
    ONB lightONB(lightDir);

    glm::vec3 lightEdge1 = lightONB.U * LS;
    glm::vec3 lightEdge2 = lightONB.V * LS;
    auto keyLight = std::make_shared<ParallelogramLight>();
    keyLight->setShape(lightEdge1, lightEdge2, lightPos);
    keyLight->setIntensity(500);
    mLights.push_back(keyLight);

    createObject<Parallelogram>("floor")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    const glm::vec3 boxExtends(W / 5, H / 25, D / 5);
    const glm::vec3 tallBoxExtends = boxExtends * glm::vec3(0.5f, 6.0f, 0.5f);
    const float RADIUS = tallBoxExtends.x * 0.45f;
    // right box
    glm::vec3 boxPos = glm::vec3(-W / 4, 0, -D / 4);
    _createBox(boxExtends, boxPos);

    constexpr float ELLIPS_RADIUS = 77;
    constexpr float ELLIPS_S = 0.2f;

    auto& tallEllipsoid = createObject<Sphere>("tall_ellipsoid")
                              .setCenter(boxPos + glm::vec3(0, boxExtends.y, 0))
                              .setRadius(ELLIPS_RADIUS / ELLIPS_S)
                              .setScale(glm::vec3(ELLIPS_S, 1.0f, ELLIPS_S));
    tallEllipsoid.createMaterial<DiffuseMaterial>().setColor(glm::vec3(1));

    auto& shortEllipsoid =
        createObject<Sphere>("disk")
            .setCenter(boxPos +
                       glm::vec3(0, boxExtends.y + ELLIPS_RADIUS * 3.8f, 0))
            .setRadius(ELLIPS_RADIUS * 1.5f)
            .setScale(glm::vec3(1.5f, ELLIPS_S * 0.5f, 1.5f));
    shortEllipsoid.createMaterial<DiffuseMaterial>().setColor(glm::vec3(1));

    // left near box
    boxPos = glm::vec3(W / 4, 0, -D / 5);
    _createBox(boxExtends, boxPos);
    _createBox(tallBoxExtends, boxPos + glm::vec3(0, boxExtends.y, 0));
    _createSphere(
        RADIUS,
        boxPos + glm::vec3(0, boxExtends.y + tallBoxExtends.y + RADIUS, 0));

    // left rear box
    boxPos = glm::vec3(W / 4, 0, D / 3.5f);
    _createBox(boxExtends, boxPos);
    _createBox(tallBoxExtends, boxPos + glm::vec3(0, boxExtends.y, 0));
    _createSphere(
        RADIUS,
        boxPos + glm::vec3(0, boxExtends.y + tallBoxExtends.y + RADIUS, 0));

    // create sphere stack
    const glm::vec3 stackPos(RADIUS * 1.5f, 0, -RADIUS * 3.5f);
    for (int h = 0; h < 3; h++) {
      for (int m = 1; m <= 3 - h; m++) {
        _createLineOfSpheres(
            m, RADIUS,
            stackPos + glm::vec3(-RADIUS * m, h * RADIUS * 2 + RADIUS,
                                 RADIUS * m * 2 + h * RADIUS));
      }
    }  // end of for
  }

  void _createLineOfSpheres(int count, float radius, glm::vec3 pos) {
    for (int i = 0; i < count; i++) {
      _createSphere(radius, pos + glm::vec3(i * radius * 2, 0, 0));
    }
  }

  void _createSphere(float radius, glm::vec3 pos) {
    createObject<Sphere>("sphere")
        .setCenter(pos)
        .setRadius(radius)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));
  }

  void _createBox(glm::vec3 extends, glm::vec3 pos) {
    auto& box =
        createObject<Box>("box").setExtents(extends.x, extends.y, extends.z);
    box.setPosition(pos).createMaterial<DiffuseMaterial>().setColor(
        glm::vec3(1));

    box.init();
  }
};

}  // namespace RayTracingHistory
