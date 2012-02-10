#ifndef FINDPATHS_H
#define FINDPATHS_H
#include <vector>

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


void getPaths(
      const std::vector< std::vector<int> >& a,
      std::vector< std::vector<point*>* >* paths);
#endif
