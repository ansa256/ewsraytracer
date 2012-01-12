#include <math.h>
#include "Sphere.h"
#include "Math/Vector3D.h"
#include "Parser/Hash.h"
#include "Materials/Material.h"
#include "Math/Maths.h"
#include "Math/Matrix.h"
#include "Textures/Texture.h"
#include "Samplers/Sampler.h"

Sphere::Sphere() :
   center(0, 0, 0),
   radius(1.0),
   thetaRange(false),
   phiRange(false),
   cosThetaMin(0),
   cosThetaMax(0),
   phiMin(0),
   phiMax(0)
{
   bbox.expand(Point3D(-1, -1, -1));
   bbox.expand(Point3D(1, 1, 1));

   nSamples = 10;
   idx = 0;
   samples = new float[nSamples*2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

Sphere::Sphere(const Point3D& c, double r) :
   center(c),
   radius(r),
   thetaRange(false),
   phiRange(false),
   cosThetaMin(0),
   cosThetaMax(0),
   phiMin(0),
   phiMax(0)
{
   bbox.expand(center - radius);
   bbox.expand(center + radius);

   nSamples = 10;
   idx = 0;
   samples = new float[nSamples*2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

Sphere::~Sphere() {
   delete[] samples;
}

bool Sphere::hit(const Ray& ray, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) return false;

   Vector3D temp(ray.origin - center);
   double a = ray.direction.dot(ray.direction);
   double b = 2.0 * temp.dot(ray.direction);
   double c = temp.dot(temp) - (radius * radius);
   double disc = b * b - 4.0 * a * c;

   if(disc < 0.0) {
      return false;
   }

   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;

   if(t > epsilon && t < ray.tHit && partCheck(ray, t)) {
      ray.tHit = t;
      sr.normal = (temp + ray.direction * t) / radius;

      if((-ray.direction).dot(sr.normal) < 0.0) {
         sr.normal *= -1.0;
      }

      sr.localHitPoint = sr.hitPoint = ray(t);
      sr.material = material;
      return true;
   }

   t = (-b + e) / denom;
   if(t > epsilon && t < ray.tHit && partCheck(ray, t)) {
      ray.tHit = t;
      sr.normal = (temp + ray.direction * t) / radius;

      if((-ray.direction).dot(sr.normal) < 0.0) {
         sr.normal *= -1;
      }

      sr.localHitPoint = sr.hitPoint = ray(t);
      sr.material = material;
      return true;
   }

   return false;
}

bool Sphere::shadowHit(const Ray& ray) const {
   if(!bbox.hit(ray)) return false;

   Vector3D temp(ray.origin - center);
   double a = ray.direction.dot(ray.direction);
   double b = 2.0 * temp.dot(ray.direction);
   double c = temp.dot(temp) - (radius * radius);
   double disc = b * b - 4.0 * a * c;

   if(disc < 0.0) {
      return false;
   }

   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;

   if(t > epsilon && partCheck(ray, t)) {
      ShadeRecord sr;
      sr.localHitPoint = ray(t);
      float alpha = material->getAlpha(sr, ray);

      if(alpha > 0.5) {
         ray.tHit = t;
         return true;
      }
   }

   t = (-b + e) / denom;
   if(t > epsilon && partCheck(ray, t)) {
      ShadeRecord sr;
      sr.localHitPoint = ray(t);
      float alpha = material->getAlpha(sr, ray);

      if(alpha > 0.5) {
         ray.tHit = t;
         return true;
      }
   }

   return false;
}

bool Sphere::partCheck(const Ray& ray, double t) const {
   if(!thetaRange && !phiRange) {
      return true;
   }

   Vector3D hit = ray.origin + ray.direction * t - center;

   if(thetaRange) {
      if(hit.y > radius * cosThetaMin || hit.y < radius * cosThetaMax) {
         return false;
      }
   }

   if(phiRange) {
      double phi = atan2(hit.x, hit.z);
      if(phi < 0.0) {
         phi += 2.0 * M_PI;
      }
      if(phi < phiMin || phi > phiMax) {
         return false;
      }
   }
   return true;
}

void Sphere::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   radius = hash->getDouble("radius");

   if(hash->contains("thetaRange")) {
      thetaRange = true;
      Array* a = hash->getValue("thetaRange")->getArray();
      double min = a->at(0)->getDouble();
      cosThetaMin = cos(min * DEG_TO_RAD);

      double max = a->at(1)->getDouble();
      cosThetaMax = cos(max * DEG_TO_RAD);
   }

   if(hash->contains("phiRange")) {
      phiRange = true;
      Array* a = hash->getValue("phiRange")->getArray();
      phiMin = a->at(0)->getDouble() * DEG_TO_RAD;
      phiMax = a->at(1)->getDouble() * DEG_TO_RAD;
   }

   bbox.expand(center - radius);
   bbox.expand(center + radius);

   material = Material::createMaterial(hash->getValue("material")->getHash());
}

Point3D Sphere::sample(const Point3D& hitPoint) {
   float x = samples[idx];
   float y = samples[idx+1];
   idx = (idx + 2) % (nSamples * 2);

   // Get a cooredinate system for sphere sampling. z axis is vector from hit point to sphere center
   Vector3D wc = center - hitPoint;
   wc.normalize();
   Vector3D wcx, wcy;
   coordinateSystem(wc, &wcx, &wcy);

   // Sample sphere inside subtended cone
   double sinTheta = radius * radius / hitPoint.distanceSquared(center);
   double cosTheta = sqrt(max(0.0, 1.0 - sinTheta));

   Ray ray(hitPoint, Sampler::uniformSampleCone(x, y, cosTheta, wcx, wcy, wc));
   if(!shadowHit(ray)) {
      ray.tHit = (center - hitPoint).dot(ray.direction.normalize());
   }

   return ray(ray.tHit);
}

Vector3D Sphere::getNormal(const Point3D& point) const {
   Vector3D n(point.x, point.y, point.z);
   n.normalize();
   return n;
}

double Sphere::pdf(const ShadeRecord& sr) const {
   double sinTheta = radius * radius / sr.hitPoint.distanceSquared(center);
   double cosTheta = sqrt(max(0.0, 1.0 - sinTheta));
   return 1.0 / (2.0 * M_PI * (1.0 - cosTheta));
}
