#include <string.h>
// for strcpy

#include <iostream>
using std::ostream;
using std::istream;

#include "../include/DNA.hpp"


DNA::DNA(char * sequence){
   setSequence(sequence);
}

DNA::DNA(): sequence(NULL), length(0) {}

DNA::~DNA(){
   delete[] sequence;
}

char DNA::operator[] (size_t i){
   if(i >= length){
      // TODO - throw
      return '\0';
   }
   return sequence[i];
}

void DNA::setSequence(char * sequence){
   if(this->sequence){
      delete[] this->sequence;
   }
   // get the length of the sequence that was input
   for(length = 0; sequence[length] != '\0'; ++length);
   // include null terminator
   ++length;
   // alocate space for the new sequence
   this->sequence = new char[length];
   // copy the string into the new sequence
   strcpy(this->sequence, sequence);
   // TODO - validate
}

ostream & operator<< (ostream & s, const DNA & d){
   s << d.sequence;
   return s;
}

istream & operator>> (istream & s, DNA & d){
   char input[DNA::MAX_LENGTH];
   s >> input;
   d.setSequence(input);
   return s;
}
