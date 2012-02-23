#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "../include/NeedlemanWunsch.h"
#include "../include/Alignment.h"

const char Alignment::MATCH = 'M';
const char Alignment::MISMATCH = 'm';
const char Alignment::GAP = 'g';


bool operator< (const Alignment & a, const Alignment & b){
   return a.score < b.score;
}

bool operator> (const Alignment & a, const Alignment & b){
   return a.score > b.score;
}

// print the whole thing
void Alignment::print() const {
   // have to print the strings char by char using reverse iterators
   // because the alignment process produces them backwards
   cout << "Score: " << score << endl;

   int size = A.size();
   for(int i = 0; i < size; ++i){
      if(matchType[i] == MATCH){
         cout << "\033[1;102;90m"; // bold, Green bg, grey fg
      }else if(matchType[i] == MISMATCH){
         cout << "\033[1;91m";     // bold, red
      }else{
         cout << "\033[90m";       // grey
      }
      cout.put(A[i]);
      cout << "\033[0m"; // reset
   }
   cout << endl;
   for(int i = 0; i < size; ++i){
      if(matchType[i] == MATCH){
         cout << "\033[1;102;90m"; // bold, Green bg, grey fg
      }else if(matchType[i] == MISMATCH){
         cout << "\033[1;91m";     // bold, red
      }else{
         cout << "\033[90m";       // grey
      }
      cout.put(B[i]);
      cout << "\033[0m"; // reset
   }
   cout << endl;
}
