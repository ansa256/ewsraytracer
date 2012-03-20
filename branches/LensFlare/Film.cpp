#include "Film.h"
#include "Filters/MitchellFilter.h"
#include "Filters/BoxFilter.h"
#include <SDL/SDL.h>
#include <math.h>
#include <algorithm>

using namespace std;

Pixel::Pixel() : r(0), g(0), b(0), a(0), weightSum(0) {}

Color Pixel::getColor() {
   if(weightSum <= 0) {
      return Color();
   }
   Color c(r / weightSum, g / weightSum, b / weightSum, a / weightSum);
   c.normalize();
   return c;
}

void Pixel::reset() {
   r = g = b = 0;
   weightSum = 0;
}

Film::Film(int w, int h) : width(w), height(h) {
//   filter = new BoxFilter(1.2, 1.2);
   filter = new MitchellFilter(1.0, 1.0, 1.f/3.f, 1.f/3.f);
   pixels = new Pixel[width * height];
}

Film::~Film() {
   delete filter;
   delete[] pixels;
}

void Film::addSample(float sx, float sy, const Color& c) {
   int x0 = max(0, (int) ceil(sx - filter->xWidth));
   int x1 = min(width-1, (int) floor(sx + filter->xWidth));
   int y0 = max(0, (int) ceil(sy - filter->yWidth));
   int y1 = min(height-1, (int) floor(sy + filter->yWidth));
   
   for(int y = y0; y < y1; y++) {
      for(int x = x0; x < x1; x++) {
         double weight = filter->evaluate(x - sx, y - sy);

         pixels[width * y + x].r += weight * c.red;
         pixels[width * y + x].g += weight * c.green;
         pixels[width * y + x].b += weight * c.blue;
         pixels[width * y + x].a += weight * c.alpha;
         pixels[width * y + x].weightSum += weight;
      }
   }
}

void Film::generateImage(SDL_Surface* surf) {
   for(int w = 0; w < width; w++) {
      for(int h = 0; h < height; h++) {
//         setPixel(surf, w, height - h - 1, pixels[width * h + w].getColor());
         Uint8 *p = (Uint8 *)surf->pixels + h * surf->pitch + w * surf->format->BytesPerPixel;
         Color color = pixels[width * h + w].getColor();
         *(Uint32*)p = SDL_MapRGBA(surf->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
      }
   }
   SDL_UpdateRect(surf, 0, 0, 0, 0);
}

void Film::reset() {
   for(int w = 0; w < width; w++) {
      for(int h = 0; h < height; h++) {
         pixels[width * h + w].reset();
      }
   }
}
