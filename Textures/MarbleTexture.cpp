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
   Point3D p = sr.localHitPoint;
   float value = noise->fbm(p);
   float y = p.y + 60 * value;
   float t = (1.0 + sin(y)) * 0.5;

   static float c[][3] = { { 0.9, 0.89, 0.847 }, { 0.9, 0.89, 0.847 }, { 0.9, 0.89, 0.847 }, { 0.9, 0.89, 0.847 },
//      { .5f, .5f, .5f }, //{ .6f, .59f, .58f }, { .58f, .58f, .6f },
/*      { .58f, .58f, .58f }, {.2f, .2f, .2f },*/ { .21f, .2f, .19f }, };
#define NC  sizeof(c) / sizeof(c[0])
#define NSEG (NC-3)
   int first = int(t * NSEG);
   t = (t * NSEG - first);

   Color c0(c[first]);
   Color c1(c[first + 1]);
   Color c2(c[first + 2]);
   Color c3(c[first + 3]);
   
   Color s0 = (1.f - t) * c0 + t * c1;
   Color s1 = (1.f - t) * c1 + t * c2;
   Color s2 = (1.f - t) * c2 + t * c3;
   
   s0 = (1.f - t) * s0 + t * s1;
   s1 = (1.f - t) * s1 + t * s2;
   
   return 1.5f * ((1.f - t) * s0 + t * s1);

//   static float threshold = 0.999;

//   if(t >= threshold) {
//      return lerp((t - threshold) / (1.0 - threshold), veinColor, lineColor);
//   }
//   return lerp(t, mainColor, veinColor);

//   Color s0 = (1.f - t) * mainColor + t * veinColor;
//   Color s1 = (1.f - t) * veinColor + t * lineColor;
//   return 1.5f * ((1.f - t) * s0 + t * s1);
}

void MarbleTexture::setHash(Hash* hash) {
   noise = new CubicNoise();
   noise->setHash(hash);
   mainColor.set(hash->getValue("mainColor")->getArray());
   veinColor.set(hash->getValue("veinColor")->getArray());
   lineColor = veinColor * 0.50;
}
