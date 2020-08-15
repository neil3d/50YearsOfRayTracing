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
#include "GlossyMaterial.h"
#include "scene/MeshInstance.h"
#include "scene/Parallelogram.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class TankMtlImporter : public MaterialImporter {
  virtual MyMaterial::Ptr importObj(const std::string& szName) override {
    auto mtl = std::make_shared<DiffuseMaterial>();
    mtl->setColor(glm::vec3(1));
    return mtl;
  }
};

class DemoScene : public MySceneWithLights {
 public:
  virtual void init() override {
    // create objects
    constexpr float W = 500;
    constexpr float D = 500;
    constexpr float H = 500;
    constexpr float LS = 120;  // ligt size

    // key light
    {
      glm::vec3 lightEdge1(0, 0, LS);
      glm::vec3 lightEdge2(LS, 0, 0);
      glm::vec3 lightPos(LS / -2, H - 0.1f, LS / -2);

      auto keyLight = std::make_shared<ParallelogramLight>();
      keyLight->setShape(lightEdge1, lightEdge2, lightPos).setIntensity(120);
      mLights.push_back(keyLight);

      createObject<Parallelogram>("light_shape")
          .setEdges(lightEdge1, lightEdge2)
          .setAnchor(lightPos)
          .createMaterial<DiffuseMaterial>()
          .setColor(glm::vec3(1))
          .enableLight();
    }
    // fill light
    {
      glm::vec3 lightEdge1(LS, 0, 0);
      glm::vec3 lightEdge2(0, LS, 0);
      glm::vec3 lightPos(0, H / 100, -D);

      auto fillLight = std::make_shared<ParallelogramLight>();
      fillLight->setShape(lightEdge2, lightEdge1, lightPos).setIntensity(5);
      mLights.push_back(fillLight);
    }

    createObject<Parallelogram>("floor")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    createObject<Parallelogram>("left_wall")
        .setEdges(glm::vec3(0, H, 0), glm::vec3(0, 0, D))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(0, 1, 0));

    createObject<Parallelogram>("right_wall")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(0, H, 0))
        .setAnchor(glm::vec3(W / 2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1, 0, 0));

    createObject<Parallelogram>("ceiling")
        .setEdges(glm::vec3(W, 0, 0), glm::vec3(0, 0, D))
        .setAnchor(glm::vec3(W / -2, H, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    createObject<Parallelogram>("back_wall")
        .setEdges(glm::vec3(0, H, 0), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / 2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    const glm::vec3 GOLD(205 / 255.0f, 127 / 255.0f, 50 / 255.0f);

#if 0  // ball
    float ball = W / 6;
    createObject<Sphere>("small_ball")
        .setCenter(glm::vec3(-120, ball * 2, 0))
        .setRadius(ball)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    ball *= 1.25f;
    createObject<Sphere>("big_ball")
        .setCenter(glm::vec3(ball, ball, 0))
        .setRadius(ball)
        .createMaterial<PhongMaterial>()
        .setColor(glm::vec3(1));
#endif

#if 1  // add teapot
    constexpr float SIZE = 120;

    const char* const szTeapotFileName = "content/teapot/teapot.obj";
    auto& teapot1 = createObject<MeshInstance>("teapot");
    teapot1.setMeshFile(szTeapotFileName)
        .createMaterial<GlossyMaterial>()
        .setColor(GOLD);
    teapot1.setPosition(glm::vec3(SIZE, 0, 0))
        .setScale(SIZE)
        .setRotation(0, -15, 0);

    auto& teapot2 = createObject<MeshInstance>("teapot");
    teapot2.setMeshFile(szTeapotFileName)
        .createMaterial<LambertianMaterial>()
        .setColor(GOLD);
    teapot2.setPosition(glm::vec3(-SIZE, 0, 0))
        .setScale(SIZE)
        .setRotation(0, -15, 0);
#endif

#if 0  // stanford dragon
    const char* const szFileName = "content/dragon/dragon.obj";
    auto& mesh = createObject<MeshInstance>("dragon");
    mesh.setMeshFile(szFileName);
    mesh.createMaterial<PhongMaterial>().setColor(GOLD);
    mesh.setScale(20)
        .setPosition(glm::vec3(0, 40, 0))
        .setRotation(0, glm::radians(180.0f), 0);
#endif

#if 0  // stanford bunny
    const char* const szFileName = "content/bunny/bunny.obj";
    auto& mesh = createObject<MeshInstance>("bunny");
    mesh.setMeshFile(szFileName);
    mesh.createMaterial<DiffuseMaterial>().setColor(glm::vec3(0, 0, 1.0f));
    mesh.setScale(150)
        .setPosition(glm::vec3(0, 100, 0))
        .setRotation(0, glm::radians(180.0f), 0);
#endif

#if 0  // tank
    const char* const szFileName = "content/tank/tank.obj";
    auto& mesh = createObject<MeshInstance>("tank");
    mesh.setMeshFile(szFileName);
    TankMtlImporter mtlImporter;
    mesh.importMaterial(&mtlImporter);
    mesh.setScale(80);
#endif
  }
};
}  // namespace RayTracingHistory
