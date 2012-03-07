#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include "../include/DNA.hpp"

int main(int argc, const char *argv[])
{

   DNA a;
   DNA b;

   cout << "Enter the two strands of DNA: ";
   cin  >> a >> b;

   cout << "the sequences were: " << endl; 
   cout << a << endl << b << endl;

   return 0;
}
