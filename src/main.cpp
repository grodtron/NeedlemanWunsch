#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include <iomanip>
using std::setw;

#include <vector>
using std::vector;

#include <algorithm>
using std::copy;

#include <iterator>
using std::back_inserter;

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
   DPM<int>::Iterator mit = matrix.begin();
   DPM<int>::Iterator mend= matrix.end();

   vector<DPM<int>::Alignment> testVect;
   //testVect.resize(10);

   //copy(mend, mit, back_inserter(testVect));
   copy(mit, mend, back_inserter(testVect));

   vector<DPM<int>::Alignment>::iterator vit = testVect.begin();
   vector<DPM<int>::Alignment>::iterator vend = testVect.end();

   //mit = DPM<int>::Iteratormatrix.begin();
   //mend= DPM<int>::Iterator(matrix.end());

   // iterate through the results
   while (vit != vend){
      (*vit).print();
      cout << endl;
      ++vit;
   }

   cout << endl << endl << "Original DNA was:" << endl << a << endl << b << endl;

   return 0;
}
