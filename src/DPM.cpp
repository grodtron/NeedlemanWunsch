#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <string>
using std::string;

#include <algorithm>
using std::copy;

#include "../include/DPM.hpp"

// little util
// TODO - handle this properly
template <typename T> T max(T a, T b, T c){
   return (a > b) ? (a > c ? a : c) : (b > c ? b : c);
}

/////////////////////////////////////////////
//                                         //
//                Private                  //
//                                         //
/////////////////////////////////////////////

// Initialization
// This should only ever be called ONCE. There will be memory leaks otherwise
template <typename T>
void DPM<T>::_init(){

   // allocate the top level of the matrix
   matrix = new DPM<T>::MatrixCell*[width];

   for(size_t i = 0; i < width; ++i){

      // allocate the second level of teh matrix
      matrix[i] = new DPM<T>::MatrixCell[height];

      for(size_t j = 0; j < height; ++j){
         if(i == 0 && j == 0){
            // gap lengths, score and directions all zero
            // this is the cell that is the endpoint of the
            // matrix (top left corner)
            matrix[i][j] = {0,0};
         }else if(i == 0){
            // these are the cells along the left-hand vertical edge
            // there gap length is their position, and their score is
            // the previous score + the gapPenalty for their position
            // they have only one possible direction, which is vertical (up)
            matrix[0][j].score = matrix[0][j-1].score + gapScore;
            matrix[0][j].direction = VERTICAL;
         }else if(j == 0){
            // this is the same initialization as above, except for the cells along
            // the top horizontal edge
            matrix[i][0].score = matrix[i-1][0].score + gapScore;
            matrix[i][0].direction = HORIZONTAL;
         }else{

            // get the cells that we are working with
            DPM<T>::MatrixCell dCell = matrix[i-1][j-1];
            DPM<T>::MatrixCell hCell = matrix[i-1][j  ];
            DPM<T>::MatrixCell vCell = matrix[i  ][j-1];

            // "this cell". I'm OCD about name lengths being the same
            DPM<T>::MatrixCell * tCell = &matrix[i][j];

            // get the possible scores for this cell, keep
            // the max of the three possibilities
            // the offset is for the blank rows along the edges (huh?)
            T dScore = dCell.score + similarity(i - 1, j - 1);
            T hScore = hCell.score + gapScore;
            T vScore = vCell.score + gapScore;

            tCell->score = max(dScore, vScore, hScore);

            // Since it's possible for multiple directions to give
            // the same score, we check for all of the possibilities,
            // and update the direction flags of this cell, which are
            // used in the traceback phase.
            tCell->direction = 0;

            if(tCell->score == dScore){
               tCell->direction |= DIAGONAL;
            }
            if(tCell->score == vScore){
               tCell->direction |= VERTICAL;
            }
            if(tCell->score == hScore){
               tCell->direction |= HORIZONTAL;
            }
         }
      }
   }
}

template <typename T>
void DPM<T>::_traceBack( list<DPM<T>::StackCell> & currentStack, char * a, char * b, size_t & index ) const{
   while(!currentStack.empty()){

      DPM<T>::StackCell & currentC = currentStack.back();
      DPM<T>::MatrixCell  currentM = matrix[currentC.i][currentC.j];
      DPM<T>::StackCell   child;

      if(currentC.flags & DPM::VISITED){
         currentStack.pop_back();
         --index;
      }else{
         if(currentC.flags == DPM<T>::VERTICAL){
            a[index] = '-';
            b[index] = B[currentC.j];
            // incrementing should always be done inside these conditionals
            // otherwise we can increment without inserting a character
            // (which we don't want)
            ++index;
         }else if(currentC.flags == DPM<T>::HORIZONTAL){
            a[index] = A[currentC.i];
            b[index] = '-';
            ++index;
         }else if(currentC.flags == DPM<T>::DIAGONAL){
            a[index] = A[currentC.i];
            b[index] = B[currentC.j];
            ++index;
         }/*else{
            If there is a bug it could be from getting to this point
            shouldn't happen, the flags are currently set and defined properly
            but if something comes up, this could be a good place to look

            this comment was surprisingly helpful
         }*/
         currentC.flags |= DPM::VISITED;

         if(currentM.direction & (DPM<T>::VERTICAL|DPM<T>::HORIZONTAL|DPM<T>::DIAGONAL)){
            if(currentM.direction & DPM<T>::VERTICAL){
               child = DPM<T>::StackCell();
               child.i = currentC.i;
               child.j = currentC.j - 1;
               child.flags = DPM<T>::VERTICAL;
               currentStack.push_back(child);
            }
            if(currentM.direction & DPM<T>::HORIZONTAL){
               child = DPM<T>::StackCell();
               child.i = currentC.i - 1;
               child.j = currentC.j;
               child.flags = DPM<T>::HORIZONTAL;
               currentStack.push_back(child);
            }
            if(currentM.direction & DPM<T>::DIAGONAL){
               child = DPM<T>::StackCell();
               child.i = currentC.i - 1;
               child.j = currentC.j - 1;
               child.flags = DPM<T>::DIAGONAL;
               currentStack.push_back(child);
            }
         }else{
            return;
         }
      }
   }
   // if we got to here, then there are no more paths.
   // set everything to null.
   a[0] = '\0';
   b[0] = '\0';
   index = 0;
   return;
}

template <typename T>
T DPM<T>::similarity(size_t i, size_t j){
   return A[i] == B[j] ? matchScore : misMatchScore;
}

/////////////////////////////////////////////
//                                         //
//                Public                   //
//                                         //
/////////////////////////////////////////////

// constructor!
template <typename T>
DPM<T>::DPM(DNA a, DNA b)
: A(a), B(b), matrix(NULL), matchScore(3), gapScore(-1), misMatchScore(-1)
{
   // an extra row must be allocated otherwise the first character of the sequences will be truncated
   width  = A.size() + 1;
   height = B.size() + 1;
   _init();
}

template <typename T>
DPM<T>::~DPM(){
   for(size_t i = 0; i < width; ++i){
      delete[] matrix[i];
      matrix[i] = NULL;
   }
   delete[] matrix;
}

template <typename T>
typename DPM<T>::Iterator DPM<T>::begin(){
   // This function creates a new iterator that will yield
   // all of the possible alignments for the current DPM
   DPM<T>::Iterator it(*this);

   it.index = 0;
   it.flags = 0;

   // the stack acts as a bookmark, holding a spot within
   // the traceback phase of the algorithm. Starting with
   // only the first cell will cause every alignment to be
   // generated
   it.currentStack.push_back(DPM<T>::StackCell());
   it.currentStack.back().i = width - 1;
   it.currentStack.back().j = height - 1;
   it.currentStack.back().flags = 0;

   ++it;

   return it /* by value */;
}

template <typename T>
typename DPM<T>::Iterator DPM<T>::end(){
   DPM<T>::Iterator it(*this);

   // an empty stack means that every alignment has been found
   it.index = 0;
   it.currentStack.resize(0);

   // incrementing the empty iterator will cause all of the correct
   // flags to be set that indicate that it is at the end of iteration
   ++it;

   return it /* by value */;
}

///////////////////////////////////////////////////////////////
//                                                           //
//                        Iterator                           //
//                                                           //
///////////////////////////////////////////////////////////////

// Iterator constructor - associates the iterator with a DPM object, and initializes it to
// the right size
template <typename T>
DPM<T>::Iterator::Iterator(DPM<T> & parent)
: parent(parent), currentAlignment(DPM<T>::Alignment()),
  flags(0), index(0) {

   a = new char[parent.width + parent.height]; 
   b = new char[parent.width + parent.height]; 
   a[0] = '\0';
   b[0] = '\0';
}

// copy cosntructor
template <typename T>
DPM<T>::Iterator::Iterator(const DPM<T>::Iterator & other)
: parent(other.parent),
  currentAlignment (DPM<T>::Alignment(other.currentAlignment)), flags(other.flags),
  currentStack(list<DPM<T>::StackCell>(other.currentStack)), index(other.index)

{
   a = new char[parent.width + parent.height]; 
   b = new char[parent.width + parent.height]; 
   size_t i;
   // NB - iterator holds the string BACKWARDS!!
   for(i = 0; i < (parent.height + parent.width); ++i){
      a[i] = other.a[i];
      b[i] = other.b[i];
   }
}

template <typename T>
typename DPM<T>::Iterator & DPM<T>::Iterator::operator=(const DPM<T>::Iterator & other){
   parent = other.parent;
   flags = other.flags;
   index = other.index;
   currentAlignment = DPM<T>::Alignment(other.currentAlignment);
   currentStack = list<DPM<T>::StackCell>(other.currentStack);
   a = new char[parent.width + parent.height]; 
   b = new char[parent.width + parent.height]; 
   size_t i;
   // NB - iterator holds the string BACKWARDS!!
   for(i = 0; i < (parent.height + parent.width); ++i){
      a[i] = other.a[i];
      b[i] = other.b[i];
   }
   return *this;
}

template <typename T>
DPM<T>::Iterator::~Iterator(){
   if (a) delete [] a;
   if (b) delete [] b;
   a = NULL;
   b = NULL;
}

// first check flags. In most cases we're checking an iterator against end(), in which case the flags
// will differentiate them. If not, then we'll have to do a full string comparison between their alignment
// objects, which is somewhat expensive
template <typename T>
bool DPM<T>::Iterator::operator==(const DPM<T>::Iterator & other) const{
   return (flags == other.flags && currentAlignment == other.currentAlignment);
}

template <typename T>
bool DPM<T>::Iterator::operator!=(const DPM<T>::Iterator & other){
   return !operator==(other);
}

// TODO - verify that post/pre incrementing works as expected
// de-reference
template <typename T>
typename DPM<T>::Alignment DPM<T>::Iterator::operator* (){
   return currentAlignment;
}


// for reference this is how post/pre-increment is handled
//
//DPM<T>::Iterator & operator++();   {++p;return *this;}
//DPM<T>::Iterator   operator++(int) {myiterator tmp(*this); operator++(); return tmp;}


// pre-increment
template <typename T>
typename DPM<T>::Iterator & DPM<T>::Iterator::operator++ (){
   // get next alignment
   parent._traceBack(currentStack, a, b, index);

   // load it into an Alignment object
   currentAlignment = DPM<T>::Alignment(a, b, index);

   // check to see if we're at the end of the iteration (past the last alignment)
   // set flags if we are
   if(a[0] == '\0'){ flags = DPM<T>::Iterator::ITERATION_COMPLETE; }

   return *this;
}

// post-increment
// (return a temporary iterator by value)
template <typename T>
typename DPM<T>::Iterator  DPM<T>::Iterator::operator++ (int){
   Iterator tmp(*this);
   operator++();
   return tmp;
}

template <typename T>
DPM<T>::Alignment::Alignment() : a(NULL), b(NULL) { }

template <typename T>
DPM<T>::Alignment::Alignment(const DPM<T>::Alignment & other) {
   if(other.a){
      size_t len;
      for(len = 0; other.a[len] != '\0'; ++len);
      // len is 1 + the index of the NULL terminator
      ++len;

      a = new char[len];
      b = new char[len];

      for(size_t i = 0; i < len; ++i){
         a[i] = other.a[i];
         b[i] = other.b[i];
      }
   }else{
      a = NULL;
      b = NULL;
   }
}

template <typename T>
bool DPM<T>::Alignment::operator==(const DPM<T>::Alignment & other) const{

   // only checking a, because b should be the same, if it's not, then we have bigger
   // problems
   if(a == NULL){
      return other.a == NULL;
   }

   size_t i = 0;

   while(
      a[i] == other.a[i]
      &&
      b[i] == other.b[i]
   ){
      if( a[i] == '\0' ) return true;
      ++i;
   }

   return false;
}

template <typename T>
typename DPM<T>::Alignment & DPM<T>::Alignment::operator=(const DPM<T>::Alignment & other) {

   if (a) { delete[] a; }
   if (b) { delete[] b; }

   // If the other object has been initialized, then initialize this one and copy
   // everything over from 'other'
   if(other.a){

      size_t len;
      for(len = 0; other.a[len] != '\0'; ++len);
      ++len;

      a = new char[len];
      b = new char[len];

      for(size_t i = 0; i < len; ++i){
         a[i] = other.a[i];
         b[i] = other.b[i];
      }
   // if the other object has not been initialized, then set our pointers to NULL
   }else{
      a = NULL;
      b = NULL;
   }

   return *this /* by reference */;
}

template <typename T>
DPM<T>::Alignment::~Alignment(){
   if (a) delete[] a;
   if (b) delete[] b;
}

template <typename T>
DPM<T>::Alignment::Alignment(char * a, char * b, size_t len){
   if(len){
      this->a = new char[len + 1];
      this->b = new char[len + 1];
      for(size_t i = 0; i < len; ++i){
         // the sequences are copied in reverse. (len - 1) is the last index
         // NB - must use 'this->a', not just 'a' to avoid confusion with
         // the 'a' and 'b' parameters
         this->a[i] = a[(len - 1) - i];
         this->b[i] = b[(len - 1) - i];
      }
      // we have to add NULL terminators by hand, because they are not included
      // in the strings that we will be given here
      this->a[len] = '\0';
      this->b[len] = '\0';
   }else{
      this->a = NULL;
      this->b = NULL;
   }
}

// TODO - fancier printing methods (ANSI, maybe HTML)
// Also, pass an std::ostream for more flexibility
template <typename T>
void DPM<T>::Alignment::print(){
   cout << a << endl << b << endl;
}
