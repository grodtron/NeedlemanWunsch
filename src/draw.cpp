#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <iostream>

using std::rand;
using std::srand;
using std::time;

using std::cout;
using std::endl;

const int width = 11;
const int height = 11;

const float wOffset = (width  - 1)/2.0;
const float hOffset = (height - 1)/2.0;

void drawGrid(){

   glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;

   glBegin(GL_LINES);
   glColor3f(1.0,0.0,0.0);

   for(int i = 0; i < width; ++i){
      for(int j = 0; j < height; ++j){
         glVertex3f(0-wOffset  , 0 , j-hOffset);
         glVertex3f(width-1-wOffset , 0 , j-hOffset);
         glVertex3f(i-wOffset  , 0 , 0-hOffset);
         glVertex3f(i-wOffset  , 0 , height-1-hOffset);
      }
   }

   glEnd();
}

void drawBars(){

   const float size = 1;

   float x = -wOffset,
         y = 0.0,
         z = -hOffset;

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
         float tallness = (i + j + 5)/5.0;
         float scaleFactor = tallness/vScale;

         glScalef(1, tallness/vScale, 1);
         glTranslatef(0, (1 - (1/scaleFactor))*(size/2) ,0);

         vScale = tallness;
         // now that we are in the right place, with the right
         // scale, draw the cube.

         glutSolidCube(size);

         // increment position in the row
         glTranslatef(0.0,0.0,1);
      }
      // go to the next row
      glTranslatef(1, 0, -width);
   }
   //cout << endl << endl;

   glScalef(1,1/vScale,1);

   glTranslatef(wOffset - width, 0, hOffset);

   glutSolidSphere(wOffset, 16,16);
}
