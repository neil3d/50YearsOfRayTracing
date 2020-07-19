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

constexpr glm::vec3 GOLD(205 / 255.0f, 127 / 255.0f, 50 / 255.0f);
constexpr glm::vec3 YELLOW(1, 1, 0);

class MoriKnobMtlImporter : public MaterialImporter {
  virtual MyMaterial::Ptr importObj(const std::string& szName) override {
    if (szName == "BackGroundMat") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(0.25f));
      return mtl;
    }

    if (szName == "InnerMat") {
      auto mtl = std::make_shared<DiffuseMaterial>();
      mtl->setColor(glm::vec3(0.5f));
      return mtl;
    }

    if (szName == "LTELogo") {
      auto mtl = std::make_shared<LambertianMaterial>();
      mtl->setColor(YELLOW);
      return mtl;
    }

    if (szName == "OuterMat") {
      auto mtl = std::make_shared<PhongMaterial>();
      mtl->setColor(GOLD);
      return mtl;
    }

    return std::make_shared<DiffuseMaterial>();
  }
};

class MoriKnobScene : public MySceneWithLights {

  void _initLight() {
    glm::vec3 v1(3.75, 12, -3.75);
    glm::vec3 v2(-3.75, 12, 3.75);
    glm::vec3 v3(-3.75, 12, -3.75);
    glm::vec3 v4(3.75, 12, 3.75);

    glm::vec3 edge1 = v3 - v1;
    glm::vec3 edge2 = v4 - v1;
    auto mainLight = std::make_shared<ParallelogramLight>();
    mainLight->setShape(edge1, edge2, v1);
    mLights.push_back(mainLight);

#if 0
    createObject<Parallelogram>("light_shape")
        .setEdges(edge1, edge2)
        .setAnchor(v1)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1))
        .enableLight();
#endif
  }

 public:

  virtual float systemUnit() const { return 1; }

  virtual void init() override {
    MoriKnobMtlImporter mtlImporter;

    auto& background = createObject<MeshInstance>("background");
    background.setMeshFile("content/mori_knob/background.obj");
    background.importMaterial(&mtlImporter);

    auto& equation = createObject<MeshInstance>("equation");
    equation.setMeshFile("content/mori_knob/equation.obj");
    equation.importMaterial(&mtlImporter);

#if 0
    auto& knob = createObject<MeshInstance>("knob");
    knob.setMeshFile("content/mori_knob/knob.obj");
    knob.importMaterial(&mtlImporter);
#endif

#if 1  // stanford dragon
    auto& mesh = createObject<MeshInstance>("dragon");
    mesh.setMeshFile("content/dragon/dragon.obj");
    mesh.createMaterial<PhongMaterial>().setColor(GOLD);
    mesh.setScale(0.1f).setPosition(glm::vec3(0, -0.25f, 0));
#endif

    _initLight();
  }
};
}  // namespace RayTracingHistory
