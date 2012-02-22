#ifndef NEW_NW_H
#define NEW_NW_H

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <string>
using std::string;

typedef vector< pair<string,string> > alignmentVector; 
typedef map< pair<int, int>, alignmentVector* > svpCoordMap;

class NewNeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      alignmentVector * alignments;
      svpCoordMap knownNodes;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();

      // align the strings
      alignmentVector * _fullAlign(int, int);

      // get similarity between two chars
      int similarity(char a, char b);

      int (*similarityFunction)(char a, char b);

   public:
      NewNeedlemanWunsch(string a, string b);
      ~NewNeedlemanWunsch();
      void print();
      int getF(int i, int j);
      void setSimilarityFunction(int (*f)(char, char));
      void fullAlign();
};

#endif
