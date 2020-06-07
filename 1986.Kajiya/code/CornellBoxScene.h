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
#include "LambertianMaterial.h"
#include "asset/MaterialImporter.h"
#include "scene/MeshInstance.h"
#include "scene/MyScene.h"
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

    if (szName == "light") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(1)).setEmission(100.0f).enableLight();
      return mtl;
    }
  }
};

class CornellBoxScene : public MyScene {
 public:
  virtual void init() override {
    const char* const szFileName = "content/cornell_box/cornell_box.obj";
    // const char* const szFileName = "content/tank/tank.obj";

    auto& mesh = createObject<MeshInstance>("mesh");
    mesh.setMeshFile(szFileName);

    CornellBoxMtlImporter mtlImporter;
    mesh.importMaterial(&mtlImporter);

// add a ball
#if 0
    constexpr float BALL_RADIUS = 50;
    constexpr float BALL_Y = 165 + BALL_RADIUS;
    constexpr float BALL_X = 240 - BALL_RADIUS;

    createObject<Sphere>("ball")
        .setCenter(glm::vec3(BALL_X, BALL_Y, 0))
        .setRadius(BALL_RADIUS)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));
#endif
  }
};
}  // namespace RayTracingHistory
