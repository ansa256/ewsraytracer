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
   if(x1 == x2) {
      vertical(surf, x1);
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
   if (x1 > x2) {
      swap(x1, x2);
   }

   x1 = max(x1, (Uint16) surf->clip_rect.x);
   x2 = min(x2, (Uint16) (surf->clip_rect.x + surf->clip_rect.w - 1));
   float dx = x2 - x1;

   for (int x = x1; x <= x2; x++) {
      float f = (x - x1) / dx;
      Color c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}

void Line::vertical(SDL_Surface * surf, Uint16 x) {
   if (y1 > y2) {
      swap(y1, y2);
   }

   y1 = max(y1, (Uint16) surf->clip_rect.y);
   y2 = min(y2, (Uint16) (surf->clip_rect.y + surf->clip_rect.h - 1));

   float dy = y2 - y1;

   for (int y = y1; y <= y2; y++) {
      float f = (y - y1) / dy;
      Color c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}
