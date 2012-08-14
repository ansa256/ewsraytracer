#include "OverlayManager.h"
#include "Parser/Parser.h"
#include "Sun.h"
#include "LensFlare.h"
#include <fstream>

auto_ptr<OverlayManager> OverlayManager::s_instance;

OverlayManager& OverlayManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new OverlayManager());
   }
   return *s_instance;
}

OverlayManager::OverlayManager() {

}

OverlayManager::~OverlayManager() {
   for(unsigned int i = 0; i < overlays.size(); i++) {
      delete overlays[i];
   }
   overlays.clear();
}

void OverlayManager::loadOverlays(string fname) {
   std::ifstream fp(fname.c_str());
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      string type = tok.getStringValue();
      Hash* hash = parser.readValue()->getHash();

      if(type == "sun") {
         Sun* sun = new Sun();
         sun->setHash(hash);
         overlays.push_back(sun);
      }
      else if(type == "lensFlare") {
         LensFlare* flare = new LensFlare();
         flare->setHash(hash);
         overlays.push_back(flare);
      }
   }

   fp.close();
}
