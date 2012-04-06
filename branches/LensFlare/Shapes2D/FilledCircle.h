#ifndef LensFlare_FilledCircle_h
#define LensFlare_FilledCircle_h

#include "Shape2D.h"
#include "Utility/Color.h"

class FilledCircle : public Shape2D {

public:
   FilledCircle(int x, int y, int r, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

private:
   int cx, cy;
   int radius;
   Color color1, color2;

   double getF(double x, double y, double radius);
};

#endif
