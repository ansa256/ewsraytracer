#include "PositionLight.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Math/Maths.h"
#include "Storage/Storage.h"

PositionLight::PositionLight(float halfDistance) : Light(), color(1, 1, 1), location() {
   attenuationPower = LogN(2, halfDistance);
}

PositionLight::PositionLight(const Point3D& loc, float halfDistance) : Light(), color(1, 1, 1), location(loc) {
   attenuationPower = LogN(2, halfDistance);
}

Vector3D PositionLight::getLightDirection(ShadeRecord& sr) {
   return (location - sr.hitPoint).normalize();
}

bool PositionLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double d = location.distance(ray.origin);

   if(GeometryManager::instance().getStorage()->shadowHit(ray) && (ray.tHit < d)) {
      return true;
   }
   return false;
}

void PositionLight::setHash(Hash* hash) {
   location.set(hash->getValue("location")->getArray());
   color.set(hash->getValue("color")->getArray());
   color *= hash->getDouble("radiance");

   if(hash->contains("halfDistance")) {
      attenuationPower = LogN(2, hash->getDouble("halfDistance"));
   }
}

Color PositionLight::L(const ShadeRecord& sr) {
//   double d = (location - sr.hitPoint).length();
   return color; // pow(d, attenuationPower);
}
