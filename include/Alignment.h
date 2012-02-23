#ifndef Alignment_H_
#define Alignment_H_

#include <string>
using std::string;

class Alignment{
      friend class NeedlemanWunsch;
      friend bool operator< (const Alignment & a, const Alignment & b);
      friend bool operator> (const Alignment & a, const Alignment & b);
      string A;
      string B;
      string matchType;
      int score;
   public:

      static const char MATCH;
      static const char MISMATCH;
      static const char GAP;


      void print() const;
};


bool operator< (const Alignment & a, const Alignment & b);
bool operator> (const Alignment & a, const Alignment & b);

#endif
