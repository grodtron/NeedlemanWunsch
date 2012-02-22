#ifndef NeedlemanWunsch_H_
#define NeedlemanWunsch_H_

#include <deque>
using std::deque;

#include <vector>
using std::vector;

#include <string>
using std::string;

struct alignment{
   string A;
   string B;
};

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      vector<alignment*> alignments;
      //tree* paths;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();

      // align the strings
      void _fullAlign(deque<char>*, deque<char>*, int, int);

      // get similarity between two chars
      int similarity(char a, char b);

      int (*similarityFunction)(char a, char b);

   public:
      NeedlemanWunsch(string a, string b);
      void print();
      int getF(int i, int j);
      void setSimilarityFunction(int (*f)(char, char));
      void fullAlign();
};

#endif
