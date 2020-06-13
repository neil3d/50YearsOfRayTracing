#include "DistributedRayTracer.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <random>

#include "Material.h"
#include "MySceneWithLight.h"
#include "framework/ThinLensCamera.h"
#include "geometry/ONB.h"
#include "sampling/JitteringSampling.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int MAX_BOUNCES = 32;
constexpr int SPP_N = 3;
constexpr float GAMA = 1.5f;

std::string DistributedRayTracer::getInfo() const {
  return std::string(" - SPP: ") + std::to_string(SPP_N * SPP_N);
}

void DistributedRayTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  ThinLensCamera* pCamera = static_cast<ThinLensCamera*>(camera.get());
  MySceneWithLight* pScene = dynamic_cast<MySceneWithLight*>(scene.get());

  constexpr int n = SPP_N;
  float invSPP = 1.0f / (n * n);

  std::vector<glm::vec2> xiR, xiS;

  for (int y = tile.top; y < tile.bottom; y++)
    for (int x = tile.left; x < tile.right; x++) {
      if (!mRuning) break;

      xiR = JitteringSampling::generateSamples(n, false);
      xiS = JitteringSampling::generateSamples(n, true);

      glm::vec3 color(0);
      for (int i = 0; i < n * n; i++) {
        const glm::vec2& pixelXi = xiR[i];
        const glm::vec2& sampleXi = xiS[i];

        Ray viewingRay = pCamera->jitteredViewingRay(
            (x + pixelXi.x) / mFrameWidth, (y + pixelXi.y) / mFrameHeight,
            sampleXi);

        viewingRay.time = (sampleXi.x + sampleXi.y) * 0.5f;
        color += _traceRay(viewingRay, pScene, 0, sampleXi);
      }

      _writePixel(x, y, glm::vec4(invSPP * color, 1.0f), GAMA);
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 DistributedRayTracer::_traceRay(const Ray& ray,
                                          MySceneWithLight* pScene, int depth,
                                          const glm::vec2 xi) {
  const glm::vec3 bgColor(0.25f, 0.25f, 0.25f);

  if (depth > MAX_BOUNCES) return glm::vec3(0);

  // intersections
  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  auto shading = _shade(ray.direction, hitRec, pScene, xi);
  float shadowFactor = std::get<0>(shading);
  glm::vec3 color = std::get<1>(shading);

  // reflection
  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);
  if (!mtl) return glm::vec3(1, 0, 0);

  float Kr = shadowFactor;
  if (mtl) Kr *= mtl->Kr;
  if (Kr > 0) {
    Ray rRay = _jitteredReflectionRay(ray.direction, hitRec.p, hitRec.normal,
                                      xi, mtl->gloss);
    rRay.time = ray.time;
    glm::vec3 rColor = _traceRay(rRay, pScene, depth + 1, xi);

    color += Kr * rColor;
  }

  return color;
}

std::tuple<float, glm::vec3> DistributedRayTracer::_shade(
    const glm::vec3& dir, const HitRecord& shadingPoint,
    MySceneWithLight* pScene, const glm::vec2 xi) {
  const auto& light = pScene->getMainLight();
  Material* mtl = dynamic_cast<Material*>(shadingPoint.mtl);

  Ray shadowRay = light.jitteredShadowRay(shadingPoint.p, xi);
  shadowRay.time = xi.x;
  HitRecord hitRecS;
  constexpr float SHADOW_E = 0.001f;

  auto stopWithAnyHit = [](const HitRecord&) { return true; };

  glm::vec3 baseColor(1);
  if (mtl) baseColor = mtl->sampleBaseColor(shadingPoint.uv, shadingPoint.p);

  glm::vec3 color;
  bool bShadow = pScene->anyHit(shadowRay, SHADOW_E, FLOAT_MAX, stopWithAnyHit);
  if (bShadow) {
    float a = light.ambient;
    color = a * baseColor;
  } else {
    float lgt = light.lighting(shadingPoint.p, shadingPoint.normal, dir, xi);
    color = lgt * baseColor;
  }
  return std::make_tuple(bShadow ? light.ambient : 1.0f, color);
}

Ray DistributedRayTracer::_jitteredReflectionRay(const glm::vec3& dir,
                                                 const glm::vec3& point,
                                                 const glm::vec3& normal,
                                                 const glm::vec2 xi,
                                                 float glossy) {
  ONB onb(normal);

  float u = -glossy / 2 + xi.x * glossy;
  float v = -glossy / 2 + xi.y * glossy;
  glm::vec3 U = glossy * onb.U;
  glm::vec3 V = glossy * onb.V;

  // jittered direction
  glm::vec3 R = glm::reflect(dir, normal);
  glm::vec3 Rp = R + u * U + v * V;

  return Ray(point + normal * 0.001f, Rp);
}

}  // namespace RayTracingHistory
