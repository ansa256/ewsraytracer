#ifndef _ATMOSPHERE_H_
#define _ATMOSPHERE_H_

#include "Material.h"
#include "Utility/Color.h"

class Atmosphere : public Material {

public:
   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   virtual float getAlpha(const ShadeRecord& sr, const Ray& ray) const;
   
private:
   Color color;
};

#endif
