#ifndef _MARBLE_H_
#define _MARBLE_H_

#include "Material.h"

class Marble : public Material {

public:
   Marble();
   
   virtual void setHash(Hash* hash);
};

#endif
