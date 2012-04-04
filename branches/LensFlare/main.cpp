#include <SDL/SDL.h>
#include "Utility/SDL_Utility.h"
#include "Shapes2D/ThickLine.h"
#include "Shapes2D/FilledCircle.h"
#include "Shapes2D/FilledEllipse.h"
#include "Shapes2D/Ring.h"
#include <vector>

using namespace std;

const int width = 960;
const int height = 540;
const int cx = 480;
const int cy = 270;

typedef vector<Shape2D*>::const_iterator ShapeIter;

void run();
int randomNoBetween(int x, int y);

int randomNoBetween(int x, int y) {
   return (rand() % (y - x + 1)) + x;
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

int main(int argc, char **argv) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }
   
   srand(0);

   SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

   SDL_Surface* surface = createSurface(width, height);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);

   vector<Shape2D*> shapes;

   Color blue(.1, .3, .8, 0);
   Color white(1, 1, 1, 1);

   shapes.push_back(new ThickLine(cx, cy, min(cx + 400, width-1) , cy, 35, white, blue));
   shapes.push_back(new ThickLine(cx, cy, max(cx - 400, 0), cy, 35, blue, white));

   shapes.push_back(new ThickLine(cx, cy, cx, cy + 200, 25, white, blue));
   shapes.push_back(new ThickLine(cx, cy, cx, cy - 200, 25, blue, white));
   
   float angle = 100;
   while(angle < 200) {
      int length = randomNoBetween(100, 250);
      shapes.push_back(new Line(cx, cy, length, angle, height, blue, white));
      shapes.push_back(new Line(cx, cy, length, angle + 180, height, blue, white));      
      angle += randomNoBetween(1, 8);
   }

   shapes.push_back(new FilledEllipse(cx, cy, 100, 85, white, blue));
   shapes.push_back(new Ring(cx, cy, 200, 220, Color(.3, .6, .8, .2), Color(.3, .6, .8, 0)));

   for(ShapeIter it = shapes.begin(); it != shapes.end(); ++it) {
      (*it)->draw(surface);
   }

   SDL_BlitSurface(surface, NULL, screen, NULL);
   SDL_Flip(screen);

   run();

   return 0;
}

