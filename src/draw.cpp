#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include "../include/findpaths.hpp"

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

vector< vector<int> > grid(0);
vector< vector<point*>* >* paths = NULL;

const float scale   = 0.2;

void initGrid(){
   srand(time(NULL));
   for(int i = 0; i < width; ++i){
      grid.push_back(vector<int>(height));
      for(int j = 0; j < height; ++j){
         if (i == 0 && j == 0) grid[0][0] = 28;
         else if(i == 0) grid[0][j] = rand()%30 + 1;
         else if(j == 0) grid[i][0] = rand()%30 + 1;
         else{
            int temp = grid[i][j-1] > grid[i-1][j] ? grid[i][j-1] : grid[i-1][j];
            grid[i][j] = temp > grid[i-1][j-1] + 2 ? temp : grid[i-1][j-1] + 2;
         }
      }
   }
   paths = new vector< vector<point*>* >;
   getPaths(grid, paths);
   cout << "received " << paths->size() << " paths" << endl;
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

void drawPaths(){

   //drawGrid();
   if(!grid.size()) initGrid();

   vector< vector<point*>*>::iterator it = paths->begin();
   vector< vector<point*>*>::iterator end = paths->end();

   glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;

   glColor3f(0.0,0.0,1.0);

   for( ; it != end; ++it){
      vector<point*>::iterator jt = (*it)->begin();
      vector<point*>::iterator jnd = (*it)->end();

      glBegin(GL_LINE_STRIP);

      //cout << "drawing path: ";

      for( ; jt != jnd; ++jt){
         float x = ((*jt)->x - wOffset) * scale;
         float y = (0.2 + grid[(*jt)->x][(*jt)->y] / 4.0) * scale;
         float z = ((*jt)->y - hOffset) * scale;
         glVertex3f(x,y,z);
        // cout << x << ' ' << y <<  ' ' << z << '|';
      }
      //cout << endl;

      glEnd();
   }
}

void drawBars(){

   if(!grid.size()) initGrid();

   drawPaths();

   glColor3f(1.0,0.0,0.0);

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
