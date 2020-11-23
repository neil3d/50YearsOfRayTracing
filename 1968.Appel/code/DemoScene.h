/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "scene/MeshInstance.h"
#include "scene/MyScene.h"
#include "scene/Parallelogram.h"

namespace RTKit1 {

class DemoScene : public MyScene {
 public:
  virtual void init() override {
    constexpr float W = 8;
    constexpr float H = 8;
    createObject<Parallelogram>("ground")
        .setEdges(glm::vec3(0, 0, H), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, H / -2));

    const char* const szFileName = "content/machine/machine.obj";
    auto& mesh = createObject<MeshInstance>("machine");
    mesh.setMeshFile(szFileName).setPosition(glm::vec3(0, 1.5f, 0));
  }
};

}  // namespace RTKit1
