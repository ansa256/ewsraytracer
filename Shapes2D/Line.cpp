#include "Line.h"
#include "Utility/SDL_Utility.h"
#include <algorithm>

using namespace std;

const int AAbits = 8;

Line::Line(Uint16 _x1, Uint16 _y1, Uint16 _x2, Uint16 _y2, const Color& c1, const Color& c2) :
   x1(_x1), y1(_y1), x2(_x2), y2(_y2), color1(c1), color2(c2)
{
}

void Line::draw(SDL_Surface* surf) {
   if(y1 > y2) {
      std::swap(x1, x2);
      std::swap(y1, y2);
   }

   int dx = x2 - x1;
   int dy = y2 - y1;

   // special cases
   if(y1 == y2) {
      horizontal(surf, y1);
      return ;
   }

   // Adjust for negative dx and set xdir
   short xdir = (dx > 0) ? 1 : -1;
   dx = abs(dx);

   int erracc = 0;
   int intshift = 32 - AAbits;

   // Draw the initial pixel in the foreground color
   setColor(surf, x1, y1, color1);

   if(dy > dx) {
      int erradj = ((dx << 16) / dy) << 16;

      int x0pxdir = x1 + xdir;
      while(--dy) {
         int erracctmp = erracc;
         erracc += erradj;
         if(erracc <= erracctmp) {
            x1 = x0pxdir;
            x0pxdir += xdir;
         }

         y1++;

         int wgt = (erracc >> intshift) & 255;
//         setWeightedColor(surf, x1, y1, color, 255 - wgt);
//         setWeightedColor(surf, x0pxdir, y1, color, wgt);
      }
   }
   else {
      int erradj = ((dy << 16) / dx) << 16;

      int y0p1 = y1 + 1;
      while(--dx) {
         int erracctmp = erracc;
         erracc += erradj;
         if(erracc <= erracctmp) {
            y1 = y0p1;
            y0p1++;
         }
         x1 += xdir;

         int wgt = (erracc >> intshift) & 255;
//         setWeightedColor(surf, x1, y1, color, 255 - wgt);
//         setWeightedColor(surf, x1, y0p1, color, wgt);
      }
   }

   setColor(surf, x2, y2, color1);
}

void Line::horizontal(SDL_Surface* surf, Uint16 y) {
   Sint16 left, right, top, bottom;
   Uint8 *pixel, *pixellast;
   int dx;
   int pixx, pixy;

   /*
    * Swap x1, x2 if required to ensure x1<=x2
    */
   if (x1 > x2) {
      swap(x1, x2);
   }

   /*
    * Get clipping boundary and
    * check visibility of hline
    */
   left = surf->clip_rect.x;
   right = surf->clip_rect.x + surf->clip_rect.w - 1;
   top = surf->clip_rect.y;
   bottom = surf->clip_rect.y + surf->clip_rect.h - 1;

   /*
    * Clip x
    */
   if (x1 < left) {
      x1 = left;
   }
   if (x2 > right) {
      x2 = right;
   }

   /*
    * More variable setup
    */
   pixx = surf->format->BytesPerPixel;
   pixy = surf->pitch;
   pixel = ((Uint8 *) surf->pixels) + pixx * (int) x1 + pixy * (int) y;

   dx = x2 - x1;
   dx = 4 * dx;
   pixellast = pixel + dx;
   int x = x1;

   for (; pixel <= pixellast; pixel += pixx) {
      float f = 1.f - (pixellast - pixel) / (float) dx;
      Color color = lerp(f, color1, color2);
//      Uint32 c = SDL_MapRGBA(surf->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
//      *(Uint32 *) pixel = c;
      setBlendColor(surf, x, (int) y, color);
      x++;
   }
}
