#ifndef Alignment_H_
#define Alignment_H_

#include <string>
using std::string;

#include <iostream>
using std::ostream;

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

		static const int  CONSOLE;
		static const int  HTML;

		static void printHeader(ostream & stream, int format);
		void print(ostream & stream, int format) const;

      int getScore() const;
};


bool operator< (const Alignment & a, const Alignment & b);
bool operator> (const Alignment & a, const Alignment & b);

#endif
