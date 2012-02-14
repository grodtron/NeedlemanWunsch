#ifndef NeedlemanWunsch_H
#define NeedlemanWunsch_H

#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>
using std::copy;

#include <deque>
using std::deque;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <string>
using std::string;

#if __GNUC__ == 4
#if __GNUC_MINOR__ < 3
#include <tr1/memory>
using std::tr1::shared_ptr;
#else
#include <memory>
using std::shared_ptr;
#endif
#else
This is some garbage incase __GNUC__ is less than four
#endif

struct alignment{
   string A;
   string B;
};

struct tree{
   shared_ptr<struct tree> left;
   shared_ptr<struct tree> center;
   shared_ptr<struct tree> right;
   int i;
   int j;
};

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      vector<alignment*> alignments;
      tree* paths;
      map<pair<int, int>, tree*> knownNodes;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();

      // align the strings
      tree* _fullAlign(deque<char>*, deque<char>*, int, int);

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
