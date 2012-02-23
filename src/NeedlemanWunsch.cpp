#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <algorithm>
using std::copy;

#include "../include/utils.cpp"
// provides these two functions:
// template max(x, y, z)
// template swap(a, b)

const char MATCH = 'M';
const char MISMATCH = 'm';
const char GAP = 'g';

// directional flags for matrixCell;
const unsigned char VERTICAL   = 1 << 0;
const unsigned char DIAGONAL   = 1 << 1;
const unsigned char HORIZONTAL = 1 << 2;

// This struct represents each cell in the score matrix
// and holds traceback information as well
struct matrixCell{
   // the score at this point inside the matrix
   int score;
   // a set of flags indicating the possible directions
   // for this position in the matrix
   unsigned char direction;
   // the length of the horizontal gap that leads to this
   // cell. This is used for calculating constant and affine
   // gap penalties
   int hGapLen;
   // same as hGapLen, except for vertical gaps
   int vGapLen;
};

class NeedlemanWunsch;

class Alignment{
      friend class NeedlemanWunsch;
      string A;
      string B;
      string matchType;
      int score;
   public:
      void print() const;
};

class NeedlemanWunsch{

      // the input strings
      string A;
      string B;

      Alignment alignment;

      // the score matrix
      int width;
      int height;
      matrixCell ** matrix;

      // initialize the matrix
      void _init();

      // get alignment from initialized matrix
      void _traceBack();

      // get similarity between two chars
      int similarity(char a, char b);
      int (*similarityFunction)(char a, char b);
      
      int gapPenalty(int gapLength);
      int (*gapPenaltyFunction)(int gapLength);

   public:
      NeedlemanWunsch(string a, string b);
      NeedlemanWunsch();
     ~NeedlemanWunsch();
      void setSimilarityFunction(int (*f)(char, char));
      void setGapPenaltyFunction(int (*f)(int));
      void setStrings(string a, string b);
      void align(Alignment &);
};

/////////////////////////////////////////////
//                                         //
//                Private                  //
//                                         //
/////////////////////////////////////////////

void NeedlemanWunsch::_init(){

   bool notYetAllocated = false;
   // allocate matrix if it hasn't been already
   if(!matrix){
      matrix = new matrixCell*[width];
      notYetAllocated = true;
   }

   for(int i = 0; i < width; ++i){

      // if the matrix hasn't already been allocated, then we need
      // to allocate space for each column
      if (notYetAllocated){
         matrix[i] = new matrixCell[height];
      }

      for(int j = 0; j < height; ++j){
         if(i == 0 && j == 0){
            // gap lengths, score and directions all zero
            // this is the cell that is the endpoint of the
            // matrix (top left corner)
            matrix[i][j] = {0,0,0,0};
         }else if(i == 0){
            // these are the cells along the left-hand vertical edge
            // there gap length is their position, and their score is
            // the previous score + the gapPenalty for their position
            // they have only one possible direction, which is vertical (up)
            matrix[0][j].score = matrix[0][j-1].score + gapPenalty(j);
            matrix[0][j].hGapLen = 0;
            matrix[0][j].vGapLen = j;
            matrix[0][j].direction = VERTICAL;
         }else if(j == 0){
            // this is the same initialization as above, except for the cells along
            // the top horizontal edge
            matrix[i][0].score = matrix[i-1][0].score + gapPenalty(i);
            matrix[i][0].hGapLen = i;
            matrix[i][0].vGapLen = 0;
            matrix[i][0].direction = HORIZONTAL;
         }else{

            // get the cells that we are working with
            matrixCell dCell = matrix[i-1][j-1];
            matrixCell hCell = matrix[i-1][j  ];
            matrixCell vCell = matrix[i  ][j-1];

            // "this cell". I'm OCD about name lengths being the same
            matrixCell tCell;

            // get the gap lengths that led up to
            // this cell from both directions
            int hGapLen = hCell.hGapLen + 1;
            int vGapLen = vCell.vGapLen + 1;

            // get the possible scores for this cell, keep
            // the max of the three possibilities
            int dScore = dCell.score + similarity(A[i], B[j]);
            int hScore = hCell.score + gapPenalty(hGapLen);
            int vScore = vCell.score + gapPenalty(vGapLen);

            tCell.score = max(dScore, vScore, hScore);

            // Since it's possible for multiple directions to give
            // the same score, we check for all of the possibilities,
            // and update the direction flags of this cell, which are
            // used in the traceback phase.
            tCell.direction = 0;
            if(tCell.score == dScore){
               tCell.direction |= DIAGONAL;
            }

            // for the vertical and horizontal paths, we also update
            // the gap length at this cell for use by descendant cells
            // zero indicates that there was no gap leading to this cell
            if(tCell.score == vScore){
               tCell.direction |= VERTICAL;
               tCell.vGapLen = vGapLen;
            }else{
               tCell.vGapLen = 0;
            }

            if(tCell.score == hScore){
               tCell.direction |= HORIZONTAL;
               tCell.hGapLen = hGapLen;
            }else{
               tCell.hGapLen = 0;
            }

            matrix[i][j] = tCell;
         }
      }
   }
}

void NeedlemanWunsch::_traceBack(){
   // This function follows the directions at each matrix cell
   // since all possible alignments are equally scoring, we will
   // favor the ones that are most diagonal first, and the ones
   // that tend towards adding gaps to the shorter of the two
   // strings second

   int i = width - 1;
   int j = height- 1;

   // re-init the aligned strings
   string alignedA("");
   string alignedB("");
   string matchType("");

   // while we're not at the end point
   // (matrix[0][0] is the only cell with direction == 0)
   while(1){
      if(matrix[i][j].direction & DIAGONAL){
         alignedA.push_back(A[i]);
         alignedB.push_back(B[j]);
         matchType.push_back(A[i] == B[j] ? MATCH : MISMATCH);
         --i;
         --j;
         // TODO - all this stuff is supposed to be outside the loop
         // this if statement that is 
         if(j == 0 && i == 0){
            alignedA.push_back(A[i]);
            alignedB.push_back(B[j]);
            matchType.push_back(A[i] == B[j] ? MATCH : MISMATCH);
            break;
         }
      }else if(matrix[i][j].direction & HORIZONTAL){
         alignedA.push_back(A[i]);
         alignedB.push_back('-');
         matchType.push_back(GAP);
         --i;
         if(j == 0 && i == 0){
            alignedA.push_back(A[i]);
            alignedB.push_back('-');
            matchType.push_back(GAP);
            break;
         }
      }else if(matrix[i][j].direction & VERTICAL){
         alignedA.push_back('-');
         alignedB.push_back(B[j]);
         matchType.push_back(GAP);
         --j;
         if(j == 0 && i == 0){
            alignedA.push_back('-');
            alignedB.push_back(B[j]);
            matchType.push_back(GAP);
            break;
         }
      }
   }

   // allocate size in the alignment object

   alignment.A.resize(alignedA.size(), ' ');
   alignment.B.resize(alignedA.size(), ' ');
   alignment.matchType.resize(alignedA.size(), ' ');

   copy(alignedA.begin(), alignedA.end(), alignment.A.rbegin());
   copy(alignedB.begin(), alignedB.end(), alignment.B.rbegin());
   copy(matchType.begin(), matchType.end(), alignment.matchType.rbegin());

   // the score is the bottom right of the matrix
   alignment.score = matrix[width-1][height-1].score;
}


int NeedlemanWunsch::similarity(char a, char b){
   if(similarityFunction){
      return similarityFunction(a, b);
   }
   return a == b ? 3 : -1;
}

int NeedlemanWunsch::gapPenalty(int length){
   if(gapPenaltyFunction){
      return gapPenaltyFunction(length);
   }
   return -1;
}

/////////////////////////////////////////////
//                                         //
//                Public                   //
//                                         //
/////////////////////////////////////////////

// constructor!
//
NeedlemanWunsch::NeedlemanWunsch(string a, string b)
: A(a), B(b), matrix(NULL), similarityFunction(NULL), gapPenaltyFunction(NULL)
{
   width  = A.size();
   height = B.size();
   // make sure that the width of the matrix is greater than the height
   // this is so that creating gaps in the shorter of the two sequences
   // is favoured over gaps in the longer of the two
   if(height > width){
      swap(A, B);
      swap(width, height);
   }
}

NeedlemanWunsch::NeedlemanWunsch()
: A(""), B(""), width(0), height(0), matrix(NULL), similarityFunction(NULL), gapPenaltyFunction(NULL)
{
}

// just have to make sure that the matrix is freed
NeedlemanWunsch::~NeedlemanWunsch(){
   if(matrix){
      for(int i = 0; i < width; ++i){
         delete[] matrix[i];
         matrix[i] = NULL;
      }
      delete[] matrix;
   }

}

void NeedlemanWunsch::setStrings(string a, string b){
   A = a;
   B = b;
   width  = A.size();
   height = B.size();
   // make sure that the width of the matrix is greater than the height
   // this is so that creating gaps in the shorter of the two sequences
   // is favoured over gaps in the longer of the two
   if(height > width){
      swap(A, B);
      swap(width, height);
   }
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

void NeedlemanWunsch::setSimilarityFunction(int (*f)(char, char)){
   similarityFunction = f;
}

void NeedlemanWunsch::setGapPenaltyFunction(int (*f)(int)){
   gapPenaltyFunction = f;
}

void NeedlemanWunsch::align(Alignment & retAlign){
   _init();
   _traceBack();
   retAlign = Alignment(alignment);
}

