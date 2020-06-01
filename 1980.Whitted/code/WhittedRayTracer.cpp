#include "WhittedRayTracer.h"

#include <algorithm>

#include "DemoScene.h"
#include "Material.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float fMax = std::numeric_limits<float>::max();
constexpr int MAX_DEPTH = 32;

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

void WhittedRayTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                         MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  int W = mFrameWidth;
  int H = mFrameHeight;

  for (int y = tile.top; y < tile.bottom; y++)
    for (int x = tile.left; x < tile.right; x++) {
      if (!mRuning) break;

      Ray eyeRay = pCamera->generateViewingRay((x + 0.5f) / W, (y + 0.5f) / H);
      glm::vec3 color = _traceRay(eyeRay, scene.get(), 0);

      constexpr float GAMA = 1.0f / 1.25f;
      _writePixel(x, y, glm::vec4(color, 1), GAMA);
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 WhittedRayTracer::_backgroundColor(const Ray& ray) {
  // blue-white linear gradient background
  const glm::vec3 topColor(0.85f, 0.85f, 0.95f);
  const glm::vec3 bottomColor(0.5f, 0.6f, 0.98f);
  float t = ray.direction.y;
  return topColor * t + bottomColor * (1.0f - t);
}

glm::vec3 WhittedRayTracer::_traceRay(Ray ray, MyScene* pScene, int depth) {
  if (depth > MAX_DEPTH) return _backgroundColor(ray);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.001f, fMax, hitRec);
  if (!bHit) return _backgroundColor(ray);

  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);
  DemoScene* scene = dynamic_cast<DemoScene*>(pScene);

  // error check
  if (!mtl) return glm::vec3(1, 0, 0);
  if (!scene) return glm::vec3(0, 1, 0);

  glm::vec3 color = _shade(ray.direction, hitRec, pScene);

  // refraction
  float reflectivity = 0.0f;
  if (mtl->Kt > 0) {
    float Kn = mtl->Kn;
    glm::vec3 normal = hitRec.normal;

    Ray rRay;
    bool bRefraction = false;

    bRefraction = _generateRefractationRay(ray.direction, hitRec.p, normal, Kn,
                                           rRay, reflectivity);

    if (bRefraction) {
      glm::vec3 rColor = _traceRay(rRay, pScene, depth + 1);
      color += (1.0f - reflectivity) * mtl->Kt * rColor;
    }
  }  // end of if(transparent object)

  // reflection
  float Ks = glm::clamp(reflectivity + mtl->Ks, 0.0f, 1.0f);
  if (Ks > 0) {
    Ray rRay = _generateReflectionRay(ray.direction, hitRec.p, hitRec.normal);
    glm::vec3 rColor = _traceRay(rRay, pScene, depth + 1);
    color += rColor * Ks;
  }

  return color;
}

glm::vec3 WhittedRayTracer::_shade(const glm::vec3& dir,
                                   const HitRecord& hitRec, MyScene* pScene) {
  DemoScene* scene = dynamic_cast<DemoScene*>(pScene);
  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);

  glm::vec3 albedo = mtl->sampleAlbedo(hitRec.uv, hitRec.p);

  glm::vec3 color(0);

  const auto& lights = scene->getLights();
  for (const auto& light : lights) {
    Ray shadowRay = light->generateShadowRay(hitRec.p);
    HitRecord hitRecS;
    constexpr float SHADOW_E = 0.001f;

    float attenuation = 1.0f;
    auto shadowHitCallback = [&attenuation](const HitRecord& hit) {
      Material* mtl = dynamic_cast<Material*>(hit.mtl);
      if (mtl) attenuation *= std::powf(mtl->Kt, 2.5f);
      return attenuation > 0.0f;
    };

    pScene->anyHit(shadowRay, SHADOW_E, fMax, shadowHitCallback);

    // lighting
    glm::vec3 lgt =
        light->blinnPhongShading(hitRec.p, hitRec.normal, dir, mtl->n);

    // ambient lighting
    color += lgt.x * albedo;

    // diffuse lighting
    color += mtl->Kd * lgt.y * attenuation * albedo;

    // specular lighting
    color += lgt.z * attenuation * albedo;
  }  // end of for each light

  return color;
}

Ray WhittedRayTracer::_generateReflectionRay(const glm::vec3& dir,
                                             const glm::vec3& point,
                                             const glm::vec3& normal) {
  glm::vec3 outDir = glm::reflect(dir, normal);
  return Ray(point, outDir);
}

bool WhittedRayTracer::_generateRefractationRay(const glm::vec3& dir,
                                                const glm::vec3& point,
                                                const glm::vec3& normal,
                                                float Kn, Ray& outRay,
                                                float& outReflectivity) {
  glm::vec3 outwardNormal;
  float niOverNt, cosine;

  cosine = glm::dot(dir, normal);
  if (cosine > 0) {
    outwardNormal = -normal;
    niOverNt = Kn;
  } else {
    outwardNormal = normal;
    niOverNt = 1.0f / Kn;
    cosine = -cosine;
  }

  glm::vec3 refracted;
  bool ret;

  if (myRefract(dir, outwardNormal, niOverNt, refracted)) {
    outReflectivity = mySchlick(cosine, Kn, 5.0f);
    outRay = Ray(point, refracted);
    ret = true;
  } else {
    // total internal reflection
    outReflectivity = 1.0f;
    ret = false;
  }

  return ret;
}

}  // namespace RayTracingHistory
