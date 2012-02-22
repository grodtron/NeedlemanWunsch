#ifndef TIMER_H
#define TIMER_H

#include <cstdlib>
#include <sys/time.h>

class Timer
{
      timeval timer[2];
   public:
      timeval start();
      timeval stop();
      float duration() const;
};

#endif
