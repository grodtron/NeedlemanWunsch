#ifndef NeedlemanWunsch_H_
#define NeedlemanWunsch_H_

#include <string>
using std::string;

#include "Alignment.h"

// This struct represents each cell in the score matrix
// and holds traceback information as well
struct matrixCell{
   // the score at this point inside the matrix
   int score;
   // a set of flags indicating the possible directions
   // for this position in the matrix
   unsigned char direction;
   // the length of the horizontal gap that leads to this
   // cell. This is used for calculating constant and affine
   // gap penalties
   int hGapLen;
   // same as hGapLen, except for vertical gaps
   int vGapLen;
};

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      Alignment alignment;

      // the score matrix
      int width;
      int height;
      matrixCell ** matrix;

      // initialize the matrix
      void _init();

      // get alignment from initialized matrix
      void _traceBack();

      // get similarity between two chars
      int similarity(char a, char b);
      int (*similarityFunction)(char a, char b);
      
      int gapPenalty(int gapLength);
      int (*gapPenaltyFunction)(int gapLength);

      void deleteMatrix();

   public:
      NeedlemanWunsch(string a, string b);
      NeedlemanWunsch();
     ~NeedlemanWunsch();
      void setSimilarityFunction(int (*f)(char, char));
      void setGapPenaltyFunction(int (*f)(int));
      void setStrings(string a, string b);
      void align(Alignment &);

      // directional flags for matrixCell;
      static const unsigned char VERTICAL  ;
      static const unsigned char DIAGONAL  ;
      static const unsigned char HORIZONTAL;
};

#endif
