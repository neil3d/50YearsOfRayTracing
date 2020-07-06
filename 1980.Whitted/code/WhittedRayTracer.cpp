#include "WhittedRayTracer.h"

#include <algorithm>

#include "Material.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float fMax = std::numeric_limits<float>::max();
constexpr int MAX_BOUNCES = 32;

bool myRefract(const glm::vec3& v, const glm::vec3& n, float niOverNt,
               glm::vec3& outRefracted) {
  float dt = glm::dot(v, n);
  float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
  if (discriminant > 0) {
    outRefracted = niOverNt * (v - n * dt) - n * sqrt(discriminant);
    return true;
  }
  return false;
}

// 1994, Schlick, Christophe. "An Inexpensive BRDF Model for Physically‐based
// Rendering"
float mySchlick(float cosine, float Kn, float exp) {
  float r0 = (1 - Kn) / (1 + Kn);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), exp);
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
                                                 hitRec.normal, mtl->Kn);
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
                                           const glm::vec3& normal, float Kn) {
  glm::vec3 N = normal;
  glm::vec3 wi = -glm::normalize(dir);
  bool bInside = glm::dot(wi, N) < 0;

  glm::vec3 outwardNormal;
  float eta;
  if (bInside) {
    outwardNormal = -N;
    eta = 1.0f / Kn;
  } else {
    outwardNormal = N;
    eta = Kn;
  }

  float cosTheta = glm::dot(wi, outwardNormal);
  float sinTheta = glm::sqrt(1.0f - cosTheta * cosTheta);

  if (eta * eta * sinTheta > 1.0f) {
    // total internal reflection
    float reflectivity = 1.0f;
    return std::make_tuple(false, reflectivity, Ray(), outwardNormal);
  }

  float reflectivity = mySchlick(cosTheta, eta, 5.0f);

  glm::vec3 refraction = glm::refract(wi, outwardNormal, eta);
  Ray rRay(point, refraction);
  rRay.applayBiasOffset(outwardNormal, 0.0001f);
  return std::make_tuple(true, reflectivity, rRay, outwardNormal);
}

}  // namespace RayTracingHistory
