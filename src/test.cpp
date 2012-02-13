#include "../include/MyClass.h"
#include <string>
using std::string;


int main()
{
   string * A = new string("foobar");
   string * B = new string("foobaz");

   MyClass * mc = new MyClass(A, B);
   
   return 0;
}
