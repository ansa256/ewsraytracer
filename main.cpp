#include <SDL/SDL.h>
#include "Utility/SDL_Utility.h"
#include "Shapes2D/ThickLine.h"
#include "Shapes2D/FilledCircle.h"
#include <vector>

using namespace std;

typedef vector<Shape2D*>::const_iterator ShapeIter;

void run();

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

int main(int argc, char **argv) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   SDL_Surface* screen = SDL_SetVideoMode(500, 500, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

   SDL_Surface* surface = createSurface(500, 500);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);

   vector<Shape2D*> shapes;

   Color blue(.1, .3, .8, 1);
   Color white(1, 1, 1, 1);

//   shapes.push_back(new FilledCircle(250, 250, 150, white, Color(.1, .3, .8, 0)));
   shapes.push_back(new ThickLine(250, 250, 499, 250, 100, white, blue));
   shapes.push_back(new ThickLine(249, 250, 0, 250, 100, blue, white));

   shapes.push_back(new ThickLine(250, 250, 250, 499, 100, white, blue));
   shapes.push_back(new ThickLine(250, 249, 250, 0, 100, blue, white));

   for(ShapeIter it = shapes.begin(); it != shapes.end(); ++it) {
      (*it)->draw(surface);
   }

   SDL_BlitSurface(surface, NULL, screen, NULL);
   SDL_Flip(screen);

   run();

   return 0;
}
