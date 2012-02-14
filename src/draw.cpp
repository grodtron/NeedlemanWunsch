#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>
//#include "../include/findpaths.hpp"
#include "../include/NeedlemanWunsch.h"

using std::rand;
using std::srand;
using std::time;

using std::cout;
using std::endl;

using std::vector;

const int width = 31;
const int height = 31;

const float wOffset = (width  - 1)/2.0;
const float hOffset = (height - 1)/2.0;

tree * paths = NULL;
NeedlemanWunsch * nw = NULL;

const float scale   = 0.2;

void initNW(){
   srand(time(NULL));

   char chars[4] = {'A', 'C', 'T', 'G'};

   string * A = new string;
   string * B = new string;

   // initialize random strings of DNA
   for(int i = 0; i < width; ++i)  A->push_back(chars[ rand() % 4 ]);
   for(int j = 0; j < height; ++j) B->push_back(chars[ rand() % 4 ]);

   nw = new NeedlemanWunsch(A, B);

   nw->align();
}

void drawGrid(){

   if(!nw) initNW();

   glColorMaterial ( GL_FRONT, GL_DIFFUSE ) ;
   glColor3f(1.0,1.0,1.0);


   const float size = 1;

   float x = -wOffset*scale,
         y = 0.0,
         z = -hOffset*scale;

   glTranslatef(x, y, z);

   float vScale = 1;

   for(int i = 0; i < width; ++i){
      for(int j = 0; j < height; ++j){

         // scale to get to the height that we want.
         // calculate the scaling based on the current
         // value to avoid fully unscaling and then re-scaling
         //
         // The translation should be calculated similarly.
         // it is -(size/2)/scaleFactor + size/2
         float tallness = (nw->getF(i, j) / 4.0) + 1.0;
         float scaleFactor = tallness/vScale;

         glScalef(1, tallness/vScale, 1);
         glTranslatef(0, (1 - (1/scaleFactor))*(size/2)*scale ,0);

         vScale = tallness;
         // now that we are in the right place, with the right
         // scale, draw the cube.

         glutSolidCube(size * scale);

         // increment position in the row
         glTranslatef(0.0,0.0,scale);
      }
      // go to the next row
      glTranslatef(scale, 0, -width*scale);
   }
   // restore
   glScalef(1,1/vScale,1);
   glTranslatef((wOffset - width)*scale, 0, hOffset*scale);
}
