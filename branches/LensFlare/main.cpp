#include <SDL/SDL.h>
#include "Utility/SDL_Utility.h"
#include "Shapes2D/ThickLine.h"
#include "Shapes2D/FilledEllipse.h"
#include "Shapes2D/Ring.h"
#include "Falloff/FalloffFilter.h"
#include <string>

const int width = 960;
const int height = 540;
const int cx = 200;
const int cy = 150;

const int COUNT = 6;
const float START = 20;

Color blue(0.1, 0.3, 0.8, 0);
Color yellow(1, 0.7, 0.1, 0);
Color white(1, 1, 1, 1);

void run();
void star(SDL_Surface* surface, int numSpikes, float startAngle);
double* CreateGaussianFilter(double piSigma, double piAlpha, int& lSize);
SDL_Surface* Convolute(SDL_Surface* source, double* filter, int fSize);

void run() {
   SDL_Event event;
   bool stopApp = false;

   while(!stopApp) {
      SDL_WaitEvent(&event);
      switch (event.type) {
         case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) {
               stopApp = true;
            }
            break;

         case SDL_QUIT:
            stopApp = true;
            break;
      }
   }

   SDL_Quit();
}

void star(SDL_Surface* surface, int numSpikes, float startAngle) {
   int length = 300;
   float offset = 360.f / numSpikes;

   for(float a = startAngle; a < 360 + startAngle; a += offset) {
      ThickLine line(cx, cy, length, a, 40, white, yellow);
      line.draw(surface);
   }

   ThickLine line1(cx, cy, 800, 0, 50, white, blue);
   line1.draw(surface);
   ThickLine line2(cx, cy, 800, 180, 50, white, blue);
   line2.draw(surface);

   FilledEllipse center(cx, cy, 100, 100, white, yellow);
   center.setFilter(new SmoothStepFilter(0.1, 1.0));
   center.draw(surface);
}

int getFarX(float percent) {
   static int centerX = width / 2;

   int x = abs((width-cx) - centerX) * percent;
   if(cx < centerX) {
      x += centerX;
   }
   else {
      x = centerX - x;
   }
   return x;
}

int getNearX(float percent) {
   static int centerX = width / 2;

   int x = abs((width-cx) - centerX) * percent;
   if(cx < centerX) {
      x = centerX - x;
   }
   else {
      x += centerX;
   }
   return x;
}

int getFarY(float percent) {
   static int centerY = height / 2;

   int y = abs((height-cy) - centerY) * percent;
   if(cy < centerY) {
      y += centerY;
   }
   else {
      y = centerY - y;
   }
   return y;
}

int getNearY(float percent) {
   static int centerY = height / 2;

   int y = abs((height-cy) - centerY) * percent;
   if(cy < centerY) {
      y = centerY - y;
   }
   else {
      y += centerY;
   }
   return y;
}

void rainbow(SDL_Surface* surface, int x, int y, int r, int t) {
   Color black(0, 0, 0, 0);
   Color color(0.35, 0, 0, 0.35);
   FilledEllipse r1(x, y, r, r, black, color);
   r1.setFilter(new SmoothStepFilter(0.8, 1.0));
   r1.draw(surface);

   r -= t;
   color.set(0.35, 0.35, 0, 0.35);
   FilledEllipse r2(x, y, r, r, black, color);
   r2.setFilter(new SmoothStepFilter(0.8, 1.0));
   r2.draw(surface);

   r -= t;
   color.set(0, 0.35, 0, 0.35);
   FilledEllipse r3(x, y, r, r, black, color);
   r3.setFilter(new SmoothStepFilter(0.8, 1.0));
   r3.draw(surface);

   r -= t;
   color.set(0, 0, 0.35, 0.35);
   FilledEllipse r4(x, y, r, r, black, color);
   r4.setFilter(new SmoothStepFilter(0.8, 1.0));
   r4.draw(surface);

   r -= t;
   color.set(0.0, 0.0, 0.0, 0.35);
   FilledEllipse r5(x, y, r, r, black, color);
   r5.setFilter(new SmoothStepFilter(0.8, 1.0));
   r5.draw(surface);
}

void flare(SDL_Surface* surface) {
   // big green
   Color color1(0, 0, 0, 0);
   Color color2(0.20, 0.45, 0, 0.35);
   FilledEllipse circle1(width - cx, height - cy, 60, 60, color1, color2);
   circle1.draw(surface);

   // small green
   color1.set(0.20, 0.45, 0.2, 0.35);
   color2.set(0.20, 0.45, 0.2, 0.35);
   FilledEllipse circle2(getFarX(0.65), getFarY(0.65), 20, 20, color1, color2);
   circle2.draw(surface);

   // big orange
   color1.set(0, 0, 0, 0);
   color2.set(0.47, 0.27, 0, 0.35);
   int x = getFarX(0.45);
   int y = getFarY(0.45);

   FilledEllipse circle3(x, y, 45, 45, color1, color2);
   circle3.draw(surface);

   // small orange
   color1.set(0.47, 0.27, 0, 0.35);
   FilledEllipse circle4(x, y, 10, 10, color1, color2);
   circle4.draw(surface);

   // medium orange
   FilledEllipse circle5(getFarX(0.4), getFarY(0.4), 20, 20, color1, color2);
   circle5.draw(surface);

   // small orange 2
   FilledEllipse circle9(getNearX(0.1), getNearY(0.1), 20, 20, color1, color2);
   circle9.draw(surface);

   // big blue
   color1.set(0.13, 0.22, 0.36, 0.35);
   color2.set(0.13, 0.22, 0.36, 0.35);
   FilledEllipse circle6(getNearX(0.4), getNearY(0.4), 35, 35, color1, color2);
   circle6.draw(surface);

   // small blue 1
   FilledEllipse circle7(getNearX(0.37), getNearY(0.37), 15, 15, color1, color2);
   circle7.draw(surface);

   // small blue 2
   FilledEllipse circle8(getNearX(0.44), getNearY(0.44), 12, 12, color1, color2);
   circle8.draw(surface);

   // center dot
   color1.set(1, 1, 1, 1);
   color2.set(.6, .7, .9, 0);
   FilledEllipse circle10(width/2, height/2, 6, 6, color1, color2);
   circle10.draw(surface);

   // other dot
   FilledEllipse circle11(getFarX(0.3), getFarY(0.3), 7, 7, color1, color2);
   circle11.draw(surface);

   // white
   color1.set(0, 0, 0, 0);
   color2.set(0.59, 0.52, 0.42, 0.35);
   FilledEllipse circle12(getNearX(1.5), getNearY(1.5), 35, 35, color1, color2);
   circle12.draw(surface);

   rainbow(surface, getFarX(1.4), getFarY(1.4), 200, 3);
}

double* CreateGaussianFilter(double piSigma, double piAlpha, int& lSize) {
   lSize = ((int)(piAlpha * piSigma) / 2)*2 + 1; //force odd-size filters

   double* filter = new double[lSize * lSize];

   for (int x = 0; x < lSize ; x++) {
      long FakeX = x - long(floor(lSize / 2.0));
      for (int y = 0 ; y < lSize ; y++) {
         long FakeY = y - long(floor(lSize / 2.0));
         double k = 1.0 / (2.0 * M_PI * piSigma * piSigma);
         filter[y * lSize + x] = k * exp( (-1 * ((FakeX * FakeX) + (FakeY *FakeY))) / (2 * piSigma * piSigma));
      }
   }
   //normalise the filter
   double lTotal=0;
   for (long x = 0; x < lSize; x++) {
      for (long y = 0;y < lSize; y++) {
         lTotal += filter[y * lSize + x];
      }
   }

   for (long x = 0;x < lSize; x++) {
      for (long y = 0;y < lSize; y++) {
         filter[y * lSize + x] /= lTotal;
      }
   }

   return filter;
}

struct RGBA {
   Uint8 r, g, b, a;
};

SDL_Surface* Convolute(SDL_Surface* source, double* filter, int fSize) {
   SDL_Surface* dest = createSurface(width, height);
   const int halfSize = (int)floor(fSize / 2.0);
   const double cf = 1.0 / 255.0;

   RGBA* cs = new RGBA[width * height];

   int idx = 0;
   for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
         Uint32 pixel = getPixel(source, x, y);
         SDL_GetRGBA(pixel, source->format, &cs[idx].r, &cs[idx].g, &cs[idx].b, &cs[idx].a);
         idx++;
      }
   }

   //for each pixel
   for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
         double SumR = 0;
         double SumG = 0;
         double SumB = 0;
         double SumA = 0;

         //For each point of the filter.
         for (int i = 0; i < fSize; i++) {
            //This is to make our origin in the center of the filter
            int FakeI = i - halfSize;

            if(y+FakeI < height && y+FakeI >= 0) {
               for (int j = 0; j < fSize; j++) {
                  int FakeJ = j - halfSize;

                  if (x+FakeJ < width && x+FakeJ >= 0) {
                     int idx = (y + FakeI) * width + (x + FakeJ);
                     SumR += cs[idx].r * filter[i * fSize + j] * cf;
                     SumG += cs[idx].g * filter[i * fSize + j] * cf;
                     SumB += cs[idx].b * filter[i * fSize + j] * cf;
                     SumA += cs[idx].a * filter[i * fSize + j] * cf;
                  }
               }
            }
         }

         Uint32 pixel = SDL_MapRGBA(source->format, SumR * 255, SumG * 255, SumB * 255, SumA * 255);
         setPixel(dest, x, y, pixel);
      }
   }

   delete[] cs;

   return dest;
}

int main(int argc, char **argv) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

   SDL_Surface* surface = createSurface(width, height);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);

   flare(surface);
   star(surface, COUNT, START);

   int fSize;
   double* filter = CreateGaussianFilter(3, 6, fSize);
   SDL_Surface* dest = Convolute(surface, filter, fSize);
   delete[] filter;

   SDL_BlitSurface(dest, NULL, screen, NULL);
   SDL_Flip(screen);
   run();

   SDL_FreeSurface(surface);
   SDL_FreeSurface(dest);

   return 0;
}

