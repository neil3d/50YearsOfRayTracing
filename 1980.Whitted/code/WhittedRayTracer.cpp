#include "WhittedRayTracer.h"

#include <algorithm>

#include "Material.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float F_MAX = std::numeric_limits<float>::max();
constexpr int MAX_BOUNCES = 32;

glm::vec3 _refract(const glm::vec3& I, const glm::vec3& N, const float& ior) {
  float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(I, N));
  float etai = 1, etat = ior;
  glm::vec3 n = N;
  if (cosi < 0) {
    cosi = -cosi;
  } else {
    std::swap(etai, etat);
    n = -N;
  }
  float eta = etai / etat;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  return k < 0.0f ? glm::vec3(0.0f) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

double _fresnel(const glm::vec3& I, const glm::vec3& N, float ior) {
  double cosi = glm::clamp(-1.0, 1.0, (double)glm::dot(I, N));
  double etai = 1, etat = ior;
  if (cosi > 0) {
    std::swap(etai, etat);
  }

  double kr;

  // compute sini using Snell's law
  double sint = etai / etat * sqrt(std::max(0., 1 - cosi * cosi));

  // total internal reflection
  if (sint >= 1) {
    kr = 1;
  } else {
    double cost = sqrt(std::max(0., 1 - sint * sint));
    cosi = fabsf(cosi);
    double Rs =
        ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
    double Rp =
        ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
    kr = (Rs * Rs + Rp * Rp) / 2;
  }
  return kr;
}

void WhittedRayTracer::_renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  TestSceneBase* pScene = dynamic_cast<TestSceneBase*>(scene.get());

  float W = static_cast<float>(mFrameWidth);
  float H = static_cast<float>(mFrameHeight);

  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      Ray eyeRay = pCamera->generateViewingRay((x + 0.5f) / W, (y + 0.5f) / H);
      glm::vec3 color = _traceRay(eyeRay, pScene, 0, 1.0f);

      constexpr float GAMA = 1.0f / 1.25f;
      _writePixel(x, y, glm::vec4(color, 1), GAMA);
      mPixelCount++;
    }  // end of for(x)

  _onRenderFinished();
}

glm::vec3 WhittedRayTracer::_backgroundColor(const Ray& ray) {
  // blue-white linear gradient background
  const glm::vec3 topColor(0.85f, 0.85f, 0.95f);
  const glm::vec3 bottomColor(0.5f, 0.6f, 0.98f);
  float t = ray.direction.y;
  return topColor * t + bottomColor * (1.0f - t);
}

glm::vec3 WhittedRayTracer::_traceRay(Ray ray, TestSceneBase* pScene, int depth,
                                      float weight) {
  if (depth > MAX_BOUNCES) return glm::vec3(0);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.001f, F_MAX, hitRec);
  if (!bHit) {
    return _backgroundColor(ray) * weight;
  }

  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);

  // error check
  if (!mtl) return glm::vec3(1, 0, 0);

  // refraction
  glm::vec3 refractionColor(0);
  float reflectivity = 0.0f;
  if (mtl->Kt * weight > 0.001f) {
    auto refractTuple = _generateRefractationRay(ray.direction, hitRec.p,
                                                 hitRec.normal, mtl->eta);
    bool bRefraction = std::get<0>(refractTuple);
    reflectivity = std::get<1>(refractTuple);
    Ray rRay = std::get<2>(refractTuple);

    if (bRefraction) {
      float Kt = (1.0f - reflectivity) * mtl->Kt;
      refractionColor = _traceRay(rRay, pScene, depth + 1, weight * Kt);
    }
  }  // end of if(transparent object)

  // reflection
  glm::vec3 reflectionColor(0);
  float Ks = glm::clamp(reflectivity + mtl->Ks, 0.0f, 1.0f);
  if (Ks * weight > 0.001f) {
    Ray rRay = _generateReflectionRay(ray.direction, hitRec.p, hitRec.normal);
    reflectionColor = _traceRay(rRay, pScene, depth + 1, weight * Ks);
  }

  glm::vec3 lighting =
      _shade(-ray.direction, hitRec.p, hitRec.normal, mtl, pScene);
  glm::vec3 baseColor = mtl->sampleBaseColor(hitRec.uv, hitRec.p);

  glm::vec3 ambient = lighting.x * baseColor;
  glm::vec3 diffuse = lighting.y * mtl->Kd * baseColor;
  glm::vec3 specular = lighting.z * Ks * baseColor;

  return ambient +
         (diffuse + specular + refractionColor + reflectionColor) * weight;
}

glm::vec3 WhittedRayTracer::_shade(const glm::vec3& wo, const glm::vec3& pt,
                                   const glm::vec3& normal, Material* mtl,
                                   TestSceneBase* pScene) {
  glm::vec3 lighting(0);

  const auto& lights = pScene->getLights();
  for (const auto& light : lights) {
    auto shadowRet = light->generateShadowRay(pt);
    Ray shadowRay = std::get<0>(shadowRet);
    float lightDistance = std::get<1>(shadowRet);
    shadowRay.applayBiasOffset(normal, 0.001f);

    HitRecord hitRecS;
    float attenuation = 1.0f;
    auto shadowHitCallback = [&attenuation,
                              lightDistance](const HitRecord& hit) {
      if (hit.t > lightDistance) return false;

      Material* mtl = dynamic_cast<Material*>(hit.mtl);
      if (mtl) attenuation *= std::powf(mtl->Kt, 2.5f);
      return attenuation > 0.0f;
    };

    pScene->anyHit(shadowRay, 0, F_MAX, shadowHitCallback);

    // lighting
    glm::vec3 lgt = light->lighting(pt, normal, wo, mtl->n);

    // ambient lighting
    lighting.x += lgt.x;

    // diffuse lighting
    lighting.y += lgt.y * attenuation;

    // specular lighting
    lighting.z += lgt.z * attenuation;
  }  // end of for each light

  return lighting;
}

Ray WhittedRayTracer::_generateReflectionRay(const glm::vec3& dir,
                                             const glm::vec3& point,
                                             const glm::vec3& normal) {
  glm::vec3 outDir = glm::reflect(dir, normal);
  Ray rRay(point, outDir);

  if (glm::dot(outDir, normal) > 0) {
    rRay.applayBiasOffset(normal, 0.001f);
  } else {
    rRay.applayBiasOffset(-normal, 0.001f);
  }
  return rRay;
}

std::tuple<bool, float, Ray> WhittedRayTracer::_generateRefractationRay(
    const glm::vec3& dir, const glm::vec3& point, const glm::vec3& normal,
    float eta) {
  double Kr = _fresnel(dir, normal, eta);

  if (Kr < 1.0f) {
    glm::vec3 refraction = _refract(dir, normal, eta);
    Ray rRay(point, refraction);

    if (glm::dot(refraction, normal) > 0) {
      rRay.applayBiasOffset(normal, 0.001f);
    } else {
      rRay.applayBiasOffset(-normal, 0.001f);
    }

    return std::make_tuple(true, Kr, rRay);
  } else {
    return std::make_tuple(false, Kr, Ray());
  }
}

}  // namespace RayTracingHistory
