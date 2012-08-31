#include "StarBox.h"
#include "Geometry/GeometryManager.h"
#include "Parser/Hash.h"
#include "Textures/ImageTexture.h"
#include "Storage/Storage.h"

StarBox::StarBox() : halfSize(1) {
}

void StarBox::setHash(Hash* h) {
   halfSize = h->getInteger("halfSize");
   yzNegX = h->getString("yzNegX");
   yzPosX = h->getString("yzPosX");
   xyNegZ = h->getString("xyNegZ");
   xyPosZ = h->getString("xyPosZ");
   xzNegY = h->getString("xzNegY");
   xzPosY = h->getString("xzPosY");
}

void StarBox::createStars() {
   if(yzNegX != "") createBoxSide(yzNegX, Vector3D(-halfSize, 0, 0), Vector3D(0, 90, 0));
   if(yzPosX != "") createBoxSide(yzPosX, Vector3D(halfSize, 0, 0), Vector3D(0, -90, 0));
   if(xyNegZ != "") createBoxSide(xyNegZ, Vector3D(0, 0, -halfSize), Vector3D(0, 0, 0));
   if(xyPosZ != "") createBoxSide(xyPosZ, Vector3D(0, 0, halfSize), Vector3D(0, 180, 0));
   if(xzNegY != "") createBoxSide(xzNegY, Vector3D(0, -halfSize, 0), Vector3D(-90, 0, 0));   
   if(xzPosY != "") createBoxSide(xzPosY, Vector3D(0, halfSize, 0), Vector3D(90, 0, 0));
}

void StarBox::createBoxSide(string name, const Vector3D& translate, const Vector3D& rotate) {
   ImageTexture* texture = new ImageTexture();
   texture->setTextureFile(name);
   
   Emissive* em = new Emissive();
   em->setTexture(texture);
   
   GeometryObject* rect = GeometryManager::createRectangle(translate, halfSize, halfSize, rotate);
   rect->setMaterial(em);
   rect->ignoreShadow = true;
   GeometryManager::instance().getStorage()->addObject(rect);
}
