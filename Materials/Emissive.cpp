#include "Emissive.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"

Emissive::Emissive() : texture(NULL) {
   ls = 1.0;
}

Emissive::~Emissive() {
   if(texture != NULL) {
      delete texture;
   }
}

void Emissive::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
}

Color Emissive::shade(ShadeRecord& sr, const Ray& ray) {
   if((sr.normal * -1.0).dot(ray.direction) > 0.0) {
      return getLe(sr);
   }
   return BLACK;
}

Color Emissive::getLe(const ShadeRecord& sr) const {
   if(texture != NULL) {
      return texture->getColor(sr) * ls;
   }
   return color * ls;
}

void Emissive::setTexture(Texture* tex) {
   texture = tex;
}
