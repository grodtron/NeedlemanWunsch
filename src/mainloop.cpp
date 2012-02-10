#include <GL/freeglut.h>
#include <cmath>
#include "../include/draw.hpp"

using std::cos;
using std::sin;

const float pi = 3.14159;

float azimuth = 0.0;
float inclination = pi / 3;
float distance = 10.0;

void incrementDistance(float amount){
   distance += amount;
   distance = distance < 1 ? 1 : distance;
   distance = distance > 30? 30: distance;
}

void incrementAzimuth(float amount){
   azimuth += amount;
}

void incrementInclination(float amount){
   inclination += amount;
   inclination = inclination < 0.05      ? 0.05      : inclination;
   inclination = inclination > pi - 0.05 ? pi - 0.05 : inclination;
}

void display (void) {
   glClearColor (0.0,0.0,0.0,1.0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();  
   float x=distance * cos(azimuth) * sin(inclination),
         y=distance * sin(azimuth) * sin(inclination),
         z=distance                * cos(inclination);
   gluLookAt (
      x, z, y, // eye position
      0.0, 0.0, 0.0,   // center position
      0.0, 1.0, 0.0    // up vector
   );

   //glRotatef(azimuth,0,1,0);

   //drawGrid();
   drawBars();

   glutSwapBuffers();
   azimuth += 0.006;
}

void reshape (int w, int h) {
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
   glMatrixMode (GL_MODELVIEW);
}
