#include "AnalyticalSphere.h"

#include "MyScene.h"

namespace RTKit2 {
static void sphereIntersectFunc(const RTCIntersectFunctionNArguments* args);
static void sphereOccludedFunc(const RTCOccludedFunctionNArguments* args);
static void sphereBoundsFunc(const struct RTCBoundsFunctionArguments* args);

AnalyticalSphere::~AnalyticalSphere() {
  if (mGeometry) {
    rtcReleaseGeometry(mGeometry);
    mGeometry = nullptr;
  }
}

void AnalyticalSphere::create(const glm::vec3& p, float r) {
  mCenter = p;
  mRadius = r;

  RTCScene rtcScene = mOwner->getRTCScene();
  RTCDevice rtcDevice = rtcGetSceneDevice(rtcScene);
  mGeometry = rtcNewGeometry(rtcDevice, RTC_GEOMETRY_TYPE_USER);

  rtcSetGeometryUserPrimitiveCount(mGeometry, 1);
  rtcSetGeometryUserData(mGeometry, this);

  rtcSetGeometryBoundsFunction(mGeometry, sphereBoundsFunc, nullptr);
  rtcSetGeometryIntersectFunction(mGeometry, sphereIntersectFunc);
  rtcSetGeometryOccludedFunction(mGeometry, sphereOccludedFunc);

  rtcCommitGeometry(mGeometry);
  mRTCID = rtcAttachGeometry(rtcScene, mGeometry);
}

// ======================================================================== //
//                     User defined sphere geometry                         //
// ======================================================================== //
inline void copyInstanceIdStack(const RTCIntersectContext* ctx, unsigned* tgt) {
  tgt[0] = ctx->instID[0];
}

void sphereBoundsFunc(const struct RTCBoundsFunctionArguments* args) {
  const AnalyticalSphere* sphere =
      (const AnalyticalSphere*)args->geometryUserPtr;
  const glm::vec3& P = sphere->getCenter();
  float R = sphere->getRadius();

  RTCBounds* bounds_o = args->bounds_o;
  
  bounds_o->lower_x = P.x - R;
  bounds_o->lower_y = P.y - R;
  bounds_o->lower_z = P.z - R;

  bounds_o->upper_x = P.x + R;
  bounds_o->upper_y = P.y + R;
  bounds_o->upper_z = P.z + R;
}

void sphereIntersectFunc(const RTCIntersectFunctionNArguments* args) {
  int* valid = args->valid;
  if (!valid[0]) return;

  void* ptr = args->geometryUserPtr;
  RTCRayHit* rayhit = (RTCRayHit*)args->rayhit;
  RTCRay& ray = rayhit->ray;
  RTCHit& hit = rayhit->hit;
  unsigned int primID = args->primID;

  assert(args->N == 1);
  const AnalyticalSphere* sphere =
      (const AnalyticalSphere*)args->geometryUserPtr;

  const glm::vec3& P = sphere->getCenter();
  float R = sphere->getRadius();
  glm::vec3 org(ray.org_x, ray.org_y, ray.org_z);
  glm::vec3 dir(ray.dir_x, ray.dir_y, ray.dir_z);

  const glm::vec3 v = org - P;
  const float A = glm::dot(dir, dir);
  const float B = 2.0f * glm::dot(v, dir);
  const float C = glm::dot(v, v) - R * R;
  const float D = B * B - 4.0f * A * C;
  if (D < 0.0f) return;

  const float Q = glm::sqrt(D);
  const float rcpA = 1.0f / A;
  const float t0 = 0.5f * rcpA * (-B - Q);
  const float t1 = 0.5f * rcpA * (-B + Q);

  RTCHit potentialHit;
  potentialHit.u = 0.0f;
  potentialHit.v = 0.0f;
  copyInstanceIdStack(args->context, potentialHit.instID);
  potentialHit.geomID = sphere->getRTCID();
  potentialHit.primID = primID;

  if ((ray.tnear < t0) & (t0 < ray.tfar)) {
    int imask;
    bool mask = 1;
    { imask = mask ? -1 : 0; }

    const glm::vec3 Ng = org + t0 * dir - P;
    potentialHit.Ng_x = Ng.x;
    potentialHit.Ng_y = Ng.y;
    potentialHit.Ng_z = Ng.z;

    RTCFilterFunctionNArguments fargs;
    fargs.valid = (int*)&imask;
    fargs.geometryUserPtr = ptr;
    fargs.context = args->context;
    fargs.ray = (RTCRayN*)args->rayhit;
    fargs.hit = (RTCHitN*)&potentialHit;
    fargs.N = 1;

    const float old_t = ray.tfar;
    ray.tfar = t0;
    rtcFilterIntersection(args, &fargs);

    if (imask == -1)
      hit = potentialHit;
    else
      ray.tfar = old_t;
  }

  if ((ray.tnear < t1) & (t1 < ray.tfar)) {
    int imask;
    bool mask = 1;
    { imask = mask ? -1 : 0; }

    const glm::vec3 Ng = org + t1 * dir - P;
    potentialHit.Ng_x = Ng.x;
    potentialHit.Ng_y = Ng.y;
    potentialHit.Ng_z = Ng.z;

    RTCFilterFunctionNArguments fargs;
    fargs.valid = (int*)&imask;
    fargs.geometryUserPtr = ptr;
    fargs.context = args->context;
    fargs.ray = (RTCRayN*)args->rayhit;
    fargs.hit = (RTCHitN*)&potentialHit;
    fargs.N = 1;

    const float old_t = ray.tfar;
    ray.tfar = t1;
    rtcFilterIntersection(args, &fargs);

    if (imask == -1)
      hit = potentialHit;
    else
      ray.tfar = old_t;
  }
}

void sphereOccludedFunc(const RTCOccludedFunctionNArguments* args) {
  int* valid = args->valid;
  if (!valid[0]) return;

  RTCRay* ray = (RTCRay*)args->ray;
  unsigned int primID = args->primID;

  assert(args->N == 1);
  const AnalyticalSphere* sphere =
      (const AnalyticalSphere*)args->geometryUserPtr;

  const glm::vec3& P = sphere->getCenter();
  float R = sphere->getRadius();
  glm::vec3 org(ray->org_x, ray->org_y, ray->org_z);
  glm::vec3 dir(ray->dir_x, ray->dir_y, ray->dir_z);

  const glm::vec3 v = org - P;
  const float A = glm::dot(dir, dir);
  const float B = 2.0f * glm::dot(v, dir);
  const float C = glm::dot(v, v) - R * R;
  const float D = B * B - 4.0f * A * C;
  if (D < 0.0f) return;

  const float Q = glm::sqrt(D);
  const float rcpA = 1.0f / A;
  const float t0 = 0.5f * rcpA * (-B - Q);
  const float t1 = 0.5f * rcpA * (-B + Q);

  RTCHit potentialHit;
  potentialHit.u = 0.0f;
  potentialHit.v = 0.0f;
  copyInstanceIdStack(args->context, potentialHit.instID);
  potentialHit.geomID = sphere->getRTCID();
  potentialHit.primID = primID;
  if ((ray->tnear < t0) & (t0 < ray->tfar)) {
    int imask;
    bool mask = 1;
    { imask = mask ? -1 : 0; }

    const glm::vec3 Ng = org + t0 * dir - P;
    potentialHit.Ng_x = Ng.x;
    potentialHit.Ng_y = Ng.y;
    potentialHit.Ng_z = Ng.z;

    RTCFilterFunctionNArguments fargs;
    fargs.valid = (int*)&imask;
    fargs.geometryUserPtr = args->geometryUserPtr;
    fargs.context = args->context;
    fargs.ray = args->ray;
    fargs.hit = (RTCHitN*)&potentialHit;
    fargs.N = 1;

    const float old_t = ray->tfar;
    ray->tfar = t0;
    rtcFilterOcclusion(args, &fargs);

    if (imask == -1)
      ray->tfar = std::numeric_limits<float>::min();
    else
      ray->tfar = old_t;
  }

  if ((ray->tnear < t1) & (t1 < ray->tfar)) {
    int imask;
    bool mask = 1;
    { imask = mask ? -1 : 0; }

    const glm::vec3 Ng = org + t1 * dir - P;
    potentialHit.Ng_x = Ng.x;
    potentialHit.Ng_y = Ng.y;
    potentialHit.Ng_z = Ng.z;

    RTCFilterFunctionNArguments fargs;
    fargs.valid = (int*)&imask;
    fargs.geometryUserPtr = args->geometryUserPtr;
    fargs.context = args->context;
    fargs.ray = args->ray;
    fargs.hit = (RTCHitN*)&potentialHit;
    fargs.N = 1;

    const float old_t = ray->tfar;
    ray->tfar = t1;
    rtcFilterOcclusion(args, &fargs);

    if (imask == -1)
      ray->tfar = std::numeric_limits<float>::min();
    else
      ray->tfar = old_t;
  }
}

}  // namespace RTKit2
