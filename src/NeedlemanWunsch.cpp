#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>
using std::copy;

#include <deque>
using std::deque;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

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
This is some garbage incase __GNUC__ is less than four to make g++ barf
#endif

typedef vector< pair<string, string> > stringPairVector; 
typedef map< pair<int, int>, stringPairVector* > svpCoordMap;

struct alignment{
   string A;
   string B;
};

int max(int x, int y, int z){
   return (x > y) ? (x > z ? x : z) : (y > z ? y : z);
}

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      // the outputted aligned strings
      stringPairVector * alignments;
      svpCoordMap knownNodes;

      // the F matrix
      int width;
      int height;
      vector< vector<int>* >* F;

      // initialize the F matrix
      void _init();

      // align the strings
      stringPairVector * _fullAlign(int, int);

      // get similarity between two chars
      int similarity(char a, char b);

      int (*similarityFunction)(char a, char b);

   public:
      NeedlemanWunsch(string a, string b);
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

void copyAndAppend(stringPairVector * from, stringPairVector * to, char appendFirst, char appendSecond){


   // allocate space for the new elements
   int increase = from->size();
   to->resize(to->size() + increase);

   // get the position within the vecotr that the new insertions
   // will start at
   stringPairVector::iterator it = to->end() - increase;

   // insert from the $from vector into the $to vector, starting
   // at the end position
   // TODO - is this a shallow copy or a deep copy?
   copy(from->begin(), from->end(), it);

   // get the new end point of the $to vector
   stringPairVector::iterator end= to->end();

   for(; it != end; ++it){
      it->first .push_back(appendFirst );
      it->second.push_back(appendSecond);
   }
}

stringPairVector * NeedlemanWunsch::_fullAlign(int i, int j){

   stringPairVector * strings = new stringPairVector;

   stringPairVector * tempStrings =  NULL;

   pair< svpCoordMap::iterator, bool> ret = 
      knownNodes.insert( pair<pair<int,int>,stringPairVector*>(pair<int, int>(i, j), strings) );

   if(ret.second){
      cout << "New node inserted " << i << ' ' << j << endl;
   }else{
      cout << "\tDuplicate node not inserted " << i << ' ' << j << endl;
      delete strings;
      return ret.first->second;
   }

   if(i > 0 && F->at(i-1)->at(j) == F->at(i)->at(j) ){
      tempStrings = _fullAlign(i-1, j);
      copyAndAppend(tempStrings, strings, A[i], '-');
   }
   if(j > 0 && F->at(i)->at(j-1) == F->at(i)->at(j) ){
      tempStrings = _fullAlign(i, j-1);
      copyAndAppend(tempStrings, strings, '-', B[j]);
   }
   if(i > 0 && j > 0 && F->at(i-1)->at(j-1) + similarity(A[i], B[j]) == F->at(i)->at(j) ){
      tempStrings = _fullAlign(i, j-1);
      copyAndAppend(tempStrings, strings, A[i], B[j]);
   }

   if(i == 0 && j == 0){
      strings->push_back( pair<string, string>(A.substr(0, 1), B.substr(0, 1)) );
   }

   return strings;
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
//
NeedlemanWunsch::NeedlemanWunsch(string a, string b)
: A(a), B(b), alignments(NULL), F(NULL), similarityFunction(NULL)
{
   width  = A.size();
   height = B.size();
   _init();
}

// print the whole thing
void NeedlemanWunsch::print(){
   if(alignments){

      stringPairVector::iterator it = alignments->begin();
      stringPairVector::iterator end= alignments->end();
      for(; it != end; ++it){
         cout << it->first  << endl << it->second << endl;
         cout << "================================" <<endl;
      }

   }else{
      cout << "Not yet aligned" << endl;
   }

}

int NeedlemanWunsch::getF(int i, int j){
   return F->at(i)->at(j);
}

void NeedlemanWunsch::setSimilarityFunction(int (*f)(char, char)){
   similarityFunction = f;
}

void NeedlemanWunsch::fullAlign(){

   // TODO proper destructor!
   //if(paths) delete paths;

   //paths = 
   alignments = _fullAlign(
      F->size() - 1,
      F->at(0)->size() - 1
   );

   print();
}

