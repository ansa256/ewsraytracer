#include "MarbleTexture.h"
#include "Noise/CubicNoise.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"

MarbleTexture::MarbleTexture() : Texture(), mainColor(1.0, 1.0, 1.0), veinColor(0.1, 0.1, 0.1), noise(NULL) {
}

MarbleTexture::~MarbleTexture() {
   delete noise;
}

Color MarbleTexture::getColor(const ShadeRecord& sr) const {
   float value = noise->fbm(sr.localHitPoint);
   float y = sr.localHitPoint.y + 40.0 * value;
   float u = (1.0 + sin(y)) * 0.5;
   
   static float threshold = 0.995;

   if(u >= threshold) {
      return lerp((u - threshold) / (1.0 - threshold), veinColor, lineColor);
   }
   return lerp(u, mainColor, veinColor);
}

void MarbleTexture::setHash(Hash* hash) {
   noise = new CubicNoise();
   noise->setHash(hash);
   mainColor.set(hash->getValue("mainColor")->getArray());
   veinColor.set(hash->getValue("veinColor")->getArray());
   lineColor = veinColor * 0.50;
}
