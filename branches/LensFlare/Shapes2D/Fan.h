#ifndef LensFlare_Fan_h
#define LensFlare_Fan_h

#include "Shape2D.h"
#include "Utility/Color.h"

class Fan : public Shape2D {

public:
   Fan(int _x, int _y, int _length, float _angle, int _spread, int _height, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

protected:
   int x, y, length, angle, spread, height;
   Color color1, color2;
};

#endif

