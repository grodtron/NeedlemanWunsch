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

   for(bool validInput = false; !validInput;){
      try{
         cout << "Enter the two strands of DNA: ";
         cin  >> a >> b;
         validInput = true;
      }catch(DNA::InvalidSequenceException& e){
         cout << "Invalid character '" << e.getSequence()[e.getPos()] << "' at:" << endl;
         cout << "   " << e.getSequence()  <<        endl
              << "   " << setw(e.getPos() + 1) << '^' << endl;
         cin.ignore(INT_MAX,'\n');
      }
   }

   DPM<int> matrix(a, b);
   matrix.align();
   cout << endl << endl;
   matrix.next();
   cout << endl << endl;
   matrix.next();
   cout << endl << endl;
   matrix.next();
   cout << endl << endl;

   cout << "the sequences were: " << endl; 
   cout << a << endl << b << endl;

   return 0;
}
