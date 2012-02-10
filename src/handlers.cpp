#include <GL/freeglut.h>
#include "../include/mainloop.hpp"

//#define DEBUG

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

void processKeys(unsigned char key, int x, int y){
   #ifdef DEBUG
   cout << "regular key pressed: " << key << endl;
   #endif

   switch(key){
      case '+':
         incrementDistance(-0.375);
         break;
      case '-':
      incrementDistance(0.375);
         break;
   }
   
}

void processSpecialKeys(int key, int x, int y) {
   #ifdef DEBUG
   cout << "special key pressed: " << key << endl;
   #endif

   switch(key){
      case GLUT_KEY_LEFT:
         incrementAzimuth(0.05);
         break;
      case GLUT_KEY_RIGHT:
         incrementAzimuth(-0.05);
         break;
      case GLUT_KEY_UP:
         incrementInclination(-0.05);
         break;
      case GLUT_KEY_DOWN:
         incrementInclination(0.05);
         break;
   }
}
