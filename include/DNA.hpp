#ifndef DNA_H_
#define DNA_H_

#include <iostream>
using std::ostream;
using std::istream;

class DNA{

      char * sequence;
      size_t length;

      void setSequence(char * sequence);
   
   public:

      static const size_t MAX_LENGTH = 1024;

      friend ostream & operator<< (ostream & s, const DNA & d);
      friend istream & operator>> (istream & s,       DNA & d);

      DNA(char * sequence);
      DNA();
     ~DNA();
      char operator[](size_t i);
};

#endif
