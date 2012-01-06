#ifndef _DISK_H_
#define _DISK_H_

#include "LightObject.h"

/**
 * Hash values:
 *    - center [ x y z ] - Center of the disk
 *    - normal [ x y z ] - Normal of the disk
 *    - radius <i>value</i> - Radius of the disk
 */
class Disk : public LightObject {

public:
   Disk();
   virtual ~Disk();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;

   virtual Point3D sample(const Point3D& hitPoint);
   virtual Vector3D getNormal(const Point3D& point) const;
   virtual double pdf(const ShadeRecord& sr) const;

private:
   Point3D center;
   Vector3D normal;
   float radius;
   float radiusSquared;
   double inverseArea;
   Vector3D a, b;
   float* samples;
   int nSamples, idx;
};

#endif
