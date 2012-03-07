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

   currentIndex = 0;

   if(currentA){ delete[] currentA; }
   if(currentB){ delete[] currentB; }

   // this is as long as it can possibly be
   currentA = new char[width + height];
   currentB = new char[width + height];

   deleteStack();
   currentStack.push(new DPM<T>::StackCell);
   currentStack.top()->i = width  - 1;
   currentStack.top()->j = height - 1;
   currentStack.top()->flags = 0;


   // TODO - figure if this is the right place for this and also
   // alloc space for currentA and currentB

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
void DPM<T>::_traceBack(){
   // This function follows the directions at each matrix cell
   // since all possible alignments are equally scoring, we will
   // favor the ones that are most diagonal first, and the ones
   // that tend towards adding gaps to the shorter of the two
   // strings second
   //
   // stackStack - use a stl::stack - it's easier
   //
   // struct stackCell{
   //    size_t i;
   //    size_t j;
   //    unsigned char flags;
   // }

   while(currentStack.size()){

      DPM<T>::StackCell * currentC = currentStack.top();
      DPM<T>::MatrixCell  currentM = matrix[currentC->i][currentC->j];
      DPM<T>::StackCell * child;

      if(currentC->flags & DPM::VISITED){
         currentStack.pop();
         --currentIndex;
      }else{
         ++currentIndex;
         currentA[currentIndex] = ((currentC->flags & DPM<T>::H_GAP) ? '-' : A[currentC->i]);
         currentB[currentIndex] = ((currentC->flags & DPM<T>::V_GAP) ? '-' : B[currentC->j]);
         currentC->flags |= DPM::VISITED;

         if(currentM.direction & (DPM<T>::VERTICAL|DPM<T>::HORIZONTAL|DPM<T>::DIAGONAL)){
            if(currentM.direction & DPM<T>::VERTICAL){
               child = new DPM<T>::StackCell;
               child->i = currentC->i;
               child->j = currentC->j - 1;
               child->flags = DPM<T>::H_GAP;
               currentStack.push(child);
            }
            if(currentM.direction & DPM<T>::HORIZONTAL){
               child = new DPM<T>::StackCell;
               child->i = currentC->i - 1;
               child->j = currentC->j;
               child->flags = DPM<T>::V_GAP;
               currentStack.push(child);
            }
            if(currentM.direction & DPM<T>::DIAGONAL){
               child = new DPM<T>::StackCell;
               child->i = currentC->i - 1;
               child->j = currentC->j - 1;
               child->flags = 0;
               currentStack.push(child);
            }
         }else{
            currentA[currentIndex + 1] = '\0';
            currentB[currentIndex + 1] = '\0';
            return;
         }
      }
   }
   // if we got to here, then there are no more paths.
   // set everything to null.
   *currentA = '\0';
   *currentB = '\0';
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
: A(a), B(b), matrix(NULL), currentA(NULL), currentB(NULL), matchScore(3), gapScore(-1), misMatchScore(-1)
{
   width  = A.size();
   height = B.size();
}

template <typename T>
DPM<T>::DPM()
: width(0), height(0), currentA(NULL), currentB(NULL), matrix(NULL)
{
}

// just have to make sure that the matrix is freed
template <typename T>
DPM<T>::~DPM(){
   deleteMatrix();
   deleteStack();
}

template <typename T>
void DPM<T>::deleteStack(){
   while(currentStack.size()){
      delete currentStack.top();
      currentStack.pop();
   }
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

template <typename T>
void DPM<T>::next(){
   _traceBack();
   cout << (currentA+2) << endl
        << (currentB+2) << endl;
}

