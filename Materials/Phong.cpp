#include "Phong.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/GlossySpecular.h"
#include "Textures/Texture.h"
#include "Parser/Hash.h"
#include "Textures/ImageTexture.h"

Phong::Phong() : ambientBRDF(new Lambertian()), diffuseBRDF(new Lambertian()), specularBRDF(new GlossySpecular()) {
}

Phong::~Phong() {
   delete ambientBRDF;
   delete diffuseBRDF;
   delete specularBRDF;
}

void Phong::setHash(Hash* hash) {
   kd = hash->getDouble("kd", 0.9);

   if(hash->contains("texture")) {
      ambientBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      diffuseBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
   }
   else if(hash->contains("color")) {
      Array* a = hash->getValue("color")->getArray();
      ambientBRDF->setColor(Color(a));
      diffuseBRDF->setColor(Color(a));
   }

   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }

   float ks = hash->getDouble("ks", 0.1);
   specularBRDF->setColor(ks, ks, ks);
   specularBRDF->setExp(hash->getDouble("exp"));

   if(hash->contains("specularMask")) {
      specularBRDF->setSpecularMask(Texture::createTexture(hash->getValue("specularMask")->getHash()));
   }
}

Color Phong::shade(ShadeRecord& sr, const Ray& ray) {
   applyNormalMap(sr);
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);

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
      L += (diffuseBRDF->f(sr, wo, wis) + specularBRDF->f(sr, wo, wis)) * power;
   }

   return L;
}

void Phong::setColor(float r, float g, float b) {
   Color c(r, g, b);
   ambientBRDF->setColor(c);
   diffuseBRDF->setColor(c);
   specularBRDF->setColor(c);
}

float Phong::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return diffuseBRDF->getAlpha(sr);
}

void Phong::setTexture(string texture) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texture);
   ambientBRDF->setTexture(tex);

   tex = new ImageTexture();
   tex->setTextureFile(texture);
   diffuseBRDF->setTexture(tex);
}

void Phong::setAmbientColor(const Color& c) {
   ambientBRDF->setColor(c);
}

void Phong::setDiffuseColor(const Color& c) {
   diffuseBRDF->setColor(c);
}

void Phong::setSpecularColor(const Color& c) {
   specularBRDF->setColor(c);
}

void Phong::setSpecularHighlight(float sh) {
   specularBRDF->setExp(sh);
}
