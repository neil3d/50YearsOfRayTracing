#include "SimpleMonteCarloApproach.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int SIMPLES = 12;
constexpr int MAX_DEPTH = 3;

void SimpleMonteCarloApproach::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                                 MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  int W = mFrameWidth;
  int H = mFrameHeight;

  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  for (int y = tile.top; y < tile.bottom; y++)
    for (int x = tile.left; x < tile.right; x++) {
      if (!mRuning) break;

      Ray primaryRay =
          pCamera->generateViewingRay((x + 0.5f) / W, (y + 0.5f) / H);

      glm::vec3 color = bgColor;
      HitRecord hitRec;
      if (pScene->closestHit(primaryRay, 0.01f, FLOAT_MAX, hitRec)) {
        MaterialBase* pMtl = static_cast<MaterialBase*>(hitRec.mtl);
        if (pMtl->getEmission() > 0.1f)
          color = pMtl->getEmission() * pMtl->getBaseColor(hitRec.uv, hitRec.p);
        else
          color =
              _shade(Ray(hitRec.p, -primaryRay.direction), hitRec, pScene, 0);
      }

      _writePixel(x, y, glm::vec4(color, 1), 0.68f);
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 SimpleMonteCarloApproach::_shade(const Ray& wo,
                                           const HitRecord& shadingPoint,
                                           MyScene* pScene, int depth) {
  if (depth > MAX_DEPTH) return glm::vec3(0);

  MaterialBase* pMtl = static_cast<MaterialBase*>(shadingPoint.mtl);

  // error check
  if (!pMtl) return glm::vec3(1, 0, 0);
  glm::vec3 baseColor = pMtl->getBaseColor(shadingPoint.uv, shadingPoint.p);

  // Monte Carlo Estimating
  glm::vec3 sum(0);
  float Kd = powf(0.25f, depth * depth);

  for (int i = 0; i < SIMPLES; i++) {
    glm::vec3 wi = pMtl->scatter(shadingPoint.normal);
    float pdf = pMtl->pdf(wi, shadingPoint.normal);
    float cosine = glm::dot(wi, shadingPoint.normal);

    Ray ray(shadingPoint.p, wi);
    HitRecord hitRec;
    if (!pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec)) continue;

    MaterialBase* pHitMtl = static_cast<MaterialBase*>(hitRec.mtl);
    if (pHitMtl->getEmission() > 0.1f) {
      // hit a light
      glm::vec3 lightColor = pHitMtl->getBaseColor(hitRec.uv, hitRec.p);
      glm::vec3 CC = pHitMtl->getEmission() * Kd * cosine * lightColor / pdf;
      sum += CC * baseColor;
    } else {
      glm::vec3 CC = _shade(Ray(hitRec.p, -wi), hitRec, pScene, depth + 1);
      sum += baseColor * CC * Kd * cosine / pdf;
    }

  }  // end of for

  return sum / (float)SIMPLES;
}

}  // namespace RayTracingHistory
