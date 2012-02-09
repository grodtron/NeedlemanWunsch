#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include "../include/drawSurface.h"

using std::vector;
using std::sin;
using std::cos;

void changeSize(int w, int h) {

   // Prevent a divide by zero, when window is too short
   // (you cant make a window of zero width).
   if (h == 0)
      h = 1;

   float ratio =  w * 1.0 / h;

   // Use the Projection Matrix
   glMatrixMode(GL_PROJECTION);

   // Reset Matrix
   glLoadIdentity();

   // Set the viewport to be the entire window
   glViewport(0, 0, w, h);

   // Set the correct perspective.
   gluPerspective(45.0f, ratio, 0.1f, 100.0f);

   // Get Back to the Modelview
   glMatrixMode(GL_MODELVIEW);
}

float angle = 0.0f;

vector< vector<float> > array;
const int w = 12;
const int h = 12;

void init(){

   using std::srand;
   using std::rand;
   using std::time;
   srand(time(NULL));

   for(int i = 0; i < w; ++i){
      array.push_back(vector<float>());
      for(int j = 0; j < h; ++j){
         array[i].push_back((rand()%128)/32.0);
      }
   }

}

void renderScene(void) {

   if (array.size() == 0) init();

   // Clear Color and Depth Buffers
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Reset transformations
   glLoadIdentity();
   // Set the camera
   gluLookAt(  5.0f * cos(angle), 8.0f, 5.0f * sin(angle),
         0.0f, 0.0f,  0.0f,
         0.0f, 1.0f,  0.0f);

   //glRotatef(angle, 0.0f, 1.0f, 0.0f);

   drawSurface(array, w, h);

   //GLfloat position[] = { 5.0f, 8.0f, 0.0f, 1.0f };
   //glLightfv(GL_LIGHT0, GL_POSITION, position);

   angle+=0.001f;

   glutSwapBuffers();
}
