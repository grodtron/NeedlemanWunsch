#ifndef DPM_H_
#define DPM_H_

#include "DNA.hpp"

#include <list>
using std::list;

#include <iterator>
using std::iterator;
using std::input_iterator_tag;


template <typename T>
class DPM{

      struct MatrixCell;
      struct StackCell;

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

      // initialize the matrix
      void _fill();

      // get alignment from initialized matrix
      void _traceBack(list<StackCell> & current, char * a, char * b, size_t & index) const;

      T matchScore;
      T gapScore;
      T misMatchScore;
      T similarity(size_t i, size_t j);

      void deleteMatrix();
   public:
      class  Alignment;
      class  Iterator;

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
            friend class DPM<T>;
            // a reference to the DPM object that this iterator
            // refers to
            const DPM<T> & parent;
            // a flag that is set when post-increment is used.
            // it indicates that the next time the alignment this
            // object points to is requested,
            // the iterator should increment
            bool incrementBeforeAccess;
            Iterator(const DPM<T> & parent);
            // these hold the current progress of the algorithm for this iterator
            list<DPM<T>::StackCell> currentStack;
            char * a;
            char * b;
            size_t index;
         public:
            Iterator();
            bool operator==(const DPM<T>::Iterator & other);
            bool operator!=(const DPM<T>::Iterator & other);
            //DPM<T>::Alignment   operator*();
            Alignment   operator*();
            DPM<T>::Iterator  & operator++();// {++p;return *this;}
            DPM<T>::Iterator  & operator++(int);// {myiterator tmp(*this); operator++(); return tmp;}
      };

      DPM(DNA a, DNA b);
      DPM();
     ~DPM();
      void setMatchScore(T val);
      void setMisMatchScore(T val);
      void setGapPenalty(T val);

      void align();

      Iterator begin();
      Iterator end();

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
