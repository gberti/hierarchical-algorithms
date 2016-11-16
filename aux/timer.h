#ifndef GB_TIMER_H
#define GB_TIMER_H

//#include <time.h>
//#include <sys/time.h>
#include <chrono>
#include <iostream>

namespace aux {
  
inline double start_timer( )
{
 
  /* struct timeval tim;
  std::gettimeofday(&tim, NULL);
  return ( tim.tv_sec+(tim.tv_usec/1000000.0) );
  */
  using namespace std::chrono;
  time_point<high_resolution_clock> now = high_resolution_clock::now();
  typedef duration<double, std::ratio<1> > second_type;
  second_type secs(now.time_since_epoch());
  return secs.count();
}
  
  
  inline double get_timer( double timer )
  {
    return (start_timer() - timer);
  }
  
  inline void stop_timer(double & timer)
  { timer = get_timer(timer); }
  
}

#endif
