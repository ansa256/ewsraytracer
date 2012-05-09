#include "Material.h"
#include "Textures/ImageTexture.h"
#include "Math/Matrix.h"
#include "Parser/Hash.h"
#include "Matte.h"
#include "Phong.h"
#include "Reflective.h"
#include "Atmosphere.h"
#include "Emissive.h"
#include "BRDFs/Lambertian.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"

Material::Material() : bsdf(), ambientBRDF(new Lambertian()), normalMap(NULL) {
}

Material::~Material() {
   if(normalMap != NULL) {
      delete normalMap;
   }
}

Material* Material::createMaterial(Hash* hash) {
   string type = hash->getString("type");

   Material* material;
   if(type == "phong") {
      material = new Phong();
   }
   else if(type == "reflective") {
      material = new Reflective();
   }
   else if(type == "atmosphere") {
      material = new Atmosphere();
   }
   else if(type == "emissive") {
      material = new Emissive();
   }
   else {
      // Matte is the default type
      material = new Matte();
   }

   material->setHash(hash);
   if(hash->contains("normalMap")) {
      material->normalMap = Texture::createTexture(hash->getValue("normalMap")->getHash());
   }
   
   if(hash->contains("normalMapFile")) {
      ImageTexture* tex = new ImageTexture();
      tex->setTextureFile(hash->getString("normalMapFile"));
      material->normalMap = tex;
   }

   return material;
}

Color Material::shade(ShadeRecord& sr, const Ray& ray) {
   if(normalMap != NULL) applyNormalMap(sr);
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);

   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Color Ld;
      float* samples = (*it)->getSamples();
      for(int s = 0; s < (*it)->getNumLightSamples(); s++) {
         Ld += estimateDirect(sr, *it, wo, samples, s);
      }
      L += Ld / (*it)->getNumLightSamples();
   }
//   L.alpha = diffuseBRDF->getAlpha(sr) + ambientBRDF->getAlpha(sr);
   return L;
}

Color Material::estimateDirect(ShadeRecord& sr, Light* light, const Vector3D& wo, float* samples, int s) {
   Color Ld;
   Vector3D wi;
   float pdf;
   
   Color Li = light->Sample_L(sr, samples[s * 2], samples[s * 2 + 1], wi, pdf);
   if(pdf > 0 && !Li.isBlack()) {
      Color f = bsdf.f(sr, wo, wi);
      if(!f.isBlack()) {
         float ndotwi = sr.normal.dot(wi);
         if(ndotwi > 0) {
            Ray shadowRay(sr.hitPoint, wi);
            bool inShadow = light->inShadow(shadowRay, sr);
            if(!inShadow) {
               Ld += f * Li * ndotwi/ pdf;
            }
         }
      }
   }
   return Ld;
}

void Material::applyNormalMap(ShadeRecord& sr) {
   Vector3D tangent(sr.normal.z, 0, -sr.normal.x);
   Vector3D binormal(sr.normal.cross(tangent));

   if(sr.normal.x == 0 && sr.normal.y == 1 && sr.normal.z == 0) {
      tangent.set(1, 0, 0);
      binormal.set(0, 0, -1);
   }
   else if(sr.normal.x == 0 && sr.normal.y == -1 && sr.normal.z == 0) {
      tangent.set(-1, 0, 0);
      binormal.set(0, 0, 1);
   }

   Matrix tangentMatrix(tangent, binormal, sr.normal);
   tangentMatrix.invert();

   Color color = normalMap->getColor(sr);
   Vector3D mapNormal(2.0 * color.red - 1.0, 2.0 * color.green - 1.0, 2.0 * color.blue - 1.0);
   sr.normal = tangentMatrix * mapNormal;
}

float Material::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return bsdf.getAlpha(sr);
}
