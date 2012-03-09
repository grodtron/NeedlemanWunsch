#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include <iomanip>
using std::setw;

#include <climits>

#include "../include/DNA.hpp"
#include "DPM.cpp"

int main(int argc, const char *argv[])
{

   DNA a;
   DNA b;

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

   // create the DPM object
   DPM<int> matrix(a, b);

   // get iterators from it
   DPM<int>::Iterator it = matrix.begin();
   DPM<int>::Iterator end= matrix.end();

   // iterate through the results
   while (it != end){
      (*it).print();
      cout << endl;
      ++it;
   }

   return 0;
}
