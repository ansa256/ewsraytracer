#include "Direction.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Storage/Storage.h"

Direction::Direction() : Light(), ls(1.0), color(), direction() {
}

Direction::~Direction() {
}

void Direction::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
   direction.set(hash->getValue("direction")->getArray());
   direction.normalize();
}

Vector3D Direction::getLightDirection(ShadeRecord& sr) {
   return direction;
}

bool Direction::inShadow(const Ray& ray, const ShadeRecord& sr) {
   if(GeometryManager::instance().getStorage()->shadowHit(ray)) {
      return true;
   }
   return false;
}

Color Direction::L(const ShadeRecord& sr) {
   return color * ls;
}
