#include "MarbleChecker.h"
#include "Parser/Hash.h"

MarbleChecker::MarbleChecker() : squares(NULL), size(1) {
}

MarbleChecker::~MarbleChecker() {
   delete[] squares;
}

Color MarbleChecker::getColor(const ShadeRecord& sr) const {
   float x = sr.localHitPoint.x;
   float z = sr.localHitPoint.y;
   int ix = (int) floor(x / size) + 4;
   int iz = (int) floor(z / size) + 4;
   
   int count = 8;
   return squares[ix * count + iz].getColor(sr);
}

void MarbleChecker::setHash(Hash* hash) {
   Color color1 = Color(hash->getValue("color1")->getArray());
   Color color2 = Color(hash->getValue("color2")->getArray());
   Color vein1 = Color(hash->getValue("vein1")->getArray());
   Color vein2 = Color(hash->getValue("vein2")->getArray());
   size = hash->getDouble("size");
   
   int count = (int)(2 / size);
   int seed = 0;
   squares = new MarbleTexture[count * count];
   
   for(int x = 0; x < count; x++) {
      for(int y = 0; y < count; y++) {
         int idx = x * count + y;
         Hash* h;
         if((x+y) % 2 == 0) {
            h = createHash(color1, vein1, seed++);
         }
         else {
            h = createHash(color2, vein2, seed++);
         }
         squares[idx].setHash(h);
      }
   }
}

Hash* MarbleChecker::createHash(const Color& c, const Color& v, int seed) {
   Hash* h = new Hash();
   Value* val = new Value(seed);
   h->addValue("seed", *val);
   
   Value* val2 = new Value(c.toArray());
   h->addValue("mainColor", *val2);
   
   Value* val3 = new Value(v.toArray());
   h->addValue("veinColor", *val3);
   
   Value* val4 = new Value(8);
   h->addValue("numOctaves", *val4);

   Value* val5 = new Value(2.0);
   h->addValue("gain", *val5);

   Value* val6 = new Value(2.0);
   h->addValue("lacunarity", *val6);

   return h;
}
