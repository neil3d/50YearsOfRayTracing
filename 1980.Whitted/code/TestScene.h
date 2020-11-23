/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/random.hpp>

#include "TestSceneBase.h"
#include "asset/MaterialImporter.h"
#include "scene/MeshInstance.h"
#include "scene/Plane.h"

namespace RTKit1 {

class MyMtlImporter : public MaterialImporter {
  virtual MyMaterial::Ptr importObj(const std::string& szName) override {
    if (szName == "cup_material") {
      auto mtl = std::make_shared<Material>();
      mtl->setParam(0.2f, 0.2f, 0.6f, 200.0f, 1.33f);
      mtl->setColor(glm::vec3(1));
      return mtl;
    } else if (szName == "OuterMat") {
      auto mtl = std::make_shared<Material>();
      mtl->setParam(0.1f, 0.1f, 0.8f, 200.0f, 1.33f);
      mtl->setColor(glm::vec3(1));
      return mtl;
    }

    auto mtl = std::make_shared<Material>();
    mtl->setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);
    if (szName == "pencil_mateiral") mtl->setColor(glm::vec3(0.8f, 0.6f, 0.1f));
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
        .setTiling(1.5f, 1)
        .setParam(1, 0, 0, 60, 1);

    enum class ETestModel { Cup, Knob, Dragon, Statue, Teapot, Spheres };
    constexpr ETestModel testMdl = ETestModel::Dragon;
    MyMtlImporter mtlImporter;

    switch (testMdl) {
      case ETestModel::Cup: {
        auto& cup = createObject<MeshInstance>("cup");
        cup.setMeshFile("content/cup/cup.obj").setPosition(glm::vec3(0, 0, -1));
        cup.importMaterial(&mtlImporter);
      } break;
      case ETestModel::Knob: {
        auto& knob = createObject<MeshInstance>("knob");
        knob.setMeshFile("content/mori_knob/knob.obj")
            .setScale(2)
            .setPosition(glm::vec3(0, 1, 0));
        knob.importMaterial(&mtlImporter);
      } break;
      case ETestModel::Dragon: {
        auto& dragon = createObject<MeshInstance>("dragon");
        dragon.setMeshFile("content/dragon/dragon.obj")
            .setScale(0.3f)
            .setPosition(glm::vec3(0, 0, 0));
        dragon.createMaterial<Material>()
            .setParam(0.2f, 0.2f, 0.6f, 160.0f, 1.33f)
            .setColor(glm::vec3(1));
      } break;
      case ETestModel::Statue: {
        auto& statue = createObject<MeshInstance>("statue");
        statue.setMeshFile("content/statue/statue.obj")
            .setScale(1.25f)
            .setRotation(0, glm::radians(180.0f), 0)
            .setPosition(glm::vec3(0, 0, 0));
        statue.createMaterial<Material>()
            .setParam(0.2f, 0.1f, 0.7f, 512.0f, 1.5f)
            .setColor(glm::vec3(1));
      } break;
      case ETestModel::Teapot: {
        constexpr float SIZE = 1.85f;

        const glm::vec3 GOLD(205 / 255.0f, 127 / 255.0f, 50 / 255.0f);

        const char* const szTeapotFileName = "content/teapot/teapot.obj";
        auto& teapot1 = createObject<MeshInstance>("teapot");
        teapot1.setMeshFile(szTeapotFileName)
            .createMaterial<Material>()
            .setParam(0.68f, 0.32f, 0.0f, 512.0f, 1.f)
            .setColor(GOLD);
        teapot1.setPosition(glm::vec3(SIZE, 0, 0))
            .setScale(SIZE)
            .setRotation(0, -1, 0);

        auto& teapot2 = createObject<MeshInstance>("teapot");
        teapot2.setMeshFile(szTeapotFileName)
            .createMaterial<Material>()
            .setParam(0.2f, 0.2f, 0.6f, 160.0f, 1.33f)
            .setColor(glm::vec3(1));
        teapot2.setPosition(glm::vec3(-SIZE, 0, 0))
            .setScale(SIZE)
            .setRotation(0, -1, 0);
      } break;
      case ETestModel::Spheres:
      default: {
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
      } break;
    }  // end of switch
  }
};
}  // namespace RTKit1
