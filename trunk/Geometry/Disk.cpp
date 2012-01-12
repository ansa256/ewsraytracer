#include "Disk.h"
#include "Parser/Hash.h"
#include "Samplers/Sampler.h"
#include "Materials/Material.h"
#include <math.h>

Disk::Disk() : LightObject(), center(), normal(), radiusSquared(1) {
   nSamples = 10;
   idx = 0;
   samples = new float[nSamples*2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

Disk::~Disk() {
   delete[] samples;
}

void Disk::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   normal.set(hash->getValue("normal")->getArray());
   normal.normalize();

   radius = hash->getDouble("radius");
   radiusSquared = radius * radius;
   inverseArea = 1.0 / M_PI * radiusSquared;

   a = Vector3D(0, 0, 1).cross(normal);
   b = normal.cross(a);

   material = Material::createMaterial(hash->getValue("material")->getHash());
}

bool Disk::hit(const Ray& ray, ShadeRecord& sr) const {
   float t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);

   if(t < epsilon || t > ray.tHit) {
      return false;
   }

   Point3D p = ray(t);
   if(center.distanceSquared(p) < radiusSquared) {
      ray.tHit = t;
      sr.normal = normal;
      sr.localHitPoint = sr.hitPoint = p;
      sr.material = material;
      return true;
   }
   return false;
}

bool Disk::shadowHit(const Ray& ray) const {
   float t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);

   if(t < epsilon) {
      return false;
   }

   Point3D p = ray.origin + ray.direction * t;
   if(center.distanceSquared(p) < radiusSquared) {
      ray.tHit = t;
      return true;
   }
   return false;
}

Point3D Disk::sample(const Point3D& hitPoint) {
   float x, y;
   Sampler::mapToDisk(samples[idx], samples[idx+1], &x, &y);
   idx = (idx + 2) % (nSamples * 2);
   return (center + a * radius * x + b * radius * y);
}

Vector3D Disk::getNormal(const Point3D& point) const {
   return normal;
}

double Disk::pdf(const ShadeRecord& sr) const {
   return inverseArea;
}
