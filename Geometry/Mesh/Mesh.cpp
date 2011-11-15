#include "Mesh.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include "Materials/Matte.h"
#include <assert.h>

typedef vector<Vector3D*>::const_iterator VectorIter;
typedef map<unsigned int, SmoothingGroup*>::const_iterator SmoothingGroupIter;
typedef map<int, Vector3D*>::const_iterator SGNormalIter;

Face::Face(Mesh& mesh, int idx1, int idx2, int idx3) : normal(), dpdu(), dpdv(), smoothGroup(0), parent(mesh) {
   vertIdxs[0] = idx1;
   vertIdxs[1] = idx2;
   vertIdxs[2] = idx3;

   Point3D* p1 = parent.getPointAt(idx1);
   Point3D* p2 = parent.getPointAt(idx2);
   Point3D* p3 = parent.getPointAt(idx3);

   bbox.expand(*p1);
   bbox.expand(*p2);
   bbox.expand(*p3);

   p1p2 = *p2 - *p1;
   p1p3 = *p3 - *p1;
   normal = p1p2.cross(p1p3);
   if(normal.length() == 0.0) {
      normal.set(0, 1, 0);
   }
   else {
      normal.normalize();
   }

   setMaterial(new Matte());
}

bool Face::hit(const Ray& ray, ShadeRecord& sr) const {
   const Vector3D& s1 = ray.direction.cross(p1p3);
   double div = s1.dot(p1p2);
   if(div < epsilon) {
      return false;
   }
   float invDiv = 1.0 / div;

   const Vector3D& s = ray.origin - *parent.getPointAt(vertIdxs[0]);
   const Vector3D& s2 = s.cross(p1p2);
   double b1 = s1.dot(s) * invDiv;
   double b2 = s2.dot(ray.direction) * invDiv;

   if(b1 < epsilon || b2 < epsilon || (b1 + b2) > 1.0) {
      return false;
   }

   double t = s2.dot(p1p3) * invDiv;
   if(t < epsilon || t > ray.tHit) {
      return false;
   }

   ray.tHit = t;

   setNormal(sr, b1, b2);

   sr.localHitPoint = ray(t);
   sr.hitPoint = ray(t);
   sr.material = material;

   if(parent.textureCoords.size() == 0) {
      sr.tu = sr.tv = 0.0;
   } else {
      double b0 = 1.0 - b1 - b2;
      sr.tu = b0 * parent.textureCoords[vertIdxs[0]].x + b1 * parent.textureCoords[vertIdxs[1]].x + b2 * parent.textureCoords[vertIdxs[2]].x;
      sr.tv = b0 * parent.textureCoords[vertIdxs[0]].y + b1 * parent.textureCoords[vertIdxs[1]].y + b2 * parent.textureCoords[vertIdxs[2]].y;
      sr.tu = normalize(sr.tu);
      sr.tv = 1.0 - normalize(sr.tv);
   }

   return true;
}

bool Face::shadowHit(const Ray& ray) const {
   const Vector3D& s1 = ray.direction.cross(p1p3);
   double div = s1.dot(p1p2);
   if(div < epsilon) {
      return false;
   }
   float invDiv = 1.0 / div;

   const Vector3D& s = ray.origin - *parent.getPointAt(vertIdxs[0]);
   const Vector3D& s2 = s.cross(p1p2);
   double b1 = s1.dot(s) * invDiv;
   double b2 = s2.dot(ray.direction) * invDiv;

   if(b1 < epsilon || b2 < epsilon || (b1 + b2) > 1.0) {
      return false;
   }

   double t = s2.dot(p1p3) * invDiv;
   if(t < epsilon) {
      return false;
   }

   ray.tHit = t;
   return true;
}

void Face::setNormal(ShadeRecord& sr, double b1, double b2) const {
   sr.normal = normal;
   sr.dpdu = dpdu;
   sr.dpdv = dpdv;
}

M3DFace::M3DFace(Mesh& mesh, int idx1, int idx2, int idx3) : Face(mesh, idx1, idx2, idx3) {}

void M3DFace::setNormal(ShadeRecord& sr, double b1, double b2) const {
   if(smoothGroup == 0) {
      sr.normal = normal;
      sr.dpdu = dpdu;
      sr.dpdv = dpdv;
   }
   else {
      sr.normal.reset();
      sr.dpdu.reset();
      sr.dpdv.reset();

      unsigned mask = 1;
      for(int i = 0; i < 32; i++) {
         if(mask & smoothGroup) {
            SmoothingGroup* g = parent.smoothingGroups[i]; // .find(i)->second;
            sr.normal += g->interpolateNormal(this, b1, b2);
            sr.dpdu += g->interpolateDPDU(this, b1, b2);
            sr.dpdv += g->interpolateDPDV(this, b1, b2);
         }
         mask = mask << 1;
      }

      sr.normal.normalize();
      sr.dpdu.normalize();
      sr.dpdv.normalize();
   }
}

WavefrontFace::WavefrontFace(Mesh& mesh, int idx1, int idx2, int idx3) : Face(mesh, idx1, idx2, idx3) {
   material->setColor(1, 1, 1);
   material->setDiffuse(0.8);
}

void WavefrontFace::setNormal(ShadeRecord& sr, double b1, double b2) const {
/*   if(parent.getNormalCount() == 0) {
      sr.normal.set(1, 0, 0);
   }
   else {
      sr.normal = *parent.getNormalAt(normalIdxs[0]) * (1.0 - b1 - b2)
                    + *parent.getNormalAt(normalIdxs[1]) * b1
                    + *parent.getNormalAt(normalIdxs[2]) * b2;
      sr.normal.normalize();
   }*/
   sr.normal = normal;
}

void WavefrontFace::setNormalIdxs(int idx1, int idx2, int idx3) {
   normalIdxs[0] = idx1;
   normalIdxs[1] = idx2;
   normalIdxs[2] = idx3;
}

void WavefrontFace::setTextureIdxs(int idx1, int idx2, int idx3) {
   textureIdxs[0] = idx1;
   textureIdxs[1] = idx2;
   textureIdxs[2] = idx3;
}

SmoothingGroup::~SmoothingGroup() {
   for(SGNormalIter it = normals.begin(), end = normals.end(); it != end; it++) {
      delete (*it).second;
   }
   normals.clear();
}

void SmoothingGroup::addFace(Face* f) {
   for(int i = 0; i < 3; i++) {
      if(normals.find(f->vertIdxs[i]) == normals.end()) {
         normals[f->vertIdxs[i]] = new Vector3D();
      }
      *normals[f->vertIdxs[i]] += f->normal;

      if(dpdu.find(f->vertIdxs[i]) == dpdu.end()) {
         dpdu[f->vertIdxs[i]] = new Vector3D();
      }
      *dpdu[f->vertIdxs[i]] += f->dpdu;

      if(dpdv.find(f->vertIdxs[i]) == dpdv.end()) {
         dpdv[f->vertIdxs[i]] = new Vector3D();
      }
      *dpdv[f->vertIdxs[i]] += f->dpdv;
   }
}

void SmoothingGroup::normalize() {
   for(SGNormalIter sit = normals.begin(), end = normals.end(); sit != end; sit++) {
      (*sit).second->normalize();
   }
   for(SGNormalIter sit = dpdu.begin(), end = dpdu.end(); sit != end; sit++) {
      (*sit).second->normalize();
   }
   for(SGNormalIter sit = dpdv.begin(), end = dpdv.end(); sit != end; sit++) {
      (*sit).second->normalize();
   }
}

Vector3D SmoothingGroup::interpolateNormal(const Face* face, const double beta, const double gamma) {
   Vector3D normal(*normals[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *normals[face->vertIdxs[1]] * beta
                 + *normals[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}

Vector3D SmoothingGroup::interpolateDPDU(const Face* face, const double beta, const double gamma) {
   Vector3D normal(*dpdu[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *dpdu[face->vertIdxs[1]] * beta
                 + *dpdu[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}

Vector3D SmoothingGroup::interpolateDPDV(const Face* face, const double beta, const double gamma) {
   Vector3D normal(*dpdv[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *dpdv[face->vertIdxs[1]] * beta
                 + *dpdv[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}


Mesh::Mesh() : Compound(), numCells(0) {
   doDelete = false;
}

Mesh::~Mesh() {
   for(unsigned int i = 0; i < points.size(); i++) {
      delete points[i];
   }
   points.clear();

   for(unsigned int i = 0; i < normals.size(); i++) {
      delete normals[i];
   }
   normals.clear();

   for(unsigned int i = 0; i < faces.size(); i++) {
      delete faces[i];
   }
   faces.clear();
}

int Mesh::addPoint(Point3D* p) {
   points.push_back(p);
   bbox.expand(*p);
   return points.size() - 1;
}

Face* Mesh::addFace(int v1, int v2, int v3, FaceType type) {
   Face *f;
   switch(type) {
      case M3D:
         f = new M3DFace(*this, v1, v2, v3);
         break;
      case WAVEFRONT:
         f = new WavefrontFace(*this, v1, v2, v3);
         break;
      default:
         f = new Face(*this, v1, v2, v3);
         break;
   }
   computePartialDerivitives(f);
   faces.push_back(f);
   return f;
}

void Mesh::calculateNormals() {
   if(!smoothingGroups.empty()) {
      for(SmoothingGroupIter it = smoothingGroups.begin(), end = smoothingGroups.end(); it != end; ++it) {
         (*it).second->normalize();
      }
   }
   else {
      normals.reserve(points.size());
      for(unsigned int i = 0; i < points.size(); i++) {
         normals.push_back(new Vector3D());
      }

      for(FaceIter fi = faces.begin(), end = faces.end(); fi != end; ++fi) {
         *normals[(*fi)->vertIdxs[0]] += (*fi)->normal;
         *normals[(*fi)->vertIdxs[1]] += (*fi)->normal;
         *normals[(*fi)->vertIdxs[2]] += (*fi)->normal;
      }

      for(VectorIter it = normals.begin(), end = normals.end(); it != end; ++it) {
         if((*it)->x == 0.0 && (*it)->y == 0.0 && (*it)->z == 0.0) {
            (*it)->y  = 1.0;
         }
         (*it)->normalize();
      }
   }
}

void Mesh::setHash(Hash* hash) {
   if(hash->contains("material")) {
      setupMaterial(hash->getValue("material")->getHash());
   }

   if(hash->contains("verticies")) {
      Array* verts = hash->getValue("verticies")->getArray();
      for(unsigned int i = 0; i < verts->size(); i++) {
         Array* vert = verts->at(i)->getArray();
         addPoint(new Point3D(vert->at(0)->getDouble(), vert->at(1)->getDouble(), vert->at(2)->getDouble()));
      }
   }

   if(hash->contains("faces")) {
      Array* faces = hash->getValue("faces")->getArray();
      for(unsigned int i = 0; i < faces->size(); i++) {
         Array* f = faces->at(i)->getArray();
         addFace(f->at(0)->getInteger(), f->at(1)->getInteger(), f->at(2)->getInteger());
      }

      calculateNormals();
   }
}

void Mesh::computePartialDerivitives(Face* face) const {
   double uvs[3][2];
   getUVs(uvs, face);

   Vector3D dp1 = *points[face->vertIdxs[0]] - *points[face->vertIdxs[2]];
   Vector3D dp2 = *points[face->vertIdxs[1]] - *points[face->vertIdxs[2]];

   // Compute deltas
   double du1 = uvs[0][0] - uvs[2][0];
   double du2 = uvs[1][0] - uvs[2][0];
   double dv1 = uvs[0][1] - uvs[2][1];
   double dv2 = uvs[1][1] - uvs[2][1];

   double determinant = du1 * dv2 - dv1 * du2;
   if(determinant == 0.0) {
      coordinateSystem(face->p1p3.cross(face->p1p2).normalize(), &face->dpdu, &face->dpdv);
   }
   else {
      face->dpdu = (dp1 * dv2 - dp2 * dv1) / determinant;
      face->dpdu.normalize();
      face->dpdv = (dp1 * -du2 + dp2 * du1) / determinant;
      face->dpdv.normalize();
   }
}

void Mesh::getUVs(double uv[3][2], Face* face) const {
   if(textureCoords.empty()) {
      uv[0][0] = 0.0;
      uv[0][1] = 0.0;
      uv[1][0] = 1.0;
      uv[1][1] = 0.0;
      uv[2][0] = 1.0;
      uv[2][1] = 1.0;
   }
   else {
      for(int i = 0; i < 3; i++) {
         unsigned int idx = face->vertIdxs[i];
         assert(idx < textureCoords.size());
         uv[i][0] = textureCoords[idx].x;
         uv[i][1] = textureCoords[idx].y;
      }
   }
}
