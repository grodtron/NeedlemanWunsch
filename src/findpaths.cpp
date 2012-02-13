#include <iostream>
#include <deque>
#include <vector>

using std::vector;
using std::deque;

using std::cout;
using std::endl;
using std::ostream;

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

ostream& operator << ( ostream& os, point p ){
   os << '(' << p.x << ", " << p.y << ')';
   return os;
}

// recursively generate a tree structure that represents all of the
// possible paths through the matrix
tree* findPaths(const vector< vector<int> > & a, int x, int y){

   tree* root     = new tree;
   root->point    = new point;
   root->point->x = x;
   root->point->y = y;
   root->left     = NULL;
   root->right    = NULL;
   root->center   = NULL;

   if(x > 0 && a[x-1][y] == a[x][y] ){
      root->left = findPaths(a, x-1, y);
   }
   if(y > 0 && a[x][y-1] == a[x][y] ){
      root->right = findPaths(a, x, y-1);
   }
   if(x > 0 && y > 0 && a[x-1][y-1] + 2 == a[x][y] ){
      root->center = findPaths(a, x-1, y-1);
   }

   return root;
}

// recursively traverse the tree, pushing and popping elements onto
// a running ancestry list. This same method could be used to create a list of
// paths by copying the current deque to a vector at the end point of each recursion
// this is the non-public implementation of printPaths
void getPaths(tree* tree, deque<point*> &ancestry, vector< vector<point*>* >* paths){

   ancestry.push_back(tree->point);

   if( tree->left ){
      getPaths(tree->left, ancestry, paths);
      ancestry.pop_back();
   }
   if( tree->center ){
      getPaths(tree->center, ancestry, paths);
      ancestry.pop_back();
   }
   if( tree->right ){
      getPaths(tree->right, ancestry, paths);
      ancestry.pop_back();
   }

   if( !(tree->right || tree->center || tree->left) ){

      // at the bottom of the recursion, copy out the current path
      // and push it into the vector of paths

      vector<point*>* path = new vector<point*>(ancestry.size());

      copy(ancestry.begin(), ancestry.end(), path->begin());

      paths->push_back(path);
      cout << "pushed path" << endl;
   }
}

// this is the public interface to the printPaths method
void getPaths(const vector< vector<int> > & a,vector< vector<point*>* >* paths){
   deque<point*> ancestry;
   tree* tree = findPaths(a, a.size() - 1, a.size() - 1);
   getPaths(tree, ancestry, paths);
}

