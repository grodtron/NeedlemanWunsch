#ifndef NeedlemanWunsch_H_
#define NeedlemanWunsch_H_

#include "DNA.h"

// This struct represents each cell in the score matrix
// and holds traceback information as well
template <typename T>
struct matrixCell{
   // the score at this point inside the matrix
   T score;
   // a set of flags indicating the possible directions
   // for this position in the matrix
   unsigned char direction;
};

template <typename T>
class NeedlemanWunsch{

      // the input strings
      DNA A;
      DNA B;

      Alignment alignment;

      // the score matrix
      size_t width;
      size_t height;
      matrixCell<T> ** matrix;

      // initialize the matrix
      void _init();

      // get alignment from initialized matrix
      void _traceBack();

      T matchScore;
      T gapScore;
      T misMatchScore;

      // get similarity between sequences at given index
      T similarity(size_t i, size_t j);

      void deleteMatrix();

   public:
      NeedlemanWunsch(DNA a, DNA b);
      NeedlemanWunsch();
     ~NeedlemanWunsch();
      void setMatchScore(T val);
      void setMisMatchScore(T val);
      void setGapPenalty(T val);
      void align(Alignment &);

      // directional flags for matrixCell;
      static const unsigned char VERTICAL  ;
      static const unsigned char DIAGONAL  ;
      static const unsigned char HORIZONTAL;
};

#endif
