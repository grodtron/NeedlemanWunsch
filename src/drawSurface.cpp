#include <GL/freeglut.h>
#include <vector>
using std::vector;

void drawColumn(float x, float y, float w, float h){
   // translate and scale, draw the cube, then untranslate and scale
   // so that the cube becomes a column where we want it
   glTranslatef(x, -w/2.0, y);
   glScalef(1,h/w,1);
   glutSolidCube(w);
   glScalef(1,w/h,1);
   glTranslatef(-x, w/2.0, -y);
}

void drawAxis(){
   glBegin(GL_LINES);
   glColor3f(1,0,0);
   glVertex3f(0,0,0);
   glVertex3f(1,0,0);
   glColor3f(0,1,0);
   glVertex3f(0,0,0);
   glVertex3f(0,1,0);
   glColor3f(0,0,1);
   glVertex3f(0,0,0);
   glVertex3f(0,0,1);
   glEnd();
}

void drawSurface(vector< vector<float> > array, const int w, const int h){

   const float SCALE = 0.3;

   float xOffset = (-w/2.0) * SCALE;
   float yOffset = (-h/2.0) * SCALE;

   //glBegin(GL_LINES);

/*

   for(int i = 0; i < w; ++i){
      for(int j = 0; j < h; ++j){
         //glVertex3f( i*SCALE + xOffset, 0                , j*SCALE + yOffset );
         //glVertex3f( i*SCALE + xOffset, array[i][j]*SCALE, j*SCALE + yOffset );
         glColor3f(1,1,1);
         drawColumn( i*SCALE + xOffset, j*SCALE + yOffset, 2*SCALE, array[i][j]*SCALE );
      }
   }
*/

   //glEnd();

   drawAxis();

   drawColumn(1,1,0.5,2);

}
