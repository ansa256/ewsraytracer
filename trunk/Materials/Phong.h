#ifndef _PHONG_H_
#define _PHONG_H_

#include "Material.h"

class Lambertian;
class GlossySpecular;

class Phong : public Material {

public:
   Phong();
   virtual ~Phong();

   virtual void setHash(Hash* hash);
};

#endif
