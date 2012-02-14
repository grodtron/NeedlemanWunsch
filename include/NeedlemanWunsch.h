#ifndef NeedlemanWunsch_H
#define NeedlemanWunsch_H
#include <deque>
using std::deque;

#include <vector>
using std::vector;

#include <string>
using std::string;

struct alignment{
   string * A;
   string * B;
};

struct tree{
   struct tree* left;
   struct tree* center;
   struct tree* right;
   int i;
   int j;
};

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      string alignedA;
      string alignedB;

      vector<alignment*> alignments;
      tree* paths;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();
      // align the strings
      void  _align();
      tree* _fullAlign(deque<char>*, deque<char>*, int, int);
      // get similarity between two chars
      int  similarity(char a, char b);

      int (*similarityFunction)(char a, char b);

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
