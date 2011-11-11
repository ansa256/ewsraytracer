#ifndef WAVEFRONTPARSER_H
#define WAVEFRONTPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Storage/Storage.h"

class Hash;
class Mesh;

using namespace std;

class WavefrontParser
{
public:
   WavefrontParser();
   virtual ~WavefrontParser();

   void setHash(Hash* h);
   bool load(const string& filename);
   GeometryObject* getMesh() { return storage; }

private:
   void getNextLine(string& line);
   void handleFace(string line, Mesh* mesh, int vertexOffset, int normalOffset);
   vector<string> faceSplit(string str) const;
   vector<int> vertexSplit(string str) const;

   double scale;
   string textureDir;
   Storage* storage;
   ifstream in;
};

#endif // WAVEFRONTPARSER_H
