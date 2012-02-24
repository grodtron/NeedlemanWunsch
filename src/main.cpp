#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <algorithm>
using std::sort;

#include "../include/NeedlemanWunsch.h"
#include "../include/qwertyDistance.h"
#include "../include/gapPenaltyFunctions.h"

int main(int argc, const char *argv[])
{

   vector<Alignment> alignments;
   Alignment alignment;

   vector<string> sentences;
   char sentence[1024];

   // create the NeedlemanWunsch object
   NeedlemanWunsch nw;
   nw.setSimilarityFunction(qwertyDistanceSimilarity);
   nw.setGapPenaltyFunction(affineGapPenalty);

   // setup the input stream
   if(argc == 1){
      cerr << "Not enough args - need one arg for input file name" << endl;
      return 1;
   }
   ifstream input(argv[1]);

   // read lines into vector
   while(!input.eof()){
      input.getline(sentence, 1024);
      if(sentence[0] == '\0'){
         continue;
      }
      sentences.push_back(string(sentence));
   }

   // contain it and end within this block
   cerr << "working... ";
   {
      // run through and align each string with each other string.
      vector<string>::iterator it = sentences.begin();
      vector<string>::iterator end = sentences.end();
      for(;it != end; ++it){
         vector<string>::iterator jt = sentences.begin();
         for(;jt != it; ++jt){
            nw.setStrings(*it, *jt);
            nw.align(alignment);
            alignments.push_back(alignment);
         }
      }
   }
   cerr << "done!" << endl;

   sort(alignments.begin(), alignments.end());

   if(!alignments.size()){
      cerr << "Empty input file" << endl;
      return 0;
   }

   vector<Alignment>::reverse_iterator it = alignments.rbegin();
   vector<Alignment>::reverse_iterator end = alignments.rend();

   Alignment::printHeader(cout, Alignment::HTML);

   for(; it != end; ++it){
      it->print(cout, Alignment::HTML);
      it->print(cerr, Alignment::CONSOLE);
   }
   
   return 0;
}
