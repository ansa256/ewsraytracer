#ifndef __PERFECT_SPECULAR__
#define __PERFECT_SPECULAR__

#include "BRDF.h"
#include "Math/Vector3D.h"

class Fresnel;

class PerfectSpecular: public BRDF 
{

public:
   PerfectSpecular();
   ~PerfectSpecular() {}

   virtual Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
   void setColor(const Color& c) { color = c; }
   
private:
   Color color;
   Fresnel* fresnel;
};

#endif
