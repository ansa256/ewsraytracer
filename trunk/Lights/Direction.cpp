#include "Direction.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Storage/Storage.h"

Direction::Direction() : Light(), color(), direction() {
   samples = new float[2];
   samples[0] = samples[1] = 0.5;
}

Direction::~Direction() {
   delete[] samples;
}

void Direction::setHash(Hash* hash) {
   color.set(hash->getValue("color")->getArray());
   color *= hash->getDouble("radiance");
   direction.set(hash->getValue("direction")->getArray());
   direction.normalize();
}

bool Direction::inShadow(const Ray& ray, const ShadeRecord& sr) {
   if(GeometryManager::instance().getStorage()->shadowHit(ray)) {
      return true;
   }
   return false;
}

Color Direction::Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const {
   lightDir = direction;
   pdf = 1.0;
   return color;
}

float* Direction::getSamples() {
   return samples;
}
