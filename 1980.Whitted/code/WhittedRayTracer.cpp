#include "WhittedRayTracer.h"

#include <algorithm>

#include "Material.h"
#include "Scene.h"

constexpr float fMax = std::numeric_limits<float>::max();
constexpr int MAX_DEPTH = 32;

float fresnelSchlick(const float cosTheta, const float exponent) {
  float e = std::powf(std::max(0.0f, 1.0f - cosTheta), exponent);
  return glm::clamp(e, 0.0f, 1.0f);
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

glm::vec3 WhittedRayTracer::_rayShading(Ray ray, MyScene* pScene, int depth) {
  if (depth > MAX_DEPTH) return glm::vec3(0);

  HitRecord hitRec;
  bool bHit = pScene->hit(ray, 0.001f, fMax, hitRec);
  if (!bHit) {
    // blue-white linear gradient background
    const glm::vec3 topColor(0.9f, 0.9f, 1);
    const glm::vec3 bottomColor(0.4f, 0.6f, 1);
    float t = ray.direction.y;
    return topColor * t + bottomColor * (1.0f - t);
  }

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
    bool bShadow = pScene->hit(shadowRay, SHADOW_E, fMax, hitRecS);

    if (!bShadow) {
      Ia += light->shadingIntensity(hitRec.p, hitRec.normal, ray.direction,
                                    mtl->Kd, mtl->Ks, mtl->n);
    } else {
      Ia += light->ambient;
    }

  }  // end of for each light
  color = Ia * albedo;

  // recursive
  if (mtl) {
    if (mtl->Ks > 0) {
      Ray rRay = _generateReflectionRay(ray.direction, hitRec.p, hitRec.normal);
      glm::vec3 rColor = _rayShading(rRay, pScene, depth + 1);
      color += rColor * mtl->Ks;
    }

    if (mtl->Kt > 0) {
      float Kn = mtl->Kn;
      Ray rRay;

      // check for inside or outside reflection
      float cosTheta = glm::dot(ray.direction, hitRec.normal);
      if (cosTheta < 0.0f) {
        cosTheta = -cosTheta;
        rRay = _generateRefractationRay(ray.direction, hitRec.p, hitRec.normal,
                                        Kn);
      } else {
        rRay = _generateRefractationRay(ray.direction, hitRec.p, -hitRec.normal,
                                        1.0f/Kn);
        cosTheta = glm::dot(rRay.direction, hitRec.normal);
      }

      float f = fresnelSchlick(cosTheta, 3.0f);
      glm::vec3 rColor = _rayShading(rRay, pScene, depth + 1);
      color += (1.0f - f) * mtl->Kt * rColor;
    }  // end of if(transparent object)

  }  // end of if(mtl)

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

Ray WhittedRayTracer::_generateRefractationRay(const glm::vec3& dir,
                                               const glm::vec3& point,
                                               const glm::vec3& normal,
                                               float Kn) {
  glm::vec3 refract = glm::refract(dir, normal, Kn);
  return Ray(point, refract);
}
