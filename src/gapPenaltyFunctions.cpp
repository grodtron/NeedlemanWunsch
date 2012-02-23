
/////////////////////////////////////////////////////////
//   Gap Penalty Functions                             //
/////////////////////////////////////////////////////////

// all alignment gaps cost the same, no matter their length.
int constantGapPenalty(int gapLength){
   return gapLength == 1 ? -3 : 0;
}

// Small gaps are expensive, but do not increase much in
// price as their length increases. Penalties returned are:
int affineGapPenalty(int gapLength){
   if(gapLength == 0) return -4;
   return -(3/gapLength);
}

// gap price grows linearly with its length
int linearGapPenalty(int gapLength){
   return -3;
}

int zeroGapPenalty(int gapLength){
   return 0;
}
