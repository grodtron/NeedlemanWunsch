/*
 * Copyright (C) 2012 Gordon Bailey
 */
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <iomanip>
using std::setw;

#include <climits>

#include "../include/DNA.hpp"
#include "../include/DPM.hpp"

void getDna(DNA & a, DNA & b){
   // Request user input until valid input is given.
   // DNA object will throw an except if it is constructed with
   // an invalid string
   for(bool validInput = false; !validInput;){
      try{
         cout << "Enter the two strands of DNA: ";
         cin  >> a >> b;
         validInput = true;
      }catch(DNA::InvalidSequenceException& e){
         // display an error message to the user showing the invalid character
         cout << "Invalid character '" << e.getSequence()[e.getPos()] << "' at:" << endl;
         cout << "   " << e.getSequence()      <<        endl
              << "   " << setw(e.getPos() + 1) << '^' << endl;
         // clear the cin buffer. If the first sequence that was entered was invalid, then
         // consider both to be - force the user to re-enter both
         cin.ignore(INT_MAX,'\n');
      }
   }
}

// data type and constants for getting parameters
const char DOUBLE = 1 << 0;
const char INT    = 1 << 1;

union multi_t{
   int    asInt;
   double asDouble;
};

// get a single parameter using a custom prompt message. Determine if he value entered
// is an int or a double and set the flag appropriately
void getParameter(const char * message, multi_t & val, char & valtype){
   cout << message;
   cin  >> val.asDouble;

   if(val.asDouble == (int)val.asDouble){
      val.asInt = (int)val.asDouble;
      valtype = INT;
   }else{
      valtype = DOUBLE;
   }
}

// get all scoring parameters, as well as forcing them all to the same data-type (either int or double)
void getScoringParameters(char & type, multi_t & match, multi_t & gap, multi_t & mismatch){

   char match_type;
   char gap_type;
   char mismatch_type;

   getParameter("Enter match score: ", match, match_type);
   getParameter("Enter gap penalty (as a positive value): ", gap, gap_type);
   getParameter("Enter mismatch penalty (as a positive value): ", mismatch, mismatch_type);

   // if any of the parameters were given as floats, then they all
   // should be converted to floats. Otherwise they are all already ints
   if( (match_type | gap_type | mismatch_type) & DOUBLE){
      match.asDouble = match_type ? match.asDouble : (double)match.asInt;
      gap.asDouble = -1 * (gap_type ? gap.asDouble : (double)gap.asInt);
      mismatch.asDouble = -1 * (mismatch_type ? mismatch.asDouble : (double)mismatch.asInt);
      type = DOUBLE;

      // this is just a warning incase the sign for entering the gap and mismatch penalties is not
      // understood properly.
      if(gap.asDouble > 0){
         cout << "WARNING - negative gap penalty (gaps will increase the score of each alignment)" << endl;
      }
      if(mismatch.asDouble > 0){
         cout << "WARNING - negative mismatch penalty (mismatches will increase the score of each alignment)" << endl;
      }
   }else{
      type = INT;
      gap.asInt *= -1;
      mismatch.asInt *= -1;

      // same warning as above
      if(gap.asInt > 0){
         cout << "WARNING - negative gap penalty (gaps will increase the score of each alignment)" << endl;
      }
      if(mismatch.asInt > 0){
         cout << "WARNING - negative mismatch penalty (mismatches will increase the score of each alignment)" << endl;
      }
   }

}

void loopA(DNA & a, DNA & b, DPM* & matrix){

   getDna(a, b);

   char type;
   multi_t match, gap, mismatch;

   getScoringParameters(type, match, gap, mismatch);

   if (matrix) delete matrix;

   // create the DPM object
   if(type == DOUBLE){
      cout << "Using double-precision floating point numbers internally" << endl;
      matrix = new DPM(a, b, match.asDouble, gap.asDouble, mismatch.asDouble);
   }else{
      cout << "Using integer numbers internally" << endl;
      matrix = new DPM(a, b, match.asInt, gap.asInt, mismatch.asInt);
   }

}

int main(int argc, const char *argv[])
{

   DNA a;
   DNA b;
   DPM * matrix = NULL;

   const char BREAK_1 = 1 << 0;
   const char BREAK_2 = 1 << 1;

   char loop_flags = 0;

   while(!(loop_flags & BREAK_1)){
      loop_flags = 0;
      loopA(a, b, matrix);

      while(!(loop_flags & BREAK_2)){
         int option;

         cout << "Choose an option:" << endl
              << "   1) print both sequences" << endl
              << "   2) print initialized matrix" << endl
              << "   3) result of the Needleman-Wunsch (print entire matrix)" << endl
              << "   4) print one optimal alignment" << endl
              << "   5) print all optimal alignments" << endl
              << "   6) enter two new sequences" << endl
              << "   7) quit" << endl;

         cin >> option;

         switch(option){
            // print sequences
            case 1:
               cout << a << endl << b << endl;
               break;
            case 2:
               // print matrix
               matrix->print();
               break;
            case 3:
               // print traceback
               matrix->printTraceback();
               break;
            // print first
            case 4:
               {
                  DPM::Iterator m_it = matrix->begin();
                  (*m_it).print();
               }
               break;
            // print all
            case 5:
               {
                  DPM::Iterator m_it = matrix->begin();
                  DPM::Iterator m_end = matrix->end();
                  while(m_it != m_end){
                     (*m_it).print();
                     ++m_it;
                     cout << endl;
                  }
               }
               break;
            // enter new sequence
            case 6:
               loop_flags = BREAK_2;
               break;
            // quit
            case 7:
               loop_flags = BREAK_1 | BREAK_2;
               break;
         }
      }
   }

   if (matrix) delete matrix;

   return 0;
}
