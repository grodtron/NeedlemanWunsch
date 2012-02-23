#ifndef NeedlemanWunsch_H_
#define NeedlemanWunsch_H_

#include <string>
using std::string;

struct matrixCell{
   int score;
   unsigned char direction;
   int hGapLen;
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

      string A;
      string B;

      Alignment alignment;

      int width;
      int height;
      matrixCell ** matrix;

      void _init();

      void _traceBack();

      int similarity(char a, char b);
      int (*similarityFunction)(char a, char b);
      
      int gapPenalty(int gapLength);
      int (*gapPenaltyFunction)(int gapLength);

   public:
      NeedlemanWunsch(string a, string b);
      NeedlemanWunsch();
     ~NeedlemanWunsch();
      void setStrings(string a, string b);
      void setSimilarityFunction(int (*f)(char, char));
      void setGapPenaltyFunction(int (*f)(int));
      void align(Alignment &);
};


#endif
