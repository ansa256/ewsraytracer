#include <SDL/SDL.h>
#include <SDL_ttf/SDL_ttf.h>
#include "Utility/SDL_Utility.h"
#include <vector>
#include <string>

using namespace std;

const int width = 1024;
const int height = 768;

typedef vector<string>::const_reverse_iterator TextIter;

void run();
void drawText(SDL_Surface* screen, int size, vector<string> text, SDL_Color color);

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
   
   TTF_Quit();
   SDL_Quit();
}

void drawText(SDL_Surface* screen, int size, vector<string> text, SDL_Color color) {
   TTF_Font *font;
//   font = TTF_OpenFont("/Library/Fonts/Bank Gothic Medium BT.ttf", size);
   font = TTF_OpenFont("/home/saariew1/.fonts/bankgthd.ttf", size);
   if(!font) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      return ;
   }

   SDL_Rect dest;
   dest.y = height;

   for(TextIter it = text.rbegin(); it != text.rend(); ++it) {
      SDL_Surface *text_surface;
      if(!(text_surface = TTF_RenderText_Blended(font, (*it).c_str(), color))) {
         printf("TTF_RenderText: %s\n", TTF_GetError());
      } 
      else {
         setAlpha(text_surface, 255);
         dest.x = width - text_surface->w;
         dest.y -= text_surface->h;

         if(SDL_BlitSurface(text_surface, NULL, screen, &dest) == -1) {
            printf("Error during blit: %s\n", SDL_GetError());
         }
         
         SDL_FreeSurface(text_surface);
      }
   }
   
   TTF_CloseFont(font);
}

int main(int argc, char **argv) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   if(TTF_Init()==-1) {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
   }

   srand(0);

   SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

   SDL_Surface* surface = createSurface(width, height);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);
   
//   You're free to speculate as you wish about the philosophical and allegorical meaning of the film (2001) ...

   SDL_BlitSurface(surface, NULL, screen, NULL);

   vector<string> text;
   text.push_back("Eric Saari ");
//   text.push_back("Presents ");

   SDL_Color color = {255, 255, 255};
   drawText(screen, 50, text, color);
   
   SDL_Flip(screen);

   run();

   return 0;
}

