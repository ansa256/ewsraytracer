#include <SDL/SDL.h>
#include "Utility/SDL_Utility.h"
#include "Shapes2D/ThickLine.h"
#include "Shapes2D/Line.h"
#include "Shapes2D/FilledEllipse.h"
#include "Shapes2D/Ring.h"
#include "Shapes2D/Fan.h"
#include "Falloff/SmoothStepFilter.h"
#include "Falloff/CosineFilter.h"
#include <vector>
#include <string>

using namespace std;

const int width = 1024;
const int height = 768;
const int cx = 100;
const int cy = 100;

vector<Shape2D*> shapes;

Color color(0.1, 0.3, 0.8, 0);
//Color color(1, 0.7, 0.1, 0);
Color white(1, 1, 1, 1);

typedef vector<Shape2D*>::const_iterator ShapeIter;
typedef vector<string>::const_reverse_iterator TextIter;

void run();
int randomNoBetween(int x, int y);
void light1();
void light2();
void thickLines();
void drawText(SDL_Surface* screen, int size, vector<string> text, SDL_Color color);

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

   SDL_Quit();
}

void light1() {
   shapes.push_back(new ThickLine(cx, cy, min(cx + 400, width-1) , cy, 35, white, color));
   shapes.push_back(new ThickLine(cx, cy, max(cx - 400, 0), cy, 35, color, white));

   shapes.push_back(new ThickLine(cx, cy, cx, cy + 200, 25, white, color));
   shapes.push_back(new ThickLine(cx, cy, cx, cy - 200, 25, color, white));

   float angle = 100;
   while(angle < 200) {
      int length = randomNoBetween(100, 250);
      shapes.push_back(new Line(cx, cy, length, angle, white, color));
      shapes.push_back(new Line(cx, cy, length, angle + 180, white, color));
      angle += randomNoBetween(1, 8);
   }

   FilledEllipse* ellipse = new FilledEllipse(cx, cy, 100, 85, white, color);
   ellipse->setFilter(new CosineFilter());
   shapes.push_back(ellipse);
   shapes.push_back(new Ring(cx, cy, 200, 220, Color(.3, .6, .8, .2), Color(.3, .6, .8, 0)));
}

void light2() {
   FilledEllipse* circle = new FilledEllipse(cx, cy, 250, 250, white, color);
   circle->setFilter(new SmoothStepFilter(0.05, 1.0));
   shapes.push_back(circle);

   int spread = 15;

   Fan* fan = new Fan(cx, cy, 320, 60, spread, Color(1, 1, 1, 0.3), color);
   fan->setFilter(new SmoothStepFilter(0.1, 1.0));
   shapes.push_back(fan);

   fan = new Fan(cx, cy, 320, 120, spread, Color(1, 1, 1, 0.3), color);
   fan->setFilter(new SmoothStepFilter(0.1, 1.0));
   shapes.push_back(fan);

   fan = new Fan(cx, cy, 320, 240, spread, Color(1, 1, 1, 0.3), color);
   fan->setFilter(new SmoothStepFilter(0.1, 1.0));
   shapes.push_back(fan);

   fan = new Fan(cx, cy, 320, 300, spread, Color(1, 1, 1, 0.3), color);
   fan->setFilter(new SmoothStepFilter(0.1, 1.0));
   shapes.push_back(fan);

   float angle = 100;
   while(angle < 210) {
      int length = randomNoBetween(150, 250);

      fan = new Fan(cx, cy, length, angle , 2, Color(1, 1, 1, 0.3), color);
      fan->setFilter(new SmoothStepFilter(0.5, 1.0));
      shapes.push_back(fan);

      fan = new Fan(cx, cy, length, angle + 180, 2, Color(1, 1, 1, 0.3), color);
      fan->setFilter(new SmoothStepFilter(0.5, 1.0));
      shapes.push_back(fan);
      angle += randomNoBetween(5, 15);
   }

   shapes.push_back(new ThickLine(cx, cy, cx + 600, (int)cy, 45, white, color));
   shapes.push_back(new ThickLine(cx, cy, cx - 600, cy, 45, white, color));

   shapes.push_back(new ThickLine(cx, cy, cx, cy + 400, 40, white, color));
   shapes.push_back(new ThickLine(cx, cy, cx, cy - 400, 40, color, white));
}

void thickLines() {
   int length = 400;

   for(int a = 0; a < 360; a += 45) {
      shapes.push_back(new ThickLine(cx, cy, length, a, 30, white, color));
   }

   FilledEllipse* center = new FilledEllipse(cx, cy, 100, 100, white, color);
   center->setFilter(new SmoothStepFilter(0.1, 1.0));
   shapes.push_back(center);
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

//   light2();
   thickLines();
   for(ShapeIter it = shapes.begin(); it != shapes.end(); ++it) {
      (*it)->draw(surface);
   }

   SDL_BlitSurface(surface, NULL, screen, NULL);

   SDL_Flip(screen);

   run();

   return 0;
}

