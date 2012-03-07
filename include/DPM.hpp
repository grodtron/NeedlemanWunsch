#ifndef DPM_H_
#define DPM_H_

#include "DNA.hpp"

#include <stack>
using std::stack;


template <typename T>
class DPM{

      // This struct represents each cell in the score matrix
      // and holds traceback information as well
      struct MatrixCell{
         // the score at this point inside the matrix
         T score;
         // a set of flags indicating the possible directions
         // for this position in the matrix
         unsigned char direction;
      };

      struct StackCell{
         size_t i;
         size_t j;
         unsigned char flags;
      };

      // the input strings
      DNA A;
      DNA B;

      // the score matrix
      size_t width;
      size_t height;
      MatrixCell ** matrix;

      // these hold the current progress of the algorithm
      stack<StackCell*> currentStack;
      char * currentA;
      char * currentB;
      size_t currentIndex;

      // initialize the matrix
      void _fill();

      // get alignment from initialized matrix
      void _traceBack();

      T matchScore;
      T gapScore;
      T misMatchScore;

      // get similarity between sequences at given index
      T similarity(size_t i, size_t j);

      void deleteMatrix();
      void deleteStack();

   public:
      DPM(DNA a, DNA b);
      DPM();
     ~DPM();
      void setMatchScore(T val);
      void setMisMatchScore(T val);
      void setGapPenalty(T val);

      void align();

      // directional flags for matrixCell;
      static const unsigned char VERTICAL   = 1 << 0;
      static const unsigned char DIAGONAL   = 1 << 1;
      static const unsigned char HORIZONTAL = 1 << 2;

      // traceback flags
      static const unsigned char VISITED    = 1 << 3;
      static const unsigned char V_GAP      = 1 << 4;
      static const unsigned char H_GAP      = 1 << 5;
};

#endif
