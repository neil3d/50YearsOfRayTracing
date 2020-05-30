#include "DistributedRayTracer.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "BilliardScene.h"
#include "Material.h"
#include "framework/ThinLensCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int MAX_DEPTH = 32;
constexpr int SPP_N = 10;
constexpr float GAMA = 1.5f;

std::string DistributedRayTracer::getInfo() const {
  return std::string(" - SPP: ") + std::to_string(SPP_N * SPP_N);
}

void DistributedRayTracer::_renderThread(MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  ThinLensCamera* pCamera = static_cast<ThinLensCamera*>(camera.get());
  BilliardScene* pScene = dynamic_cast<BilliardScene*>(scene.get());

  constexpr int n = SPP_N;
  float invN = 1.0f / n;
  float invSPP = 1.0f / (n * n);

  std::array<glm::vec2, n * n> jitteredPointsR, jitteredPointsS;
  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());

  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      // jittering/stratified sampling
      for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
          int index = p * n + q;
          jitteredPointsR[index] =
              invN * (glm::linearRand(0.0f, 1.0f) + glm::vec2(p, q));
          jitteredPointsS[index] =
              invN * (glm::linearRand(0.0f, 1.0f) + glm::vec2(p, q));
        }  // end of for(q)
      }    // end of for(p)
      std::shuffle(jitteredPointsS.begin(), jitteredPointsS.end(), stdRand);

      glm::vec3 color(0);
      for (int i = 0; i < n * n; i++) {
        const glm::vec2& pixelXi = jitteredPointsR[i];
        const glm::vec2& sampleXi = jitteredPointsS[i];

        Ray viewingRay = pCamera->jitteredViewingRay(
            (x + pixelXi.x) / mFrameWidth, (y + pixelXi.y) / mFrameHeight,
            sampleXi);

        color += _traceRay(viewingRay, pScene, 0, sampleXi);
      }

      _writePixel(x, y, glm::vec4(invSPP * color, 1.0f), GAMA);
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 DistributedRayTracer::_traceRay(const Ray& ray, BilliardScene* pScene,
                                          int depth, const glm::vec2 xi) {
  const glm::vec3 bgColor(0.25f, 0.25f, 0.25f);

  if (depth > MAX_DEPTH) return glm::vec3(0);

  // update object's transform by animator
  float t = glm::clamp(xi.x, 0.0f, 1.0f);
  pScene->evaluateAnim(t);

  // intersections
  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  glm::vec3 color = _shade(ray.direction, hitRec, pScene, xi);

  // reflection
  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);

  float Ks = 0.5f;
  if (mtl) Ks = mtl->Ks;
  if (Ks > 0) {
    Ray rRay = _jitteredReflectionRay(ray.direction, hitRec.p, hitRec.normal,
                                      xi, mtl->gloss);
    glm::vec3 rColor = _traceRay(rRay, pScene, depth + 1, xi);
    color += rColor * Ks;
  }

  return color;
}

glm::vec3 DistributedRayTracer::_shade(const glm::vec3& dir,
                                       const HitRecord& shadingPoint,
                                       BilliardScene* pScene,
                                       const glm::vec2 xi) {
  const auto& light = pScene->getMainLight();
  Material* mtl = dynamic_cast<Material*>(shadingPoint.mtl);

  Ray shadowRay = light.jitteredShadowRay(shadingPoint.p, xi);
  HitRecord hitRecS;
  constexpr float SHADOW_E = 0.001f;

  auto stopWithAnyHit = [](const HitRecord&) { return true; };

  glm::vec3 albedo(1);
  if (mtl) albedo = mtl->sampleAlbedo(shadingPoint.uv, shadingPoint.p);

  glm::vec3 color;
  bool bShadow = pScene->anyHit(shadowRay, SHADOW_E, FLOAT_MAX, stopWithAnyHit);
  if (bShadow) {
    float a = 0;  // light.ambient;
    color = a * albedo;
  } else {
    float lgt = light.lighting(shadingPoint.p, shadingPoint.normal, dir, xi);
    color = lgt * albedo;
  }
  return color;
}

Ray DistributedRayTracer::_jitteredReflectionRay(const glm::vec3& dir,
                                                 const glm::vec3& point,
                                                 const glm::vec3& normal,
                                                 const glm::vec2 xi,
                                                 float glossy) {
  // choose any vector T not collinear with normal
  glm::vec3 T = normal;

  if (normal.x < normal.y && normal.x < normal.z) {
    T.x = 1.0f;
  } else if (normal.y < normal.x && normal.y < normal.z) {
    T.y = 1.0f;
  } else
    T.z = 1.0f;

  // constructing orthonormal bases
  T = glm::normalize(T);
  float u = -glossy / 2 + xi.x * glossy;
  float v = -glossy / 2 + xi.y * glossy;
  glm::vec3 U = glossy * glm::cross(T, normal);
  glm::vec3 V = glossy * glm::cross(normal, U);

  // jittered direction
  glm::vec3 R = glm::reflect(dir, normal);
  glm::vec3 Rp = R + u * U + v * V;

  return Ray(point + normal * 0.001f, Rp);
}

}  // namespace RayTracingHistory
