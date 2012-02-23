#!/usr/bin/python

import sys

# do some error checking and stuff
if len(sys.argv) != 2 and len(sys.argv) != 3:
   sys.stderr.write("Exactly one or two command line arg is required (keyword + nn pages)\n")
   sys.stderr.write("Btw, it is adviseable to pipe through sort and then uniq\n")
   sys.stderr.write("Par example:\n    "+__file__+" \"keyword\" | sort | uniq > twitter.txt\n")
   sys.exit(2)


try:
   from twython import Twython
except ImportError:
   sys.stderr.write( "Module 'twython' required. Get it from here https://github.com/ryanmcgrath/twython\n" )
   sys.stderr.write( "or: sudo apt-get install setuptools-python; sudo easy_install twython\n" )
   sys.exit(1)

# do the other stuff
# ... and stuff

twitter = Twython()
for i in range(int(sys.argv[2]) if len(sys.argv) == 3 else 3):
   i+=1
   results = twitter.search(q=sys.argv[1], page=str(i))
   print '\n'.join(r['text'].encode('ascii', 'ignore').replace('\n','') for r in results['results'])
