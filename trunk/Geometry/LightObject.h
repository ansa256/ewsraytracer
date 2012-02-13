#ifndef _LIGHT_OBJECT_H_
#define _LIGHT_OBJECT_H_

#include "GeometryObject.h"
#include "Math/Vector3D.h"

class LightObject : public GeometryObject {

public:
   LightObject() {}
   virtual ~LightObject() {}

   virtual Point3D sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal) = 0;
   virtual double pdf(const ShadeRecord& sr) const = 0;
};

#endif
