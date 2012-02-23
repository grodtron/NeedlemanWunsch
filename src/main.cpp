#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <string>
using std::string;

#include "../include/NeedlemanWunsch.h"
#include "../include/qwertyDistance.h"

/////////////////////////////////////////////////////////
//   Gap Penalty Functions                             //
/////////////////////////////////////////////////////////

// all alignment gaps cost the same, no matter their length.
// (-2)
int constantGapPenalty(int gapLength){
   return gapLength == 1 ? -4 : 0;
}

// Small alignments are expensive, but do not increase much in
// price as their length increases. Penalties returned are:
// 1 : -4
// 2 : -2
// 3 : -1
// 4 : -1
// 5 : 0
// 6 : 0
//   : 0
int affineGapPenalty(int gapLength){
   return -(3/gapLength);
}

// gap price grows linearly with its length
int linearGapPenalty(int gapLength){
   return -4;
}

int zeroGapPenalty(int gapLength){
   return 0;
}

int main(int argc, const char *argv[])
{

   string A, B;
   Alignment alignment;

   // get strings to aline from command line args, otherwise ask for them:
   if(argc == 3){
      A = argv[1];
      B = argv[2];
   }else if(argc == 1){
      cout << "Enter two strings to be aligned: " << endl << endl;
      cin >> A >> B;
   }else{
      cout << "Received bad args (got " << argc-1 << ", should be 2 or 0)." << endl;
      return 1;
   }
   
   NeedlemanWunsch nw(A, B);
   nw.setSimilarityFunction(qwertyDistanceSimilarity);

   nw.setGapPenaltyFunction(linearGapPenalty);
   nw.align(alignment);
   cout << endl << "Alignment using linear penalty:" << endl;
   alignment.print();

   nw.setGapPenaltyFunction(affineGapPenalty);
   nw.align(alignment);
   cout << endl << "Alignment using affine penalty:" << endl;
   alignment.print();

   nw.setGapPenaltyFunction(constantGapPenalty);
   nw.align(alignment);
   cout << endl << "Alignment using constant penalty:" << endl;
   alignment.print();

   nw.setGapPenaltyFunction(zeroGapPenalty);
   nw.align(alignment);
   cout << endl << "Alignment using zero penalty:" << endl;
   alignment.print();
   
   return 0;
}
