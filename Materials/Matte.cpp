#include "Matte.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "BRDFs/Lambertian.h"
#include "Textures/Texture.h"
#include "Textures/ImageTexture.h"
#include "Parser/Hash.h"
#include "Lights/Light.h"

Matte::Matte() : ambientBRDF(new Lambertian()), diffuseBRDF(new Lambertian()), kd(0.9) {
}

Matte::~Matte() {
   delete ambientBRDF;
   delete diffuseBRDF;
}

void Matte::setHash(Hash* hash) {
   kd = hash->getDouble("kd", 0.9);

   if(hash->contains("texture")) {
      ambientBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      diffuseBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
   }
   if(hash->contains("color")) {
      Array* a = hash->getValue("color")->getArray();
      ambientBRDF->setColor(Color(a));
      diffuseBRDF->setColor(Color(a));
   }

   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }
}

Color Matte::shade(ShadeRecord& sr, const Ray& ray) {
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr) * (1.f - kd);

   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Color power;
      Vector3D wis;

      for(int s = 0; s < (*it)->getNumLightSamples(); s++) {
         Vector3D wi = (*it)->getLightDirection(sr);
         wis += wi;
         float ndotwi = sr.normal.dot(wi);

         if(ndotwi > 0.0) {
            Ray shadowRay(sr.hitPoint, wi);
            bool inShadow = (*it)->inShadow(shadowRay, sr);

            if(!inShadow) {
               power += (*it)->L(sr) * (*it)->G(sr) * ndotwi / (*it)->pdf(sr);
            }
         }
      }

      power = power / (*it)->getNumLightSamples();
      wis.normalize();
      L += diffuseBRDF->f(sr, wo, wis) * power * kd;
   }

   L.alpha = diffuseBRDF->getAlpha(sr);
   return L;
}

float Matte::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return diffuseBRDF->getAlpha(sr);
}

void Matte::setColor(float r, float g, float b) {
   ambientBRDF->setColor(Color(r, g, b));
   diffuseBRDF->setColor(Color(r, g, b));
}

void Matte::setColor(const Color& c) {
   ambientBRDF->setColor(c);
   diffuseBRDF->setColor(c);
}

void Matte::setAmbientColor(const Color& c) {
   ambientBRDF->setColor(c);
}

void Matte::setTexture(string texture) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texture);
   ambientBRDF->setTexture(tex);

   tex = new ImageTexture();
   tex->setTextureFile(texture);
   diffuseBRDF->setTexture(tex);
}
