#include "CornellBox.h"

#include "Box.h"
#include "Parallelogram.h"
#include "Sphere.h"

namespace RayTracingHistory {

void CornellBox::init() {
  // create objects
  constexpr float W = 5;
  constexpr float D = 5;
  constexpr float H = 5;
  constexpr float LS = 3.5f;  // ligt size scale

  createObject<Parallelogram>("light")
      .setEdges(glm::vec3(0, 0, D / LS), glm::vec3(W / LS, 0, 0))
      .setAnchor(glm::vec3(W / -2 / LS, H - 0.01f, D / -2 / LS));

  createObject<Parallelogram>("ceiling")
      .setEdges(glm::vec3(W, 0, 0), glm::vec3(0, 0, D))
      .setAnchor(glm::vec3(W / -2, H, D / -2));

  createObject<Parallelogram>("floor")
      .setEdges(glm::vec3(0, 0, D), glm::vec3(W, 0, 0))
      .setAnchor(glm::vec3(W / -2, 0, D / -2));

  createObject<Parallelogram>("left_wall")
      .setEdges(glm::vec3(0, H, 0), glm::vec3(0, 0, D))
      .setAnchor(glm::vec3(W / -2, 0, D / -2));

  createObject<Parallelogram>("right_wall")
      .setEdges(glm::vec3(0, 0, D), glm::vec3(0, H, 0))
      .setAnchor(glm::vec3(W / 2, 0, D / -2));

  createObject<Parallelogram>("back_wall")
      .setEdges(glm::vec3(0, H, 0), glm::vec3(W, 0, 0))
      .setAnchor(glm::vec3(W / -2, 0, D / 2));

  createObject<Box>("tall_box")
      .setExtents(W / 4.5f, H / 2, D / 4.5f)
      .setPosition(glm::vec3(-W / 5, 0, 0))
      .setRotation(0, glm::pi<float>() / 6, 0);

  glm::vec3 boxPos = glm::vec3(W / 4, 0, 0);
  glm::vec3 boxSize = glm::vec3(W / 4, H / 3.5f, D / 4);
  createObject<Box>("short_box")
      .setExtents(boxSize.x, boxSize.y, boxSize.z)
      .setPosition(boxPos)
      .setRotation(0, glm::pi<float>() / 10, 0);

#if 0
  float ball = W / 4.5f / 2;
  createObject<Sphere>("ball")
      .setCenter(boxPos + glm::vec3(0, ball + boxSize.y, 0))
      .setRadius(ball);
#endif
}

}  // namespace RayTracingHistory
