#include "CornellBox.h"

#include "Parallelogram.h"

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
}

}  // namespace RayTracingHistory
