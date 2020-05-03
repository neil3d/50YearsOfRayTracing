#include "RayCastingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void RayCastingRenderer::_renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (mRuning) {
        _drawSinglePixel(x, y);
      } else
        break;
    }  // end of for(x)
}

void RayCastingRenderer::_drawSinglePixel(int x, int y) {
  glm::vec3 normal = glm::sphericalRand(1.0f);
  normal = glm::normalize(normal);

  glm::vec3 light(1, 1, 0);
  light = glm::normalize(light);

  float c = glm::dot(normal, light);
  glm::vec4 color(c, c, c, 1.0f);

  //_writePixel(x, y, color);
}