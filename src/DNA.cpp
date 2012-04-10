/*
 * Copyright (C) 2012 Gordon Bailey
 */

#include <iostream>
using std::ostream;
using std::istream;

#include "../include/DNA.hpp"

// construct a DNA object based on a c-string
DNA::DNA(char * sequence) : sequence(NULL), length(0){
   setSequence(sequence);
}

// copy constructor
DNA::DNA(const DNA & other) :sequence(NULL), length(0){
   setSequence(other);
}

// default constructor
DNA::DNA(): sequence(NULL), length(0) {}

// destructor
DNA::~DNA(){
   delete[] sequence;
}

// access characters from the sequence. Throws an exception if
// the index is out of bounds
char DNA::operator[] (size_t i) const{
   if(i >= length){
      throw DNA::InvalidIndexException(i);
   }
   return sequence[i];
}

size_t DNA::size() const{
   return length;
}

// set the sequence of this object by copying the sequence of
// another object
void DNA::setSequence(const DNA & sequence){
   if(this->sequence){
      delete[] this->sequence;
   }
   length = sequence.size();
   // alocate space for the new sequence
   this->sequence = new char[length];
   // copy the string into the new sequence
   // No need to validate, we assume that any existing DNA
   // object has valid data
   for(size_t i = 0; i < length; ++i){
      this->sequence[i] = sequence[i];
   }
}

// set this objects sequences using a c-string
// validation is performed and an exception is
// thrown when appropriate
void DNA::setSequence(char * sequence){
   if(this->sequence){
      delete[] this->sequence;
   }
   // get the length of the sequence that was input
   for(length = 0; sequence[length] != '\0'; ++length);
   // don't include null terminator
   //
   // alocate space for the new sequence
   this->sequence = new char[length];
   // copy the string into the new sequence
   // throw exception if there is an invalid character present
   for(size_t i = 0; i < length; ++i){
      char c = sequence[i];
      if(c != 'A' && c != 'C' && c != 'G' && c != 'T'){
         throw DNA::InvalidSequenceException(sequence, i);
      }
      this->sequence[i] = c;
   }
}

ostream & operator<< (ostream & s, const DNA & d){
   // these are not c-strings, so they have to be output like this
   // (there is no NULL terminator)
   for(size_t i = 0; i < d.length; ++i){
      s.put(d.sequence[i]);
   }
   return s;
}

// load data from an istream
istream & operator>> (istream & s, DNA & d){
   char input[DNA::MAX_LENGTH];
   s >> input;
   d.setSequence(input);
   return s;
}
