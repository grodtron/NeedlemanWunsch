So, how do you use this anyways?

Test it out:
------------

First compile it - I'm not sure how portable my makefile is, but as long as you have
g++ and python you shouldn't have any problems. If you do run into problems, or have any
portability tricks to share, let me know.

    make

Then get some test data. This script uses the very nice [twython](https://github.com/ryanmcgrath/twython)
module for communicating with Twitter, so you'll need that if you don't already have it installed. Piping
the tweets through `sort` and `uniq` is a suggestion rather than a requirement

    ./getTwitter.py <keyword> [<num pages>] | sort | uniq > twitter.txt

Then align the tweets! The cpp code is set up to read lines from a file and align each
line with each other line. It writes out html to stdout, and highlighted results using
ANSI escape sequences to stderr. You can redirect to suit your needs.

    ./nw twitter.txt > alignments.html

Mess with the code:
-------------------

I've tried to comment the code well, so hopefully it is somewhat self-explanatory.
`src/NeedlemanWunsch.cpp` contains the class that wraps the main business logic. Its usage is
fairly simple:

    // initialize
    NeedlemanWunsch nw;
    Alignment al;
    nw.setStrings(A, B);
    // configure (optional)
    nw.setGapPenaltyFunction(someFunction);
    nw.setSimilarityFunction(someFunction);

    // run the algorithm
    nw.align(al);

    // show the results
    al.printHeader(cout, Alignment::HTML);
    al.print(cout, Alignment::HTML);
    al.print(cerr, Alignment::CONCOLE);

The optional configuration functions allow changing the scoring scheme that the algorithm uses.
`setGapPenaltyFunction` takes a pointer to a function that produces a penalty for a gap, based on
the length of the gap. `setSimilarityFunction` takes two chars, and returns an int representing
how similar the two chars are. Currently the similarity is based on the distance between the two
characters on a standard qwerty keyboard.
