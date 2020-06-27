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
#include "MySceneWithLight.h"
#include "ParallelogramLight.h"
#include "PhongMaterial.h"
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

class DemoScene : public MySceneWithLight {
  ParallelogramLight mMainLight;

 public:
  virtual const AreaLight* getMainLight() const override { return &mMainLight; }

  virtual void init() override {
    // create objects
    constexpr float W = 500;
    constexpr float D = 500;
    constexpr float H = 500;
    constexpr float LS = 120;  // ligt size

    glm::vec3 lightEdge1(0, 0, LS);
    glm::vec3 lightEdge2(LS, 0, 0);
    glm::vec3 lightPos(LS / -2, H - 0.1f, LS / -2);
    mMainLight.setShape(lightEdge1, lightEdge2, lightPos);

    createObject<Parallelogram>("light_shape")
        .setEdges(lightEdge1, lightEdge2)
        .setAnchor(lightPos)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1))
        .enableLight();

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

#if 1  // ball
    float ball = W / 6;
    /*createObject<Sphere>("small_ball")
        .setCenter(glm::vec3(-120, ball * 2, 0))
        .setRadius(ball)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));*/

    ball *= 1.5f;
    createObject<Sphere>("big_ball")
        .setCenter(glm::vec3(0, ball, 0))
        .setRadius(ball)
        .createMaterial<PhongMaterial>()
        .setColor(glm::vec3(1));
#endif

#if 0  // stanford dragon
    constexpr glm::vec3 GOLD(205 / 255.0f, 127 / 255.0f, 50 / 255.0f);

    const char* const szFileName = "content/dragon/dragon.obj";
    auto& mesh = createObject<MeshInstance>("dragon");
    mesh.setMeshFile(szFileName);
    mesh.createMaterial<DiffuseMaterial>().setColor(GOLD);
    mesh.setScale(25)
        .setPosition(glm::vec3(0, 100, 0))
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
