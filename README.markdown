So, how do you use this code anyways?

compile:
--------

    make

(Any suggestions to make my makefile more portable are welcome)

Test it out:
------------

Get some tweets:

    ./getTwitter.py <keyword> [<num pages>] | sort | uniq > twitter.txt

Piping through sort and uniq is recommended to avoid wasting time aligning
duplicate tweets.

Align some tweets:

    ./nw twitter.txt > alignments.html

An html representation of the alignments is written to stdout, and a coloured
version using ANSI escape sequences is written to stderr. 

Comments, criticisms and forks welcome.
