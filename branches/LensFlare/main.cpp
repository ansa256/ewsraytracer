#include <SDL/SDL.h>
#include <math.h>
#include "Shapes/Arc.h"
#include "Shapes/Circle.h"
#include "Shapes/Line.h"
#include "Film.h"
#include <vector>

using namespace std;

vector<Shape2D*> shapes;
typedef vector<Shape2D*>::const_iterator ShapeIter;

const int size = 500;

SDL_Surface* createSurface(int w, int h) {
   SDL_Surface *surface;
   Uint32 rmask, gmask, bmask, amask;
   
   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
    on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif
   
   surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
   if(surface == NULL) {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      exit(1);
   }
   
   return surface;
}

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
}

void drawShapes(SDL_Surface* surf) {
   Color black(0, 0, 0, 1), pColor(0, 0, 0, 0);
   
   Film film(size, size);
   float step = 0.5;

   for(float x = 0; x < size; x += step) {
      for(float y = 0; y < size; y += step) {
         bool hit = false;
         for(ShapeIter it = shapes.begin(); it != shapes.end(); ++it) {
            if((*it)->hit(x, y, pColor)) {
               film.addSample(x, y, pColor);
               hit = true;
            }
         }
         if(!hit) {
            film.addSample(x, y, black);
         }
      }
   }
   
   film.generateImage(surf);
   printf("done\n");
}

int main(int argc, char **argv) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }
      
   SDL_Surface* screen = SDL_SetVideoMode(size, size, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
   
   SDL_Surface* surface = createSurface(size, size);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);
   
   //shapes.push_back(new Circle(250, 250, 200));
   //shapes.push_back(new Line(Point2D(10, 10), 100, 0));
   //shapes.push_back(new Line(Point2D(0, 0), 100, 45));
   //shapes.push_back(new Line(Point2D(0, 0), 100, 60));
   shapes.push_back(new Arc(250, 250, 200, 0, 5));
   drawShapes(surface);

   SDL_BlitSurface(surface, NULL, screen, NULL);   
   SDL_Flip(screen);

   run();
   
   return 0;
}
