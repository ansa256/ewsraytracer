#include "FilledCircle.h"
#include "Utility/SDL_Utility.h"
#include <math.h>

FilledCircle::FilledCircle(int x, int y, int r, const Color& c1, const Color& c2) :
   cx(x), cy(y), radius(r), color1(c1), color2(c2)
{
}

void FilledCircle::draw(SDL_Surface* surf) {
   static const int BPP = 4;

   double r = (double)radius;

   for (double dy = 1; dy <= r; dy += 1.0) {
      // This loop is unrolled a bit, only iterating through half of the
      // height of the circle.  The result is used to draw a scan line and
      // its mirror image below it.
      // The following formula has been simplified from our original.  We
      // are using half of the width of the circle because we are provided
      // with a center and we need left/right coordinates.
      double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
      int x = cx-dx;

      // Grab a pointer to the left-most pixel for each half of the circle
      Uint8 *target_pixel_a = (Uint8 *)surf->pixels + ((int)(cy + r - dy)) * surf->pitch + x * BPP;
      Uint8 *target_pixel_b = (Uint8 *)surf->pixels + ((int)(cy - r + dy)) * surf->pitch + x * BPP;

      for (; x <= cx + dx; x++) {
         Color c = lerp(getF(dy, x, cx, r), color2, color1);
         setBlendColor(surf, (Uint32*) target_pixel_a, c);
         setBlendColor(surf, (Uint32*) target_pixel_b, c);
         target_pixel_a += BPP;
         target_pixel_b += BPP;
      }
   }
}

double FilledCircle::getF(double y, int x, int cx, double radius) {
   double deltay = radius - y;
   double deltax = fabs(x - cx);
   double dist = sqrt(deltay*deltay + deltax*deltax);
   return (radius - dist) / radius;
}
