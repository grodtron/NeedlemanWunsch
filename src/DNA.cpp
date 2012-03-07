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
      throw DNA::InvalidIndexException(i);
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
   // throw exception if there is an invalid character present
   for(size_t i = 0; i < length - 1; ++i){
      char c = sequence[i];
      if(c != 'A' && c != 'C' && c != 'G' && c != 'T'){
         throw DNA::InvalidSequenceException(sequence, i);
      }
      this->sequence[i] = c;
   }
   // this is not done inside the loop because it causes an exception to
   // be thrown
   this->sequence[length - 1] = '\0';

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
