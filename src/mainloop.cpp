#include <GL/freeglut.h>
#include "../include/draw.hpp"

GLfloat angle = 0.0;

void display (void) {
   glClearColor (0.0,0.0,0.0,1.0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();  
   gluLookAt (-5.0, 3.0, -8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glRotatef(angle,0,1,0);

   //drawGrid();
   drawBars();

   glutSwapBuffers();
   angle += 0.06;
}

void reshape (int w, int h) {
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
   glMatrixMode (GL_MODELVIEW);
}
