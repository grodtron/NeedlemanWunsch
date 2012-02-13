#ifndef NeedlemanWunsch_H
#define NeedlemanWunsch_H

#include <string>
using std::string;

struct tree{
   struct tree* left;
   struct tree* center;
   struct tree* right;
   int i;
   int j;
};

class NeedlemanWunsch{
   public:
      NeedlemanWunsch(string & a, string & b);
      NeedlemanWunsch(string * a, string * b);
      void print();
      int getF(int i, int j);
      void setSimilarityFunction(int (*f)(char, char));
      void align();
      void fullAlign();
};

#endif
