#ifndef _STARBOX_H_
#define _STARBOX_H_

#include "Materials/Emissive.h"

class Hash;

class StarBox {
   
public:
   StarBox();
   
   void setHash(Hash* h);
   void createStars();
   
private:
   void createBoxSide(string name, const Vector3D& translate, const Vector3D& rotate);

   int count;
   int halfSize;
   int size;   
};

#endif
