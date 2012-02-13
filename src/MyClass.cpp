#include <vector>
using std::vector;

#include <string>
using std::string;

class MyClass{
      string * myString1;
      string * myString2;

      int length1;
      int length2;

      vector< vector<int>* > * myVector;

   public:
      MyClass(string * a, string * b);
};

MyClass::MyClass(string * a, string * b)
: myString1(a), myString2(b)
{
   length1 = myString1->size();

   // commenting either or both of these lines makes
   // it work

   length2 = myString2->size();

   myVector = new vector< vector<int>* >;
}
