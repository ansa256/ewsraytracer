#ifndef _STARBOX_H_
#define _STARBOX_H_

#include "Materials/Emissive.h"
#include <string>

using namespace std;

class Hash;

class StarBox {
   
public:
   StarBox();
   
   void setHash(Hash* h);
   void createStars();
   
private:
   void createBoxSide(string name, const Vector3D& translate, const Vector3D& rotate);

   string yzNegX;
   string yzPosX;
   string xyNegZ;
   string xyPosZ;
   string xzNegY;
   string xzPosY;
   int halfSize;
};

#endif
