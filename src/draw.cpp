#include <GL/freeglut.h>

void drawGrid(){

   glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;

   glBegin(GL_LINES);
   glColor3f(1.0,0.0,0.0);

   for(int i = 0; i < 11; ++i){
      for(int j = 0; j < 11; ++j){
         glVertex3f(0-5,0,j-5);
         glVertex3f(10-5,0,j-5);
         glVertex3f(i-5,0,0-5);
         glVertex3f(i-5,0,10-5);
      }
   }

   glEnd();
}

void drawBars(){
   glTranslatef(-3.0,1.0,-3.0);
   glScalef(1.0,2.0,1.0);
   glutSolidCube(1);
   glScalef(1.0,2.0,1.0);
   glTranslatef(2.0,0.25,0.0);
   glutSolidCube(1);
   glScalef(1.0,0.225,1.0);
   glTranslatef(1.0,-2.0,3.0);
}
