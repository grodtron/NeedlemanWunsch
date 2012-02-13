#ifndef MyClass_H
#define MyClass_H

#include <string>
using std::string;

#include <vector>
using std::vector;

class MyClass{
      string * A;
      string * B;
      vector< vector<int>* > * myVector;

      int length1;
      int length2;
   public:
      MyClass(string * a, string * b);
};

#endif
