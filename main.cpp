#include <SDL/SDL.h>
#include "Utility/SDL_Utility.h"
#include "Shapes2D/ThickLine.h"
#include "Shapes2D/FilledEllipse.h"
#include "Falloff/FalloffFilter.h"
#include <vector>
#include <string>

using namespace std;

const int width = 960;
const int height = 540;
const int cx = 200;
const int cy = 150;

const int COUNT = 12;
const float START = 10;

vector<Shape2D*> shapes;

//Color color(0.1, 0.3, 0.8, 0);
Color color(1, 0.7, 0.1, 0);
Color white(1, 1, 1, 1);

typedef vector<Shape2D*>::const_iterator ShapeIter;
typedef vector<string>::const_reverse_iterator TextIter;

void run();
void star(int numSpikes, float startAngle);
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

void star(int numSpikes, float startAngle) {
   int length = 430;
   float offset = 360.f / numSpikes;

   for(float a = startAngle; a < 360 + startAngle; a += offset) {
      shapes.push_back(new ThickLine(cx, cy, length, a, 40, white, color));
   }

   FilledEllipse* center = new FilledEllipse(cx, cy, 200, 200, white, color);
   center->setFilter(new SmoothStepFilter(0.05, 1.0));
   shapes.push_back(center);
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

SDL_Surface* Convolute(SDL_Surface* source, double* filter, int fSize) {
   SDL_Surface* dest = createSurface(width, height);
   Uint8 r, g, b, a;
   const int halfSize = (int)floor(fSize / 2.0);
   const double cf = 1.0 / 255.0;

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
                     Uint32 pixel = getPixel(source, x + FakeJ, y + FakeI);
                     SDL_GetRGBA(pixel, source->format, &r, &g, &b, &a);
                     SumR += r * filter[i * fSize + j] * cf;
                     SumG += g * filter[i * fSize + j] * cf;
                     SumB += b * filter[i * fSize + j] * cf;
                     SumA += a * filter[i * fSize + j] * cf;
                  }
               }
            }				
         }

         Uint32 pixel = SDL_MapRGBA(source->format, SumR * 255, SumG * 255, SumB * 255, SumA * 255);
         setPixel(dest, x, y, pixel);
      }
   }
   
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

   star(COUNT, START);
   for(ShapeIter it = shapes.begin(); it != shapes.end(); ++it) {
      (*it)->draw(surface);
   }
   
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

