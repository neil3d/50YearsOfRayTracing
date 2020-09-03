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
#include "MySceneWithLights.h"
#include "ParallelogramLight.h"
#include "PhongMaterial.h"
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

class CornellBoxScene : public MySceneWithLights {
  void _initLight() {
    float h = 0.1f;
    glm::vec3 v1(343.0f, 548.0f - h, 227.0f);
    glm::vec3 v2(343.0f, 548.0f - h, 332.0f);
    glm::vec3 v3(213.0f, 548.0f - h, 332.0f);
    glm::vec3 v4(213.0f, 548.0f - h, 227.0f);

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v4 - v1;

    auto keyLight = std::make_shared<ParallelogramLight>();
    keyLight->setShape(edge2, edge1, v1);
    mLights.push_back(keyLight);

    createObject<Parallelogram>("light_shape")
        .setEdges(edge1, edge2)
        .setAnchor(v1)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1))
        .enableLight();
  }

 public:
  virtual void init() override {
    const char* const szFileName = "content/cornell_box/cornell_box.obj";

    auto& mesh = createObject<MeshInstance>("mesh");
    mesh.setMeshFile(szFileName);

    CornellBoxMtlImporter mtlImporter;
    mesh.importMaterial(&mtlImporter);

    _initLight();

    const glm::vec3 GOLD(205 / 255.0f, 127 / 255.0f, 50 / 255.0f);

#if 1  // add teapot
    const char* const szTeapotFileName = "content/teapot/teapot.obj";
    auto& teapot = createObject<MeshInstance>("teapot");
    teapot.setMeshFile(szTeapotFileName)
        .createMaterial<PhongMaterial>()
        .setColor(GOLD);
    teapot.setPosition(glm::vec3(185.5f, 165, 169)).setScale(100);
#endif
  }
};
}  // namespace RayTracingHistory
