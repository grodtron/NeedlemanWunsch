#ifndef gapPenaltyFunctions_H_
#define gapPenaltyFunctions_H_

// all gaps cost the same, no matter their length.
int constantGapPenalty(int gapLength);

// Small gaps are expensive, but do not increase much in
int affineGapPenalty(int gapLength);

// gap price grows linearly with its length
int linearGapPenalty(int gapLength);

// all gaps are free (zero penalty)
int zeroGapPenalty(int gapLength);

#endif
