#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "LightObject.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class Sampler;

class Rectangle : public LightObject {

public:
   Rectangle();
   Rectangle(const Point3D& o, const Vector3D& _a, const Vector3D& _b);
   ~Rectangle();

   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;
   virtual void setHash(Hash* hash);

   virtual Point3D sample(const Point3D& hitPoint);
   virtual Vector3D getNormal(const Point3D& point) const;
   virtual double pdf(const ShadeRecord& sr) const;

private:
   void setup();

   Point3D origin;
   Vector3D a;
   Vector3D b;
   Vector3D normal;

   double lengthASquared;
   double lengthBSquared;
   double inverseArea;
   
   float* samples;
   uint32_t nSamples, idx;
};

#endif
