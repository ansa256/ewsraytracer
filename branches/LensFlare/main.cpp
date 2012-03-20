#include <SDL/SDL.h>
#include <math.h>
#include "Shapes/Circle.h"
#include "Shapes/Line.h"
#include "Film.h"
#include <vector>

using namespace std;

vector<Shape2D*> shapes;
typedef vector<Shape2D*>::const_iterator ShapeIter;

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
   Color color(0, 0, 0, 0), pColor(0, 0, 0, 0);
//   int div = 0;
   Film film(500, 500);

   for(int x = 0; x < 500; x++) {
      for(int y = 0; y < 500; y++) {
         for(float sx = x; sx < x+1; sx += 0.1) {
            for(float sy = y; sy < y+1; sy += 0.1) {
               for(ShapeIter it = shapes.begin(); it != shapes.end(); ++it) {
                  if((*it)->hit(sx, sy, pColor)) {
                     film.addSample(sx, sy, pColor);
//                     color += pColor;
//                     div++;
                  }
               }
            }
         }
         
//         color = color / div;
//         Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * surf->format->BytesPerPixel;
//         *(Uint32*)p = SDL_MapRGBA(surf->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
//         color.set(0, 0, 0, 0);
//         div = 0;
      }
   }
   
   film.generateImage(surf);
}

int main(int argc, char **argv) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }
      
   SDL_Surface* screen = SDL_SetVideoMode(500, 500, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
   
   SDL_Surface* surface = createSurface(500, 500);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);
   
   shapes.push_back(new Circle(250, 250, 200));
//   shapes.push_back(new Circle(0, 0, 100));
   shapes.push_back(new Line(Point2D(0, 0), 100, 30));
   shapes.push_back(new Line(Point2D(0, 0), 100, 45));
   shapes.push_back(new Line(Point2D(0, 0), 100, 60));
   drawShapes(surface);

   SDL_BlitSurface(surface, NULL, screen, NULL);   
   SDL_Flip(screen);

   run();
   
   return 0;
}
