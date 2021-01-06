#include "AnalyticDirectIntegrator.h"

#include <glm/gtc/constants.hpp>
#include <limits>

#include "framework/PinholeCamera.h"
#include "material/PhongMaterial.h"
#include "scene/QuadLight.h"

namespace RTKit2 {
constexpr float BIAS = 0.001f;

void AnalyticDirectIntegrator::_renderThread(MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());

  float W = static_cast<float>(mFrameWidth);
  float H = static_cast<float>(mFrameHeight);

  for (int y = 0; y < mFrameHeight; y++) {
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;
      MyRay eyeRay = pCamera->generateViewingRay((x + 0.5f)/W, (y + 0.5f)/H);
      glm::vec3 color = _traceRay(eyeRay, scene);

      _writePixel(x, y, glm::vec4(color, 1), 1.0f);
      mPixelCount++;
    }  // end of for(x)
  }    // end of for(y)

  _onRenderFinished();
}

glm::vec3 AnalyticDirectIntegrator::_traceRay(MyRay ray, MyScene::Ptr scene) {
  const float FARMOST = 2000;//std::numeric_limits<float>::infinity();
  HitRecord hrc;
  bool hit = scene->intersect(ray, 0, FARMOST, hrc);
  if (!hit) return glm::vec3(0);

  glm::vec3 color(0);
  // hit a light
  if (hrc.isLight) {
    QuadLight* lgt = dynamic_cast<QuadLight*>(hrc.obj);
    color = lgt->getIntensity();
  } else {
    // hit geometry
    const auto& lights = scene->getLights();

    const PhongMaterial* mtlPtr = dynamic_cast<PhongMaterial*>(hrc.mtl);
    glm::vec3 Kd = mtlPtr->diffuse;

    for (const auto& lgt : lights) {
      QuadLight* lgtPtr = dynamic_cast<QuadLight*>(lgt.get());
      color += _shade(hrc.pt, hrc.normal, Kd, lgtPtr);
    }  // end of for
  }
  return color;
}

glm::vec3 AnalyticDirectIntegrator::_shade(glm::vec3 pt, glm::vec3 normal,
                                           glm::vec3 Kd, QuadLight* lgt) {
  const float PI = glm::pi<float>();

  glm::vec3 phi(0);
  for (int i = 0; i < 4; i++) {
    glm::vec3 Vi = lgt->getQuadVertex(i);
    glm::vec3 Vj = lgt->getQuadVertex((i + 1) % 4);

    glm::vec3 gama = glm::normalize(glm::cross(Vi - pt, Vj - pt));

    float theta = glm::dot(glm::normalize(Vi - pt), glm::normalize(Vj - pt));
    theta = acosf(theta);

    phi += gama * theta;
  }
  phi *= 0.5f;

  glm::vec3 Li = lgt->getIntensity();
  return Kd / PI * Li * glm::max(0.0f, glm::dot(phi, normal));
}

}  // namespace RTKit2