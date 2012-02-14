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
   cout << "Enter two strings to be aligned: ";
   cin >> A >> B;
   NeedlemanWunsch nw(&A, &B);
   nw.fullAlign();
   nw.print();
   
   return 0;
}
