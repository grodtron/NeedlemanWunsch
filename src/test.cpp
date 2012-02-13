#include "../include/NeedlemanWunsch.h"
#include <string>
using std::string;


int main(int argc, const char *argv[])
{
   string A = "foobar";
   string B = "foobaz";

   NeedlemanWunsch * nw = new NeedlemanWunsch(&A, &B);
   
   return 0;
}
