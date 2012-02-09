#include <GL/freeglut.h>
#include "../include/handlers.h"

int main(int argc, char **argv) {

   // init GLUT and create window
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(1200,100);
   glutInitWindowSize(320,320);
   glutCreateWindow("OpenGL 2D array test");

   glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
   glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
   glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glEnable(GL_COLOR_MATERIAL);

   //glMaterialf(GL_FRONT_AND_BACK, GL_AMIBENT_AND_DIFFUSE, )
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

   // register callbacks
   glutDisplayFunc(renderScene);
   glutReshapeFunc(changeSize);
   glutIdleFunc(renderScene);

   // enter GLUT event processing cycle
   glutMainLoop();

   return 1;
}
