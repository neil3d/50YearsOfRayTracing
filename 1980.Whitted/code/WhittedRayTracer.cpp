#include "WhittedRayTracer.h"

#include <algorithm>

#include "Material.h"
#include "Scene.h"

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

void WhittedRayTracer::_renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
  mEyePos = camera->getEyePos();
  camera->getFocalPlane(mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop);

  MyScene* pScene = scene.get();
  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      Ray viewRay = _generateViewRay(x, y);
      glm::vec3 color = _rayShading(viewRay, scene.get(), 0);
      _writePixel(x, y, glm::vec4(color, 1));
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 WhittedRayTracer::_backgroundColor(const Ray& ray) {
  // blue-white linear gradient background
  const glm::vec3 topColor(0.9f, 0.9f, 1);
  const glm::vec3 bottomColor(0.4f, 0.6f, 1);
  float t = ray.direction.y;
  return topColor * t + bottomColor * (1.0f - t);
}

glm::vec3 WhittedRayTracer::_rayShading(Ray ray, MyScene* pScene, int depth) {
  if (depth > MAX_DEPTH) return _backgroundColor(ray);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.001f, fMax, hitRec);
  if (!bHit) return _backgroundColor(ray);

  glm::vec3 color(0);
  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);
  glm::vec3 albedo(1);
  if (hitRec.mtl) albedo = mtl->sampleAlbedo(hitRec.uv, hitRec.p);

  float Ia = 0;
  Scene* scene = dynamic_cast<Scene*>(pScene);
  const auto& lights = scene->getLights();
  for (const auto& light : lights) {
    Ray shadowRay = light->generateShadowRay(hitRec.p);
    HitRecord hitRecS;
    constexpr float SHADOW_E = 0.1f;

    float attenuation = 1.0f;
    auto shadowHitCallback = [&attenuation](const HitRecord& hit) {
      Material* mtl = dynamic_cast<Material*>(hit.mtl);
      if (mtl) attenuation *= mtl->Kt;
      return attenuation > 0.0f;
    };

    pScene->anyHit(shadowRay, SHADOW_E, fMax, shadowHitCallback);

    float lgt = light->blinnPhongShading(hitRec.p, hitRec.normal, ray.direction,
                                         mtl->Kd, mtl->Ks, mtl->n);

    Ia += light->ambient;
    Ia += lgt * attenuation;
  }  // end of for each light
  color = Ia * albedo;

  if (!mtl) return color;

  // recursive
  float reflectivity = 0.0f;
  if (mtl->Kt > 0) {
    float Kn = mtl->Kn;
    glm::vec3 normal = hitRec.normal;

    Ray rRay;
    bool bRefraction = false;

    bRefraction = _generateRefractationRay(ray.direction, hitRec.p, normal, Kn,
                                           rRay, reflectivity);

    if (bRefraction) {
      glm::vec3 rColor = _rayShading(rRay, pScene, depth + 1);
      color += (1.0f - reflectivity) * mtl->Kt * rColor;
    }
  }  // end of if(transparent object)

  float Ks = std::max(mtl->Ks, reflectivity);
  if (Ks > 0) {
    Ray rRay = _generateReflectionRay(ray.direction, hitRec.p, hitRec.normal);
    glm::vec3 rColor = _rayShading(rRay, pScene, depth + 1);
    color += rColor * Ks;
  }

  return color;
}

Ray WhittedRayTracer::_generateViewRay(int x, int y) {
  float s = (x + 0.5f) / (float)(mFrameWidth);
  float t = (y + 0.5f) / (float)(mFrameHeight);

  glm::vec3 origin = mEyePos;
  return Ray(origin,
             mFocalPlaneLeftTop + s * mFocalPlaneH - t * mFocalPlaneV - origin);
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
