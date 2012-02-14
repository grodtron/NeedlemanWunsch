#include <GL/freeglut.h>
#include "../include/mainloop.hpp"
#include "../include/draw.hpp"
#include "../include/handlers.hpp"

void init (void) {
   glEnable (GL_DEPTH_TEST);
   glEnable (GL_LIGHTING);
   glEnable (GL_COLOR_MATERIAL);

   float global_ambient[] = { 0.5, 0.5, 0.5, 0.2 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

   glShadeModel(GL_SMOOTH);
   glEnable (GL_LIGHT0);

   float specular0[] = {1.0f, 1.0f, 1.0f , 0.1f};
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

   float diffuse0[] = {1.0f, 0.2f, 0.2f , 1.0f};
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);

   float position0[] = { 5.0, 5.0, 5.0, 1.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, position0);

   glEnable (GL_LIGHT1);

   float specular1[] = {1.0f, 0.8f, 0.3f , 0.1f};
   glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

   float diffuse1[] = {0.2f, 1.0f, 0.2f , 1.0f};
   glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);

   float position1[] = { 4.0, 5.0, 6.0, 1.0 };
   glLightfv(GL_LIGHT1, GL_POSITION, position1);

   //glLightfv

}

int main (int argc, char **argv) {
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Needleman-Wunsch Visualization");
   init ();

   glutDisplayFunc (display);
   glutIdleFunc (display);
   glutReshapeFunc (reshape);

   glutKeyboardFunc(processKeys);
   glutSpecialFunc(processSpecialKeys);

   glutMainLoop ();
   return 0;
}


