#include "SpotLight.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Math/Maths.h"
#include "Storage/Storage.h"

SpotLight::SpotLight() : Light(), ls(1.0), color(1, 1, 1), cosWidth(0), cosFalloff(0), location(), direction() {
   samples = new float[2];
   samples[0] = samples[1] = 0.5;
}

SpotLight::~SpotLight() {
   delete[] samples;
}

bool SpotLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double d = location.distance(ray.origin);

   if(GeometryManager::instance().getStorage()->shadowHit(ray) && (ray.tHit < d)) {
      return true;
   }
   return false;
}

void SpotLight::setHash(Hash* hash) {
   location.set(hash->getValue("location")->getArray());

   direction.set(hash->getValue("direction")->getArray());
   direction.normalize();

   color.set(hash->getValue("color")->getArray());
   ls = hash->getDouble("radiance");

   float width = hash->getDouble("width");
   cosWidth = cos(DEG_TO_RAD * width);

   float falloff = hash->getDouble("falloff");
   cosFalloff = cos(DEG_TO_RAD * falloff);
}

double SpotLight::falloff(const Point3D& p) const {
   Vector3D v = p - location;
   v.normalize();
   double cosTheta = v.dot(direction);

   if(cosTheta < cosWidth) {
      return 0;
   }
   if(cosTheta > cosFalloff) {
      return 1;
   }
   double delta = (cosTheta - cosWidth) / (cosFalloff - cosWidth);
   return delta * delta * delta * delta;
}

Color SpotLight::Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const {
   lightDir = -direction;
   pdf = 1.0;
   double f = falloff(sr.hitPoint);
   return color * ls * f;
}

float* SpotLight::getSamples() {
   return samples;
}
