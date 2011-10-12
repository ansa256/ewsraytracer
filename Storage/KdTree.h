#ifndef KDTREE_H
#define KDTREE_H

#include "Storage.h"
#include <boost/dynamic_bitset.hpp>

typedef unsigned int uint32_t;

struct KdNode {
   void initLeaf(uint32_t* _idxs, int np);
   void initInterior(uint32_t axis, uint32_t ac, float s);

   float getSplit() const { return split; }
   uint32_t nPrimitives() const { return nPrims >> 2; }
   uint32_t getAxis() const { return flags & 3; }
   bool isLeaf() const { return (flags & 3) == 3; }
   uint32_t getAboveChild() const { return aboveChild >> 2; }

   union {
      float split;      // interior
      uint32_t* idxs;   // leaf
   };

private:
   union {
      uint32_t flags;
      uint32_t nPrims;
      uint32_t aboveChild;
   };
};

struct BoundEdge {
   void set(float t, bool s) { tsplit = t; type = s ? START : END; }
   bool operator< (const BoundEdge& e) const;

   float tsplit;
   enum { START, END } type;
};

class KdTree : public Storage {

public:
   KdTree();
   virtual ~KdTree();

   virtual void setup();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

private:
   void buildTree(int nodeNum, unsigned depth, uint32_t* idxs, int nPrimitives, const BBox& bounds, uint32_t* lidxs, uint32_t* ridxs);
   bool checkNode(const Ray& ray, int node, double& tmin, ShadeRecord& sr, boost::dynamic_bitset<>& checked) const;
   bool checkNodeShadow(const Ray& ray, int node, double& tmin) const;
   void findSplit(uint32_t* idxs, int np, const BBox& bounds, int& axis, double& split);

   KdNode *nodes;
   BoundEdge* edges;
   int nAllocedNodes, nextFreeNode;
   unsigned maxDepth;
   unsigned maxObjects;
   unsigned travCost;
   unsigned isectCost;
};

#endif // KDTREE_H
