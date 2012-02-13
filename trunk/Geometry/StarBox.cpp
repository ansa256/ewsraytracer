#include "StarBox.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/Rectangle.h"
#include "Parser/Hash.h"
#include "Utility/SDL_Utility.h"
#include "Textures/ImageTexture.h"
#include "Storage/Storage.h"

StarBox::StarBox() : count(0), halfSize(1), size(2) {
}

void StarBox::setHash(Hash* h) {
   count = h->getInteger("count");
   halfSize = h->getInteger("halfSize");
   size = halfSize * 2;
}

void StarBox::createStars() {
   srand(0);
   createBoxSide("yzNegX", Vector3D(-halfSize, 0, 0), Vector3D(0, 90, 0));
   createBoxSide("yzPosX", Vector3D(halfSize, 0, 0), Vector3D(0, -90, 0));
   createBoxSide("xyNegZ", Vector3D(0, 0, -halfSize), Vector3D(0, 0, 0));
   createBoxSide("xyPosZ", Vector3D(0, 0, halfSize), Vector3D(0, 180, 0));
   createBoxSide("xzNegY", Vector3D(0, -halfSize, 0), Vector3D(-90, 0, 0));   
   createBoxSide("xzPosY", Vector3D(0, halfSize, 0), Vector3D(90, 0, 0));
}

void StarBox::createBoxSide(string name, const Vector3D& translate, const Vector3D& rotate) {
   SDL_Surface* surface = createSurface(size, size);
   
   Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 0, 255);
   SDL_FillRect(surface, NULL, color);

   for(int i = 0; i < count; i++) {
      float x = (float) rand() / (float) RAND_MAX * (size - 1);
      float y = (float) rand() / (float) RAND_MAX * (size - 1);
      float c = (float) rand() / (float) RAND_MAX;
      setPixel(surface, x, y, Color(c, c, c, 1.0));
   }
   
   ImageTexture* texture = new ImageTexture();
   texture->setSurface(surface);
   
   Emissive* em = new Emissive();
   em->setTexture(texture);
   
   GeometryObject* rect = GeometryManager::createRectangle(translate, size, size, rotate);
   rect->setMaterial(em);
   rect->ignoreShadow = true;
   GeometryManager::instance().getStorage()->addObject(rect);
}
