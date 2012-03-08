#ifndef DPM_H_
#define DPM_H_

#include "DNA.hpp"

#include <stack>
using std::stack;

#include <iterator>
using std::iterator;
using std::input_iterator_tag;


template <typename T>
class DPM{

      // a small class that wraps alignments. These are returned by DPM::Iterator
      // and handle the printing functionality. 
      // TODO - info about match types could be added here to extend to fancier
      // printing modes, such as ANSI or HTML
      class Alignment{
            char * a;
            char * b;
            Alignment(char * a, char * b);
         public:
            Alignment();
            void print();
      };

      // an iterator that can be used to get successive alignment objects
      // as the results of the algorithm
      class Iterator : public iterator<input_iterator_tag, Alignment>{
            // a reference to the DPM object that this iterator
            // refers to
            DPM & parent;
            // a flag that is set when post-increment is used.
            // it indicates that the next time the alignment this
            // object points to is requested,
            // the iterator should increment
            bool updateBeforeAccess;
            Iterator(DPM<T> & parent);
         public:
            Iterator();
            bool operator==(const DPM<T>::Iterator & other);
            bool operator!=(const DPM<T>::Iterator & other);
            Alignment & operator*();
            DPM<T>::Iterator& operator++();// {++p;return *this;}
            DPM<T>::Iterator  operator++(int);// {myiterator tmp(*this); operator++(); return tmp;}
      };

      struct MatrixCell{
         T score;
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
      Alignment currentAlignment;
      Iterator  iter;

      // initialize the matrix
      void _fill();

      // get alignment from initialized matrix
      void _traceBack();

      T matchScore;
      T gapScore;
      T misMatchScore;
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
      void next();

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
