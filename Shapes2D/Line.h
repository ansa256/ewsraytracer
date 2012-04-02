#ifndef LensFlare_Line_h
#define LensFlare_Line_h

#include "Shape2D.h"
#include "Utility/Color.h"

class Line : public Shape2D {

public:
   Line(Uint32 _x1, Uint32 _y1, Uint32 _x2, Uint32 _y2, const Color& c1, const Color& c2);
   Line(Uint32 x, Uint32 y, Uint32 length, float angle, Uint32 height, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

protected:
   Uint32 x1, y1, x2, y2;
   Color color1, color2;

   void horizontal(SDL_Surface* surf, Uint32 y);
   void vertical(SDL_Surface * surf, Uint32 x);
};

#endif
