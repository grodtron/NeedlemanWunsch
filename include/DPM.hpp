#ifndef DPM_H_
#define DPM_H_

#include "DNA.hpp"

#include <list>
using std::list;

#include <iterator>
using std::iterator;
using std::input_iterator_tag;

class DPM_ImplementationBase;

class DPM{

      DPM_ImplementationBase * matrix;

   public:

      struct StackCell{
         size_t i;
         size_t j;
         unsigned char flags;
      };

      class  Alignment;
      class  Iterator;

      // a small class that wraps alignments. These are returned by DPM::Iterator
      // and handle the printing functionality. 
      // TODO - info about match types could be added here to extend to fancier
      // printing modes, such as ANSI or HTML
      class Alignment{
            friend class DPM::Iterator;
            char * a;
            char * b;
            Alignment(char * a, char * b, size_t len);
         public:
            Alignment();
            Alignment(const DPM::Alignment & other);
           ~Alignment();
            DPM::Alignment & operator=(const DPM::Alignment & other);
            bool operator==(const DPM::Alignment & other) const;
            void print();
      };

      // an iterator that can be used to get successive alignment objects
      // as the results of the algorithm
      class Iterator : public iterator<input_iterator_tag, Alignment>{
            friend class DPM;
            // a reference to the DPM object that this iterator
            // refers to
            DPM & parent;
            DPM::Alignment currentAlignment;
            // a flag that is set when post-increment is used.
            // it indicates that the next time the alignment this
            // object points to is requested,
            // the iterator should increment
            unsigned char flags;
            Iterator(DPM & parent);
            // these hold the current progress of the algorithm for this iterator
            list<DPM::StackCell> currentStack;
            char * a;
            char * b;
            size_t index;
         public:
           ~Iterator();
            Iterator(const DPM::Iterator & other);
            bool operator==(const DPM::Iterator & other) const;
            bool operator!=(const DPM::Iterator & other);
            //DPM::Alignment   operator*();
            DPM::Iterator & operator=(const DPM::Iterator & other);
            Alignment operator*();
            DPM::Iterator  & operator++();// {++p;return *this;}
            DPM::Iterator    operator++(int);// {myiterator tmp(*this); operator++(); return tmp;}
            static const unsigned char  INCREMENT_BEFORE_ACCESS = 1 << 0;
            static const unsigned char  ITERATION_COMPLETE      = 1 << 0;
      };

      DPM(DNA a, DNA b, int matchScore, int gapPenalty, int mismatchPenalty);
      DPM(DNA a, DNA b, double matchScore, double gapPenalty, double mismatchPenalty);
     ~DPM();

      Iterator begin();
      Iterator end();


      // traceback flags
      static const unsigned char VISITED    = 1 << 3;
};


#endif
