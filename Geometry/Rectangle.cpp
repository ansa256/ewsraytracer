#include "Rectangle.h"
#include "Parser/Hash.h"
#include "Samplers/Sampler.h"

Rectangle::Rectangle() : LightObject() {
   nSamples = 10;
   idx = 0;
   samples = new float[nSamples*2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

Rectangle::Rectangle(const Point3D& o, const Vector3D& _a, const Vector3D& _b) :
   LightObject(),
   origin(o),
   a(_a),
   b(_b)
{
   setup();
   nSamples = 10;
   idx = 0;
   samples = new float[nSamples*2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

Rectangle::~Rectangle() {
   delete[] samples;
}

void Rectangle::setHash(Hash* hash) {
   origin.set(hash->getValue("origin")->getArray());
   a.set(hash->getValue("a")->getArray());
   b.set(hash->getValue("b")->getArray());
   setup();

   setupMaterial(hash->getValue("material")->getHash());
}

void Rectangle::setup() {
   normal = a.cross(b).normalize();

   lengthASquared = a.length() * a.length();
   lengthBSquared = b.length() * b.length();

   inverseArea = 1.0 / (a.length() * b.length());

   bbox.expand(origin);
   bbox.expand(origin + a);
   bbox.expand(origin + b);
}

bool Rectangle::hit(const Ray& ray, ShadeRecord& sr) const {
   double t = (origin - ray.origin).dot(normal) / ray.direction.dot(normal);
   if(t <= epsilon || t > ray.tHit) {
      return false;
   }

   Point3D p = ray(t);
   Vector3D d = p - origin;

   float ddota = d.dot(a);
   if(ddota < 0.0 || ddota > lengthASquared) {
      return false;
   }

   float ddotb = d.dot(b);
   if(ddotb < 0.0 || ddotb > lengthBSquared) {
      return false;
   }

   ray.tHit = t;
   sr.normal = normal;
   sr.localHitPoint = sr.hitPoint = p;
   sr.material = material;
   sr.tu = ddota / lengthASquared;
   sr.tv = ddotb / lengthBSquared;

   return true;
}

bool Rectangle::shadowHit(const Ray& ray) const {
   if(ignoreShadow) {
      return false;
   }

   double t = (origin - ray.origin).dot(normal) / ray.direction.dot(normal);
   if(t <= epsilon) {
      return false;
   }

   Point3D p = ray.origin + ray.direction * t;
   Vector3D d = p - origin;

   float ddota = d.dot(a);
   if(ddota < 0.0 || ddota > lengthASquared) {
      return false;
   }

   float ddotb = d.dot(b);
   if(ddotb < 0.0 || ddotb > lengthBSquared) {
      return false;
   }

   ray.tHit = t;
   return true;
}

Point3D Rectangle::sample(const Point3D& hitPoint) {
   float x = samples[idx];
   float y = samples[idx+1];
   idx = (idx + 2) % (nSamples * 2);
   return (origin + a * x + b * y);
}

Vector3D Rectangle::getNormal(const Point3D& point) const {
   return normal;
}

double Rectangle::pdf(const ShadeRecord& sr) const {
   return inverseArea;
}
