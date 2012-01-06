#include "TInstance.h"
#include "Parser/Hash.h"

TInstance::TInstance() : texture(NULL), invMatrix(){
}

TInstance::TInstance(Texture* tex) : texture(tex), invMatrix() {
}

TInstance::~TInstance() {
}

void TInstance::setHash(Hash* hash) {
   texture = Texture::createTexture(hash->getValue("texture")->getHash());

   if(hash->contains("translate")) {
      Array* a = hash->getValue("translate")->getArray();
      invMatrix.invTranslate(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
   }
   if(hash->contains("scale")) {
      Array* a = hash->getValue("scale")->getArray();
      invMatrix.invScale(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
   }
   if(hash->contains("rotate")) {
      Array* a = hash->getValue("rotate")->getArray();
      invMatrix.invRotateX(a->at(0)->getDouble());
      invMatrix.invRotateY(a->at(1)->getDouble());
      invMatrix.invRotateZ(a->at(2)->getDouble());
   }
}

Color TInstance::getColor(const ShadeRecord& sr) const {
   ShadeRecord lsr(sr);
   lsr.localHitPoint = invMatrix * sr.localHitPoint;
   return texture->getColor(lsr);
}

float TInstance::getAlpha(const ShadeRecord& sr) const {
   ShadeRecord lsr(sr);
   lsr.localHitPoint = invMatrix * sr.localHitPoint;
   return texture->getAlpha(lsr);
}
