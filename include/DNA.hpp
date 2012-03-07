#ifndef DNA_H_
#define DNA_H_

#include <iostream>
using std::ostream;
using std::istream;

#include <exception>
using std::exception;

class DNA{

      char * sequence;
      size_t length;

      void setSequence(char * sequence);
   
   public:

      // TODO - Define exception class and throw when appropriate

      class InvalidSequenceException: public exception{
            char * sequence;
            size_t pos;
         public:
            InvalidSequenceException(char * sequence, size_t pos) 
            : sequence(sequence), pos(pos) {};
            size_t getPos()      const { return pos;      };
            char * getSequence() const { return sequence; };
      };

      static const size_t MAX_LENGTH = 1024;

      friend ostream & operator<< (ostream & s, const DNA & d);
      friend istream & operator>> (istream & s,       DNA & d);

      DNA(char * sequence);
      DNA();
     ~DNA();
      char operator[](size_t i);
};

#endif
