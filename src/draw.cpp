#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <iostream>

using std::rand;
using std::srand;
using std::time;

using std::cout;
using std::endl;

const int width = 31;
const int height = 31;

const float wOffset = (width  - 1)/2.0;
const float hOffset = (height - 1)/2.0;

int grid[width][height] = {{0}};

const float scale   = 0.2;

void initGrid(){
   for(int i = 0; i < width; ++i){
      for(int j = 0; j < height; ++j){
         if(i == 0) grid[0][j] = height - j;
         else if(j == 0) grid[i][0] = width - i;
         else{
            int temp = grid[i][j-1] > grid[i-1][j] ? grid[i][j-1] : grid[i-1][j];
            grid[i][j] = temp > grid[i-1][j-1] + 2 ? temp : grid[i-1][j-1] + 2;
         }
      }
   }
}

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

   if(!grid[0][0]) initGrid();

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
         float tallness = grid[i][j] / 4.0;
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
   //cout << endl << endl;

   glScalef(1,1/vScale,1);

   glTranslatef((wOffset - width)*scale, 0, hOffset*scale);

   //glutSolidSphere(wOffset, 16,16);
}
