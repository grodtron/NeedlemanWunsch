#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>
using std::copy;

#include <deque>
using std::deque;

#include <vector>
using std::vector;

#include <utility>
using std::pair;

#include <string>
using std::string;

#include "../include/utils.h"
// max(int, int, int)

struct alignment{
   string A;
   string B;
};

class OldNeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      vector<alignment*> alignments;
      tree* paths;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();

      // align the strings
      void _fullAlign(deque<char>*, deque<char>*, int, int);

      // get similarity between two chars
      int similarity(char a, char b);

      int (*similarityFunction)(char a, char b);

   public:
      OldNeedlemanWunsch(string a, string b);
      void print();
      int getF(int i, int j);
      void setSimilarityFunction(int (*f)(char, char));
      void fullAlign();
};

/////////////////////////////////////////////
//                                         //
//                Private                  //
//                                         //
/////////////////////////////////////////////

void OldNeedlemanWunsch::_init(){

   delete F;
   F = NULL;

   F = new vector< vector<int>* >(width);

   for(int i = 0; i < width; ++i){
      F->at(i) = new vector<int>(height);
      for(int j = 0; j < height; ++j){
         if(i == 0 || j == 0){
            F->at(i)->at(j) = 0;
         }else{
            F->at(i)->at(j) = max(
               F->at(i-1)->at(j-1) + similarity(A[i], B[j]),
               F->at(i-1)->at(j  ),
               F->at(i  )->at(j-1)
            );
         }
      }
   }
}

void OldNeedlemanWunsch::_fullAlign(
   deque<char> * ancestryA,
   deque<char> * ancestryB,
   int i, int j
){

   int score = F->at(i)->at(j);

   if(i > 0 && F->at(i-1)->at(j) == score ){
      ancestryA->push_back(A[i]);// no gap
      ancestryB->push_back('-'); // gap
      _fullAlign(ancestryA, ancestryB, i-1, j);
      ancestryA->pop_back();
      ancestryB->pop_back();
   }
   if(j > 0 && F->at(i)->at(j-1) == score ){
      ancestryA->push_back('-'); // gap
      ancestryB->push_back(B[j]);// no gap
      _fullAlign(ancestryA, ancestryB, i, j-1);
      ancestryA->pop_back();
      ancestryB->pop_back();
   }
   if(i > 0 && j > 0 && F->at(i-1)->at(j-1) + similarity(A[i], B[j]) == score ){
      ancestryA->push_back(A[i]);// no gap
      ancestryB->push_back(B[j]);// no gap
      _fullAlign(ancestryA, ancestryB, i-1, j-1);
      ancestryA->pop_back();
      ancestryB->pop_back();
   }

   // this means we're at a leaf node
   // copy the current deques into the alignment
   // vector to save them
   if(i == 0 && j == 0){
      alignment * al = new alignment;

      // push back the current alignment

      al->A.assign(ancestryA->begin(), ancestryA->end());
      al->B.assign(ancestryB->begin(), ancestryB->end());

      alignments.push_back(al);
   }

   return root;
}

int OldNeedlemanWunsch::similarity(char a, char b){
   if(similarityFunction){
      return similarityFunction(a, b);
   }
   return a == b ? 3 : -1;
}

/////////////////////////////////////////////
//                                         //
//                Public                   //
//                                         //
/////////////////////////////////////////////

// constructor!
//
OldNeedlemanWunsch::OldNeedlemanWunsch(string a, string b)
: A(a), B(b), paths(NULL), F(NULL), similarityFunction(NULL)
{
   width  = A.size();
   height = B.size();
   _init();
}

OldNeedlemanWunsch::~OldNeedlemanWunsch(){

}

// print the whole thing
void OldNeedlemanWunsch::print(){
   if(&alignments){

      vector<alignment*>::iterator it = alignments.begin();
      vector<alignment*>::iterator end= alignments.end();
      for(; it != end; ++it){
         cout << (*it)->A  << endl << (*it)->B << endl;
         cout << "================================" <<endl;
      }

   }else{
      cout << "Not yet aligned" << endl;
   }

}

int OldNeedlemanWunsch::getF(int i, int j){
   return F->at(i)->at(j);
}

void OldNeedlemanWunsch::setSimilarityFunction(int (*f)(char, char)){
   similarityFunction = f;
}

void OldNeedlemanWunsch::fullAlign(){

   deque<char> * ancestryA = new deque<char>();
   deque<char> * ancestryB = new deque<char>();

   _fullAlign(
      ancestryA,
      ancestryB,
      F->size() - 1,
      F->at(0)->size() - 1
   );

   delete ancestryA;
   delete ancestryB;
}

