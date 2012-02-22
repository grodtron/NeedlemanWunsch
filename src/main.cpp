#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <string>
using std::string;

#include "../include/old_NW.h"
#include "../include/new_NW.h"
#include "../include/timer.h"


int main(int argc, const char *argv[])
{

   Timer tm;

   string A, B;

   if(argc == 3){
      A = argv[1];
      B = argv[2];
   }else if(argc == 1){
      cout << "Enter two strings to be aligned: ";
      cin >> A >> B;
   }else{
      cout << "Received bad args (got " << argc << ", should be 2 or 0)." << endl;
      return 1;
   }

   
   tm.start();
   NewNeedlemanWunsch nnw(A, B);
   nnw.fullAlign();
   tm.stop();
   cout << tm.duration() << ' ';

   tm.start();
   OldNeedlemanWunsch onw(A, B);
   onw.fullAlign();
   tm.stop();
   cout << tm.duration() << endl;

   //onw.print();
   
   return 0;
}
