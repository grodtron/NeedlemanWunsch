#ifndef FINDPATHS_H
#define FINDPATHS_H
#include <iostream>

struct point{
   int x;
   int y;
};

struct tree{
   struct tree* left;
   struct tree* center;
   struct tree* right;
   struct point* point;
};

tree* findpaths(int a[50][50], int x, int y);

void printPaths(tree* tree);
#endif
