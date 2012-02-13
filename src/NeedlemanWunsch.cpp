#include <vector>
using std::vector;

#include <string>
using std::string;

class NeedlemanWunsch{

      string * myString1;
      string * myString2;

      int length1;
      int length2;

      vector< vector<int>* >* myVector;

      void _init();
   public:
      NeedlemanWunsch(string * a, string * b);
};

NeedlemanWunsch::NeedlemanWunsch(string * a, string * b)
: myString1(a), myString2(b)
{
   length1 = myString1->size();
   length2 = myString2->size();
   myVector = new vector< vector<int>* >;
}
