#include <iostream>
#include <deque>

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

std::ostream& operator << ( std::ostream& os, point p ){
   os << '(' << p.x << ", " << p.y << ')';
   return os;
}

// recursively generate a tree structure that represents all of the
// possible paths through the matrix
tree* findpaths(int a[50][50], int x, int y){

   tree* root     = new tree;
   root->point    = new point;
   root->point->x = x;
   root->point->y = y;
   root->left     = NULL;
   root->right    = NULL;
   root->center   = NULL;

   if(x > 0 && a[x-1][y] == a[x][y] ){
      root->left = findpaths(a, x-1, y);
   }
   if(y > 0 && a[x][y-1] == a[x][y] ){
      root->right = findpaths(a, x, y-1);
   }
   if(x > 0 && y > 0 && a[x-1][y-1] + 2 == a[x][y] ){
      root->center = findpaths(a, x-1, y-1);
   }

   return root;
}

// recursively traverse the tree, pushing and popping elements onto
// a running ancestry list. This same method could be used to create a list of
// paths by copying the current deque to a vector at the end point of each recursion
// this is the non-public implementation of printPaths
void printPaths(tree* tree, std::deque<point*> &ancestry){
   std::cout << "pushing " << *(tree->point) << std::endl;
   ancestry.push_back(tree->point);
   if( tree->left ){
      printPaths(tree->left, ancestry);
      std::cout << "popping " << *ancestry.back() << std::endl;
      ancestry.pop_back();
   }
   if( tree->center ){
      printPaths(tree->center, ancestry);
      std::cout << "popping " << *ancestry.back() << std::endl;
      ancestry.pop_back();
   }
   if( tree->right ){
      printPaths(tree->right, ancestry);
      std::cout << "popping " << *ancestry.back() << std::endl;
      ancestry.pop_back();
   }
   if( !(tree->right || tree->center || tree->left) ){
      std::deque<point*>::iterator it = ancestry.begin();
      std::deque<point*>::iterator end   = ancestry.end();
      std::cout << "Beginning of Path: " << std::endl;
      while(it != end){
         std::cout << **it << ", ";
         ++it;
      }
      std::cout << std::endl;
   }
}

// this is the public interface to the printPaths method
void printPaths(tree* tree){
   std::deque<point*>* ancestry = new std::deque<point*>;
   printPaths(tree, *ancestry);
   delete ancestry;
}

