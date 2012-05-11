#ifndef _GLASS_H_
#define _GLASS_H_

#include "Phong.h"

class SpecularTransmission;

class Glass : public Phong {

public:
   Glass();
   virtual ~Glass();

   virtual void setHash(Hash* hash);

private:
   SpecularTransmission* stBRDF;
};

#endif
