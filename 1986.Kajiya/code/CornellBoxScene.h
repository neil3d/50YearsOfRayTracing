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
#include "MySceneWithLight.h"
#include "ParallelogramLight.h"
#include "asset/MaterialImporter.h"
#include "scene/MeshInstance.h"
#include "scene/Parallelogram.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class CornellBoxMtlImporter : public MaterialImporter {
  virtual MyMaterial::Ptr importObj(const std::string& szName) override {
    if (szName == "white") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(1));
      return mtl;
    }

    if (szName == "red") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(1, 0, 0));
      return mtl;
    }

    if (szName == "green") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(0, 1, 0));
      return mtl;
    }

    if (szName == "blue") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(0, 0, 1));
      return mtl;
    }

    // TODO
    /*if (szName == "light") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(1)).setEmission(100.0f).enableLight();
      return mtl;
    }*/

    return std::make_shared<DiffuseMaterial>();
  }
};

class CornellBoxScene : public MySceneWithLight {
  ParallelogramLight mMainLight;

  void _initLight() {
    float h = 0.1f;
    glm::vec3 v1(343.0f, 548.0f - h, 227.0f);
    glm::vec3 v2(343.0f, 548.0f - h, 332.0f);
    glm::vec3 v3(213.0f, 548.0f - h, 332.0f);
    glm::vec3 v4(213.0f, 548.0f - h, 227.0f);

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v4 - v1;
    mMainLight.setShape(edge2, edge1, v1);

    createObject<Parallelogram>("light_shape")
        .setEdges(edge1, edge2)
        .setAnchor(v1)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1))
        .enableLight();
  }

 public:
  virtual const AreaLight* getMainLight() const override { return &mMainLight; }

  virtual void init() override {
    const char* const szFileName = "content/cornell_box/cornell_box.obj";

    auto& mesh = createObject<MeshInstance>("mesh");
    mesh.setMeshFile(szFileName);

    CornellBoxMtlImporter mtlImporter;
    mesh.importMaterial(&mtlImporter);

    _initLight();

// add a ball
#if 1
    constexpr float BALL_RADIUS = 50;
    constexpr float BALL_Z = 114;
    constexpr float BALL_X = 290 + BALL_RADIUS * 2;
    constexpr float BALL_S = 0.2f;

    auto& tallEllipsoid = createObject<Sphere>("tall_ellipsoid")
                              .setCenter(glm::vec3(BALL_X, 0, BALL_Z))
                              .setRadius(BALL_RADIUS / BALL_S)
                              .setScale(glm::vec3(BALL_S, 1.0f, BALL_S));
    tallEllipsoid.createMaterial<DiffuseMaterial>().setColor(glm::vec3(1));

    auto& shortEllipsoid =
        createObject<Sphere>("short_ellipsoid")
            .setCenter(glm::vec3(BALL_X, BALL_RADIUS * 3.8f, BALL_Z))
            .setRadius(BALL_RADIUS * 1.5f)
            .setScale(glm::vec3(1.5f, BALL_S*0.5f, 1.5f));
    shortEllipsoid.createMaterial<DiffuseMaterial>().setColor(glm::vec3(1));
#endif
  }
};
}  // namespace RayTracingHistory
