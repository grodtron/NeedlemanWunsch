#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>
using std::copy;

#include <deque>
using std::deque;

#include <vector>
using std::vector;

#include <string>
using std::string;

#if __GNUC__ == 4
#if __GNUC_MINOR__ < 3
#include <tr1/memory>
using std::tr1::shared_ptr;
#else
#include <memory>
using std::shared_ptr;
#endif
#else
This is some garbage incase __GNUC__ < four
#endif

struct alignment{
   string * A;
   string * B;
};

struct tree{
   shared_ptr<struct tree> left;
   shared_ptr<struct tree> center;
   shared_ptr<struct tree> right;
   int i;
   int j;
};

// utils

int max(int x, int y, int z){
   return (x > y) ? (x > z ? x : z) : (y > z ? y : z);
}

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      string alignedA;
      string alignedB;

      vector<alignment*> alignments;
      tree* paths;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();
      // align the strings
      void  _align();
      tree* _fullAlign(deque<char>*, deque<char>*, int, int);
      // get similarity between two chars
      int  similarity(char a, char b);

      int (*similarityFunction)(char a, char b);

   public:
      NeedlemanWunsch(string & a, string & b);
      NeedlemanWunsch(string * a, string * b);
      void print();
      int getF(int i, int j);
      void setSimilarityFunction(int (*f)(char, char));
      void align();
      void fullAlign();
};

/////////////////////////////////////////////
//                                         //
//                Private                  //
//                                         //
/////////////////////////////////////////////

void NeedlemanWunsch::_init(){

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

// go through the matrix and pick the first
// path (instead of recursing through the
// whole thing)
void NeedlemanWunsch::_align(){
   int i = width  - 1;
   int j = height - 1;

   while(1){

      int val = F->at(i)->at(j);
      if(i > 0 && j > 0 && val == F->at(i-1)->at(j-1) + similarity(A[i],B[j])){
         //cout << "A += " << A[i] << ", B += " << B[j] << endl;
         alignedA.insert(alignedA.begin(), A[i]);
         alignedB.insert(alignedB.begin(), B[j]);
         j -= 1;
         i -= 1;

      }else if(i > 0 && val == F->at(i-1)->at(j) ){
         //cout << "A += " << A[i] << ", B += -" << endl;
         alignedA.insert(alignedA.begin(), A[i]);
         alignedB.insert(alignedB.begin(),  '-');
         i -= 1;

      }else if(j > 0 && val == F->at(i)->at(j-1) ){
         //cout << "A += -, B += " << B[j] << endl;
         alignedA.insert(alignedA.begin(),  '-');
         alignedB.insert(alignedB.begin(), B[j]);
         j -= 1;

      }else{
         break;

      }
   }

   //cout << "A += " << A[i] << ", B += " << B[j] << endl;
   alignedA.insert(alignedA.begin(), A[i]);
   alignedB.insert(alignedB.begin(), B[j]);
}

tree* NeedlemanWunsch::_fullAlign(
   deque<char> * ancestryA,
   deque<char> * ancestryB,
   int i, int j
){

   // construct Tree. I think this is only gonna be for
   // visualization
   tree* root     = new tree;
   root->i        = i;
   root->j        = j;
   // Autoset to Null by shared_ptr (I assume)
   //root->left     = NULL;
   //root->right    = NULL;
   //root->center   = NULL;

   if(i > 0 && F->at(i-1)->at(j) == F->at(i)->at(j) ){
      ancestryA->push_back(A[i]);// no gap
      ancestryB->push_back('-'); // gap
      root->left.reset(_fullAlign(ancestryA, ancestryB, i-1, j));
      ancestryA->pop_back();
      ancestryB->pop_back();
   }
   if(j > 0 && F->at(i)->at(j-1) == F->at(i)->at(j) ){
      ancestryA->push_back('-'); // gap
      ancestryB->push_back(B[j]);// no gap
      root->right.reset(_fullAlign(ancestryA, ancestryB, i, j-1));
      ancestryA->pop_back();
      ancestryB->pop_back();
   }
   if(i > 0 && j > 0 && F->at(i-1)->at(j-1) + similarity(A[i], B[j]) == F->at(i)->at(j) ){
      ancestryA->push_back(A[i]);// no gap
      ancestryB->push_back(B[j]);// no gap
      root->center.reset(_fullAlign(ancestryA, ancestryB, i-1, j-1));
      ancestryA->pop_back();
      ancestryB->pop_back();
   }

   // this means we're at a leaf node
   // copy the current deques into the alignment
   // vector to save them
   if(i == 0 && j == 0){
      alignment * al = new alignment;

      // ancestryA and ancestryB should be 
      // equal, so this is kind of just incase
      al->A = new string;
      al->B = new string;

      al->A->reserve(ancestryA->size());
      al->B->reserve(ancestryB->size());

      // copy the current deque into a new alignment struct
      copy(ancestryA->begin(), ancestryA->end(), al->A->begin());
      copy(ancestryB->begin(), ancestryB->end(), al->B->begin());

      alignments.push_back(al);
   }

   return root;
}

int NeedlemanWunsch::similarity(char a, char b){
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
NeedlemanWunsch::NeedlemanWunsch(string & a, string & b)
: A(a), B(b), alignedA(""), alignedB(""), paths(NULL), F(NULL), similarityFunction(NULL)
{
   width  = A.size();
   height = B.size();
   _init();
   _align();
}

// constructor!
//
NeedlemanWunsch::NeedlemanWunsch(string * a, string * b)
: A(*a), B(*b), alignedA(""), alignedB(""), paths(NULL), F(NULL), similarityFunction(NULL)
{
   width  = A.size();
   height = B.size();
   _init();
   _align();
}

// print the whole thing
void NeedlemanWunsch::print(){
   if(&alignments){
      cout << *(alignments[0]->A) << endl << *(alignments[0]->B) << endl;
   }else{
      cout << endl << "Aligned strings:" << endl << alignedA << endl << alignedB << endl;
   }
}

int NeedlemanWunsch::getF(int i, int j){
   return F->at(i)->at(j);
}

void NeedlemanWunsch::setSimilarityFunction(int (*f)(char, char)){
   similarityFunction = f;
}

void NeedlemanWunsch::align(){
   alignedA = "";
   alignedB = "";
   _init();
   _align();
}

void NeedlemanWunsch::fullAlign(){

   deque<char> * ancestryA = new deque<char>();
   deque<char> * ancestryB = new deque<char>();

   // TODO proper destructor!
   if(paths) delete paths;

   paths = _fullAlign(
      ancestryA,
      ancestryB,
      F->size() - 1,
      F->at(0)->size() - 1
   );

   delete ancestryA;
   delete ancestryB;
}

