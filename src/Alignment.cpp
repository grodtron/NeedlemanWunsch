#include <iostream>
using std::ostream;
using std::endl;

#include <string>
using std::string;

#include <cassert>

#include "../include/NeedlemanWunsch.h"
#include "../include/Alignment.h"

// These constants are used to keep track of whether
// each position in the alignment is a match, mismatch
// or gap
const char Alignment::MATCH = 'M';
const char Alignment::MISMATCH = 'm';
const char Alignment::GAP = 'g';


// operators for sorting. One of them is not use, but I forget which
// TODO - check and remove the unneeded one
bool operator< (const Alignment & a, const Alignment & b){
   return a.score < b.score;
}

bool operator> (const Alignment & a, const Alignment & b){
   return a.score > b.score;
}

int Alignment::getScore() const{
   return score;
}


// Below are the functions related to create output

const int Alignment::CONSOLE = 0;
const int Alignment::HTML    = 1;

// the css that is necessary for the HTML version to render properly
void Alignment::printHeader(ostream & stream, int format){

   // output the css for the html version
   if(format == HTML){
      stream << "<style>"                   << endl
             << "body{"                     << endl
             << "   background:black;"      << endl
             << "   font-family:monospace;" << endl
             << "   font-size:10px;"        << endl
             << "   overflow-x:auto;"       << endl
             << "}"                         << endl
             << ".NW_mismatch{"             << endl
             << "   color:red;"             << endl
             << "}"                         << endl
             << ".NW_gap{"                  << endl
             << "   color:gray;"            << endl
             << "}"                         << endl
             << ".NW_match{"                << endl
             << "   color:gray;"            << endl
             << "   background-color:#0F0;" << endl
             << "}"                         << endl
             << "</style>"                  << endl;

   }
}

// print the whole thing
void Alignment::print(ostream & stream, int format) const {
   
   assert((format == CONSOLE || format == HTML) && "invalid format type");
   
   const char * matchStrings[]    = {"\033[1;102;90m", "<span class = NW_match>"};
   const char * mismatchStrings[] = {"\033[1;91m", "<span class = NW_mismatch>"};
   const char * gapStrings[]      = {"\033[90m", "<span class = NW_gap>"};
   const char * endStrings[]      = {"\033[0m", "</span>"};

   const char * matchString    = matchStrings[format];
   const char * mismatchString = mismatchStrings[format];
   const char * gapString      = gapStrings[format];
   const char * endString      = endStrings[format];

   if(format == HTML) stream << "<pre>";
   stream << "Score: " << score << endl;

   string s = A;

   do{
      int size = s.size();
      for(int i = 0; i < size; ++i){

         if(i == 0 || matchType[i] != matchType[i - 1]){
            if (i != 0) stream << endString;

            if(matchType[i] == MATCH){
               stream << matchString;
            }else if(matchType[i] == MISMATCH){
               stream << mismatchString;
            }else{
               stream << gapString;
            }
         }
         stream.put(s[i]);
      }
      stream << endString << endl;
      // this is a funky loop that will execute once for A and once for B
      //
      // (s = B) == B is used instead of (s = B) because
      // apparently you can't cast a string directly to a bool... who knew
   }while(s == A && (s = B) == B);

   if(format == HTML) stream << "</pre>";
}
