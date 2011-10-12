#ifndef KDTREE_H
#define KDTREE_H

#include "Storage.h"

typedef unsigned int uint32_t;

struct KdNode {
   KdNode(vector<int> _idxs);
   KdNode(KdNode* l, KdNode* r, int _axis, double _split);

   bool isLeaf() const;

   KdNode* left;
   KdNode* right;
   vector<int> idxs;
   int axis;
   double split;
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
   KdNode* buildTree(unsigned depth, vector<int> idxs, const BBox& bounds);
   bool checkNode(const Ray& ray, KdNode* node, double& tmin, ShadeRecord& sr) const;
   bool checkNodeShadow(const Ray& ray, KdNode* node, double& tmin) const;
   void findSplit(vector<int>& idxs, const BBox& bounds, int& axis, double& split);

   KdNode *root;
   BoundEdge* edges;
   unsigned maxDepth;
   unsigned maxObjects;
   unsigned travCost;
   unsigned isectCost;
};

#endif // KDTREE_H
