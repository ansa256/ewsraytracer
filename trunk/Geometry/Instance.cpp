#include "Instance.h"
#include "Parser/Hash.h"
#include "GeometryManager.h"

const double kHugeValue = 1.0E10;

Instance::Instance() : object(NULL), invMatrix(), fwdMatrix() {
}

Instance::Instance(GeometryObject* obj) : object(obj), invMatrix(), fwdMatrix() {
}

Instance::~Instance() {
   if(object->doDelete) {
      delete object;
   }
}

void Instance::setHash(Hash* hash) {
   Hash* objHash = hash->getValue("object")->getHash();
   string type = objHash->getString("type");

   object = GeometryManager::instance().createObject(type, objHash, false);

   if(hash->contains("material")) {
      setupMaterial(hash->getValue("material")->getHash());
   }
   else {
      material = object->getMaterial();
   }

   if(hash->contains("translate")) {
      Array* a = hash->getValue("translate")->getArray();
      invMatrix.invTranslate(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
   }
   if(hash->contains("scale")) {
      Array* a = hash->getValue("scale")->getArray();
      invMatrix.invScale(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
   }
   if(hash->contains("rotate")) {
      Array* a = hash->getValue("rotate")->getArray();
      invMatrix.invRotateX(a->at(0)->getDouble());
      invMatrix.invRotateY(a->at(1)->getDouble());
      invMatrix.invRotateZ(a->at(2)->getDouble());
   }

   computeBBox();
}

bool Instance::hit(const Ray& ray, ShadeRecord& sr) const {
   Ray invRay;
   invRay.origin = invMatrix * ray.origin;
   invRay.direction = invMatrix * ray.direction;
   invRay.tHit = ray.tHit;

   if(object->hit(invRay, sr)) {
      sr.normal = invMatrix.transformNormal(sr.normal);
      sr.normal.normalize();
      ray.tHit = invRay.tHit;
      sr.localHitPoint = ray(ray.tHit);
      return true;
   }

   return false;
}

bool Instance::shadowHit(const Ray& ray) const {
   Ray invRay;
   invRay.origin = invMatrix * ray.origin;
   invRay.direction = invMatrix * ray.direction;
   invRay.tHit = ray.tHit;

   if(object->shadowHit(invRay)) {
      ray.tHit = invRay.tHit;
      return true;
   }
   return false;
}

void Instance::computeBBox() {
   BBox object_bbox = object->bbox;

   // Apply the affine transformations to the box.
   // We must apply the transformations to all 8 vertices of the orginal box
   // and then work out the new minimum and maximum values

   Point3D v[8];
   v[0].x = object_bbox.x0; v[0].y = object_bbox.y0; v[0].z = object_bbox.z0;
   v[1].x = object_bbox.x1; v[1].y = object_bbox.y0; v[1].z = object_bbox.z0;
   v[2].x = object_bbox.x1; v[2].y = object_bbox.y1; v[2].z = object_bbox.z0;
   v[3].x = object_bbox.x0; v[3].y = object_bbox.y1; v[3].z = object_bbox.z0;

   v[4].x = object_bbox.x0; v[4].y = object_bbox.y0; v[4].z = object_bbox.z1;
   v[5].x = object_bbox.x1; v[5].y = object_bbox.y0; v[5].z = object_bbox.z1;
   v[6].x = object_bbox.x1; v[6].y = object_bbox.y1; v[6].z = object_bbox.z1;
   v[7].x = object_bbox.x0; v[7].y = object_bbox.y1; v[7].z = object_bbox.z1;

   // Transform these using the forward matrix

   v[0] = fwdMatrix * v[0];
   v[1] = fwdMatrix * v[1];
   v[2] = fwdMatrix * v[2];
   v[3] = fwdMatrix * v[3];
   v[4] = fwdMatrix * v[4];
   v[5] = fwdMatrix * v[5];
   v[6] = fwdMatrix * v[6];
   v[7] = fwdMatrix * v[7];

   // Compute the min and max values

   bbox.reset();

   for (int j = 0; j < 8; j++)  {
      bbox.x0 = min(bbox.x0, v[j].x);
      bbox.y0 = min(bbox.y0, v[j].y);
      bbox.z0 = min(bbox.z0, v[j].z);

      bbox.x1 = max(bbox.x1, v[j].x);
      bbox.y1 = max(bbox.y1, v[j].y);
      bbox.z1 = max(bbox.z1, v[j].z);
   }
}
