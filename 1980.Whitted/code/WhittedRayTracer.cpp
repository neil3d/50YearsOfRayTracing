#include "WhittedRayTracer.h"

#include <algorithm>

#include "Material.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float fMax = std::numeric_limits<float>::max();
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

float _fresnel(const glm::vec3& I, const glm::vec3& N, const float& ior) {
  float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(I, N));
  float etai = 1, etat = ior;
  if (cosi > 0) {
    std::swap(etai, etat);
  }

  float kr;

  // compute sini using Snell's law
  float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));

  // total internal reflection
  if (sint >= 1) {
    kr = 1;
  } else {
    float cost = sqrtf(std::max(0.f, 1 - sint * sint));
    cosi = fabsf(cosi);
    float Rs =
        ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
    float Rp =
        ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
    kr = (Rs * Rs + Rp * Rp) / 2;
  }
  // As a consequence of the conservation of energy, transmittance is given by:
  // kt = 1 - kr;
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
  bool bHit = pScene->closestHit(ray, 0.001f, fMax, hitRec);
  if (!bHit) {
    return _backgroundColor(ray) * weight;
  }

  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);

  // error check
  if (!mtl) return glm::vec3(1, 0, 0);

  glm::vec3 outwardNormal = hitRec.normal;

  // refraction
  glm::vec3 refractionColor(0);
  float reflectivity = 0.0f;
  if (mtl->Kt * weight > 0.001f) {
    auto refractTuple = _generateRefractationRay(ray.direction, hitRec.p,
                                                 hitRec.normal, mtl->eta);
    bool bRefraction = std::get<0>(refractTuple);
    reflectivity = std::get<1>(refractTuple);
    Ray rRay = std::get<2>(refractTuple);
    outwardNormal = std::get<3>(refractTuple);

    if (bRefraction) {
      float Kt = (1.0f - reflectivity) * mtl->Kt;
      refractionColor = _traceRay(rRay, pScene, depth + 1, weight * Kt);
    }
  }  // end of if(transparent object)

  // reflection
  glm::vec3 reflectionColor(0);
  float Ks = glm::clamp(reflectivity + mtl->Ks, 0.0f, 1.0f);
  if (Ks * weight > 0.001f) {
    Ray rRay = _generateReflectionRay(ray.direction, hitRec.p, outwardNormal);
    rRay.applayBiasOffset(outwardNormal, 0.001f);
    reflectionColor = _traceRay(rRay, pScene, depth + 1, weight * Ks);
  }

  glm::vec3 lighting =
      _shade(-ray.direction, hitRec.p, outwardNormal, mtl, pScene);
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

    pScene->anyHit(shadowRay, 0, fMax, shadowHitCallback);

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
  return Ray(point, outDir);
}

std::tuple<bool, float, Ray, glm::vec3>
WhittedRayTracer::_generateRefractationRay(const glm::vec3& dir,
                                           const glm::vec3& point,
                                           const glm::vec3& normal, float eta) {
  bool bOutside = glm::dot(dir, normal) < 0;
  glm::vec3 outwardNormal;
  if (bOutside)
    outwardNormal = normal;
  else
    outwardNormal = -normal;

  float reflectivity = _fresnel(dir, normal, eta);

  if (reflectivity < 1.0f) {
    glm::vec3 refraction = _refract(dir, normal, eta);
    Ray rRay(point, refraction);
    rRay.applayBiasOffset(outwardNormal, 0.0001f);
    return std::make_tuple(true, reflectivity, rRay, outwardNormal);
  } else {
    return std::make_tuple(false, reflectivity, Ray(), outwardNormal);
  }
}

}  // namespace RayTracingHistory
