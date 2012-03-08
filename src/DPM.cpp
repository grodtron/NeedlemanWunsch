#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <algorithm>
using std::copy;

#include "../include/DPM.hpp"

// little util
template <typename T> T max(T a, T b, T c){
   return (a > b) ? (a > c ? a : c) : (b > c ? b : c);
}

/////////////////////////////////////////////
//                                         //
//                Private                  //
//                                         //
/////////////////////////////////////////////

template <typename T>
void DPM<T>::_fill(){

   bool notYetAllocated = false;
   // allocate matrix if it hasn't been already
   if(!matrix){
      matrix = new DPM<T>::MatrixCell*[width];
      notYetAllocated = true;
   }

   for(size_t i = 0; i < width; ++i){

      // if the matrix hasn't already been allocated, then we need
      // to allocate space for each column
      if (notYetAllocated){
         matrix[i] = new DPM<T>::MatrixCell[height];
      }

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
            T dScore = dCell.score + similarity(i, j);
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
   while(currentStack.size()){

      DPM<T>::StackCell & currentC = currentStack.top();
      DPM<T>::MatrixCell  currentM = matrix[currentC.i][currentC.j];
      DPM<T>::StackCell   child;

      if(currentC.flags & DPM::VISITED){
         currentStack.pop();
         --index;
      }else{
         // TODO - copy strings into some kind of Alignment struct/class BACKWARDS
         // starting from the NULL terminator, up to BUT NOT INCLUDING
         // the terminating NULL
         a[index] = ((currentC.flags & DPM<T>::H_GAP) ? '-' : A[currentC.i]);
         a[index] = ((currentC.flags & DPM<T>::V_GAP) ? '-' : B[currentC.j]);
         ++index;
         currentC.flags |= DPM::VISITED;

         if(currentM.direction & (DPM<T>::VERTICAL|DPM<T>::HORIZONTAL|DPM<T>::DIAGONAL)){
            if(currentM.direction & DPM<T>::VERTICAL){
               child = DPM<T>::StackCell();
               child.i = currentC.i;
               child.j = currentC.j - 1;
               child.flags = DPM<T>::H_GAP;
               currentStack.push(child);
            }
            if(currentM.direction & DPM<T>::HORIZONTAL){
               child = DPM<T>::StackCell();
               child.i = currentC.i - 1;
               child.j = currentC.j;
               child.flags = DPM<T>::V_GAP;
               currentStack.push(child);
            }
            if(currentM.direction & DPM<T>::DIAGONAL){
               child = DPM<T>::StackCell();
               child.i = currentC.i - 1;
               child.j = currentC.j - 1;
               child.flags = 0;
               currentStack.push(child);
            }
         }else{
            // todo - copy backwards from here into struct and
            // add to struct or some kind of something
            // a[index + 1] = '\0';
            // a[index + 1] = '\0';
            return;
         }
      }
   }
   // if we got to here, then there are no more paths.
   // set everything to null.
   // TODO - end iteration
   *a = '\0';
   *b = '\0';
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
// (in case it wasn't obvious)
template <typename T>
DPM<T>::DPM(DNA a, DNA b)
: A(a), B(b), matrix(NULL), matchScore(3), gapScore(-1), misMatchScore(-1)
{
   width  = A.size();
   height = B.size();
}

template <typename T>
DPM<T>::DPM()
: width(0), height(0), matrix(NULL)
{
}

// just have to make sure that the matrix is freed
template <typename T>
DPM<T>::~DPM(){
   deleteMatrix();
}

template <typename T>
void DPM<T>::deleteMatrix(){
   if(matrix){
      for(size_t i = 0; i < width; ++i){
         delete[] matrix[i];
         matrix[i] = NULL;
      }
      delete[] matrix;
   }
   matrix = NULL;
}

template <typename T>
void DPM<T>::align(){
   _fill();
}

/*
template <typename T>
void DPM<T>::next(){
   // TODO - define an iterator, and possibly a some kind of Match struct
   // and use them for this functionality
   //_traceBack();
   //cout << (currentA+2) << endl
   //     << (currentB+2) << endl;
}
*/
///////////////////////////////////////////////////////////////
//                                                           //
//                        Iterator                           //
//                                                           //
///////////////////////////////////////////////////////////////



template <typename T>
DPM<T>::Iterator::Iterator() : parent(NULL) {}

template <typename T>
DPM<T>::Iterator::Iterator(const DPM<T> & parent) : parent(parent) {}

template <typename T>
bool DPM<T>::Iterator::operator==(const DPM<T>::Iterator & other){ return other.parent == parent; }

template <typename T>
bool DPM<T>::Iterator::operator!=(const DPM<T>::Iterator & other){ return other.parent != parent; }

///*
// de-reference
template <typename T>
typename DPM<T>::Alignment DPM<T>::Iterator::operator* (){
   if(!parent.currentAlignment || incrementBeforeAccess ){
      parent._traceBack(currentStack, a, b, index);
      incrementBeforeAccess = false;
   }
   return parent->currentAlignment;
}

// pre-increment
template <typename T>
typename DPM<T>::Iterator & DPM<T>::Iterator::operator++ (){
   if(incrementBeforeAccess){
      parent._traceBack(currentStack, a, b, index);
      incrementBeforeAccess = false;
   }
   parent._traceBack(currentStack, a, b, index);
   return *this;
}
//*/
// post-increment
template <typename T>
typename DPM<T>::Iterator & DPM<T>::Iterator::operator++ (int){
   if(incrementBeforeAccess){
      parent._traceBack(currentStack, a, b, index);
   }
   incrementBeforeAccess = true;
   return *this;
}
//*/

