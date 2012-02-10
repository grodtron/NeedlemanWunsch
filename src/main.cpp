#include <GL/freeglut.h>
#include "../include/mainloop.hpp"
#include "../include/draw.hpp"
#include "../include/handlers.hpp"

void init (void) {
   glEnable (GL_DEPTH_TEST);
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable ( GL_COLOR_MATERIAL ) ;
}

int main (int argc, char **argv) {
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Cubotron");
   init ();

   glutDisplayFunc (display);
   glutIdleFunc (display);
   glutReshapeFunc (reshape);

   glutKeyboardFunc(processKeys);
   glutSpecialFunc(processSpecialKeys);

   glutMainLoop ();
   return 0;
}


