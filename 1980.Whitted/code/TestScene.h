/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/random.hpp>

#include "TestSceneBase.h"
#include "asset/MaterialImporter.h"
#include "scene/MeshInstance.h"
#include "scene/Plane.h"

namespace RayTracingHistory {

class MyMtlImporter : public MaterialImporter {
  virtual MyMaterial::Ptr importObj(const std::string& szName) override {
    if (szName == "cup_material") {
      auto mtl = std::make_shared<Material>();
      mtl->setParam(0.2f, 0.2f, 0.6f, 160.0f, 1.33f);
      mtl->setColor(glm::vec3(1));
      return mtl;
    }

    auto mtl = std::make_shared<Material>();
    mtl->setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);
    mtl->setColor(glm::vec3(0.8f, 0.6f, 0.1f));
    return mtl;
  }
};

class TestScene : public TestSceneBase {
 public:
  virtual void init() override {
    _createLights(false, true);

    constexpr float W = 12;
    constexpr float H = 12;
    createObject<Parallelogram>("floor")
        .setEdges(glm::vec3(0, 0, H), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, H / -2))
        .createMaterial<Material>()
        .setCheckerTexture(glm::vec3(0.32f), glm::vec3(0.68f))
        .setTiling(0.01f, 0.01f)
        .setParam(1, 0, 0, 60, 1);

#if 1
    const char* const szFileName = "content/cup/cup.obj";
    auto& mesh = createObject<MeshInstance>("cup");
    mesh.setMeshFile(szFileName);

    MyMtlImporter mtlImporter;
    mesh.importMaterial(&mtlImporter);
#else
    // create spheres
    constexpr float D = 4;
    constexpr int COUNT = 3;

    for (int y = 0; y < COUNT; y++)
      for (int x = 0; x < COUNT; x++) {
        std::string szName("sphere_");
        szName.append(std::to_string(x + COUNT * y));

        float radius = glm::linearRand(0.25f, D / COUNT);
        glm::vec3 center;
        center.x = x * D - D / 2;
        center.z = y * D - D / 2;
        center.y = radius;

        _createSphere(szName, radius, center)
            .setColor(glm::vec3(1.0f))
            .setParam(0.7f, 0.3f, 0.0f, 80, 1.0f);
      }
#endif
  }
};
}  // namespace RayTracingHistory
