#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <string>
using std::string;

#include "../include/NeedlemanWunsch.h"

int main(int argc, const char *argv[])
{

   string A, B;

   // get strings to aline from command line args, otherwise ask for them:
   if(argc == 3){
      A = argv[1];
      B = argv[2];
   }else if(argc == 1){
      cout << "Enter two strings to be aligned: " << endl << endl;
      cin >> A >> B;
   }else{
      cout << "Received bad args (got " << argc-1 << ", should be 2 or 0)." << endl;
      return 1;
   }
   
   NeedlemanWunsch nw(A, B);
   nw.align();
   nw.print();
   
   return 0;
}
