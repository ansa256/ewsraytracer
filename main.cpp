#include <SDL/SDL.h>
#include <SDL_ttf/SDL_ttf.h>
#include "Utility/SDL_Utility.h"
#include <vector>
#include <string>

using namespace std;

const int width = 960;
const int height = 540;

const int inFrameOne = 15;
const int inFrameTwo = 100;
const int fadeFrames = 30;
const int outFrame = 180;
const int lastFrame = outFrame + fadeFrames + inFrameOne;

typedef vector<string>::const_iterator TextIter;

void run();
void drawText(SDL_Surface* screen, int size, vector<string> text, SDL_Color color, SDL_Color color2, bool draw2 = false);

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

void drawText(SDL_Surface* screen, int size, vector<string> text, SDL_Color color, SDL_Color color2, bool draw2) {
   TTF_Font *font;
   font = TTF_OpenFont("/Library/Fonts/Bank Gothic Medium BT.ttf", size);
//   font = TTF_OpenFont("/home/saariew1/.fonts/bankgthd.ttf", size);
   if(!font) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      return ;
   }
   
   vector<SDL_Surface*> surfaces;
   int width = 0;
   int height = 0;

   for(TextIter it = text.begin(); it != text.end(); ++it) {
      SDL_Surface* surf = TTF_RenderText_Blended(font, (*it).c_str(), color);
      surfaces.push_back(surf);
      width = max(width, surf->w);
      height = surf->h;
   }

   SDL_Rect dest;
   dest.x = (screen->w - width) / 2;
   dest.y = (screen->h - ((text.size() + 2) * height)) / 2;

   for(vector<SDL_Surface*>::const_iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
      if(SDL_BlitSurface((*it), NULL, screen, &dest) == -1) {
         printf("Error during blit: %s\n", SDL_GetError());
      }
         
      dest.y += (*it)->h;
      SDL_FreeSurface(*it);
   }

   TTF_CloseFont(font);

   if(draw2) {
      font = TTF_OpenFont("/Library/Fonts/Bank Gothic Medium BT.ttf", size * 0.80);
   
      SDL_Surface* surf = TTF_RenderText_Blended(font, "- STANLEY KUBRICK", color2);
      dest.x = dest.x + width - surf->w;
      dest.y += height;
      SDL_BlitSurface(surf, NULL, screen, &dest);
      SDL_FreeSurface(surf);
   
      TTF_CloseFont(font);
   }
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

   SDL_Surface* screen = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

   SDL_Surface* surface = createSurface(width, height);
   Uint32 black = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
   SDL_FillRect(surface, NULL, black);
   
//   You're free to speculate as you wish about the
//   philosophical and allegorical meaning of the film (2001) ...

   SDL_BlitSurface(surface, NULL, screen, NULL);
   SDL_UpdateRect(screen, 0, 0, 0, 0);
   
   char fname[512];
   string outputDir = "/Users/esaari1/workspace/raytracer/output/title";
   unsigned long frame = 0;
   
   while(frame < inFrameOne) {
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);
      frame++;
   }

   vector<string> text;
   text.push_back("YOU'RE FREE TO SPECULATE AS YOU");
   text.push_back("WISH ABOUT THE PHILISOPHICAL AND");
   text.push_back("ALLEGORICAL MEANING OF THE FILM");
   
   float denom = 1.f / fadeFrames;
   while(frame < (inFrameOne + fadeFrames)) {
      float p = (frame - inFrameOne) * denom;
      int c = (int)(p * 255.f);
      SDL_Color color = {c, c, c};

      SDL_FillRect(screen, NULL, black);
      drawText(screen, 30, text, color, color, false);
      SDL_UpdateRect(screen, 0, 0, 0, 0);

      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);

      frame++;
   }
   
   while(frame < inFrameTwo) {
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);
      frame++;
   }
   
   SDL_Color white = {255, 255, 255};
   while(frame < (inFrameTwo + fadeFrames)) {
      float p = (frame - inFrameTwo) * denom;
      int c = (int)(p * 255.f);
      SDL_Color color = {c, c, c};
      
      SDL_FillRect(screen, NULL, black);
      drawText(screen, 30, text, white, color, true);
      SDL_UpdateRect(screen, 0, 0, 0, 0);
      
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);
      
      frame++;
   }
   
   while(frame < outFrame) {
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);
      frame++;
   }
   
   while(frame < (outFrame + fadeFrames)) {
      float p = 1.f - (frame - outFrame) * denom;
      int c = (int)(p * 255.f);
      SDL_Color color = {c, c, c};
      
      SDL_FillRect(screen, NULL, black);
      drawText(screen, 30, text, color, color, true);
      SDL_UpdateRect(screen, 0, 0, 0, 0);
      
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);
      
      frame++;
   }
   
   while(frame < lastFrame) {
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), frame);
      saveImage(screen, fname);
      frame++;
   }
   
   printf("Done\n");

   run();

   return 0;
}

