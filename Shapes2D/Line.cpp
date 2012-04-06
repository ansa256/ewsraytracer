#include "Line.h"
#include "Utility/SDL_Utility.h"
#include "Utility/Math.h"
#include <algorithm>
#include <math.h>

using namespace std;

Line::Line(int _x1, int _y1, int _x2, int _y2, const Color& c1, const Color& c2) :
   x1(_x1), y1(_y1), x2(_x2), y2(_y2), color1(c1), color2(c2)
{
}

Line::Line(int x, int y, int length, float angle, int height, const Color& c1, const Color& c2) {
   x1 = x;
   y1 = y;
   
   if(angle > 360) {
      angle -= 360;
   }
   
   bool swap = (angle > 180);
   angle *= M_PI / 180.f;
   x2 = (int)(length * cos(angle)) + x1;
   y2 = (int)(-length * sin(angle)) + y1;

   color1 = c1;
   color2 = c2;
   
   if(swap) {
      std::swap(color1, color2);
   }
}

void Line::draw(SDL_Surface* surf) {
   if(y1 > y2) {
      std::swap(x1, x2);
      std::swap(y1, y2);
   }

   int32_t dx = x2 - x1;
   int32_t dy = y2 - y1;

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
   int xdir = (dx >= 0) ? 1 : -1;
   dx = abs(dx);

   Uint32 erracc = 0;

   // Draw the initial pixel in the foreground color
   setBlendColor(surf, x1, y1, color1);
   
   float length = sqrt(dx*dx + dy*dy);
   float pos = 0;

   if(dy > dx) {
      Uint32 erradj = ((dx << 16) / dy) << 16;

      int x0pxdir = x1 + xdir;
      while(--dy) {
         int erracctmp = erracc;
         erracc += erradj;
         if(erracc <= erracctmp) {
            x1 = x0pxdir;
            x0pxdir += xdir;
         }

         y1++;

         float f = pos++ / length;
         Color c = lerp(f, color1, color2);
         Uint32 wgt = (erracc >> 24) & 255;
         setBlendColor(surf, x1, y1, c, 255 - wgt);
         setBlendColor(surf, x0pxdir, y1, c, wgt);
      }
   }
   else {
      Uint32 erradj = ((dy << 16) / dx) << 16;

      int y0p1 = y1 + 1;
      while(--dx) {
         int erracctmp = erracc;
         erracc += erradj;
         if(erracc <= erracctmp) {
            y1 = y0p1;
            y0p1++;
         }
         x1 += xdir;
         
         float f = pos++ / length;
         Color c = lerp(f, color1, color2);
         Uint32 wgt = (erracc >> 24) & 255;
         setBlendColor(surf, x1, y1, c, 255 - wgt);
         setBlendColor(surf, x1, y0p1, c, wgt);
      }
   }

   setBlendColor(surf, x2, y2, color2);
}

void Line::horizontal(SDL_Surface* surf, int y) {
   if (x1 > x2) {
      swap(x1, x2);
   }

   x1 = max(x1, (int) surf->clip_rect.x);
   x2 = min(x2, (int) (surf->clip_rect.x + surf->clip_rect.w - 1));
   float dx = x2 - x1;

   for (int x = x1; x <= x2; x++) {
      float f = (x - x1) / dx;
      Color c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}

void Line::vertical(SDL_Surface * surf, int x) {
   if (y1 > y2) {
      swap(y1, y2);
   }

   y1 = max(y1, (int) surf->clip_rect.y);
   y2 = min(y2, (int) (surf->clip_rect.y + surf->clip_rect.h - 1));

   float dy = y2 - y1;

   for (int y = y1; y <= y2; y++) {
      float f = (y - y1) / dy;
      Color c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}

