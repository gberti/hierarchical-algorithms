#ifndef NESTED_REDUCE_ALGORITHMS_H
#define NESTED_REDUCE_ALGORITHMS_H

#include "hierarchical-iterator-traits.h"
#include <iterator>

namespace hierarchical {
  
/*! Generic implementation of a generic reduction type algorithm

 */
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_reduction(Algo f, Res init, It b, It e, Other... other);

// bottom case for flat sequence
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_reduction(false_type, Algo f, Res init, It b, It e, Other... other)
{
  //std::cout << "nested reduction flat.\n";
  return f(init,b,e, other...);
}

// nested case
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_reduction(true_type,  Algo f, Res init, It b, It e, Other... other)
{
  //std::cout << "nested reduction hier.\n";
  using std::begin;
  using std::end;
  typedef typename It::segment_iterator segit;
  typedef typename It::local_iterator   locit;
  segit bs = b.segment(), es = e.segment(); // es = last or one-past-last segmen
  if (bs != b.end_segment()) {
    locit bl = b.local();
    //locit el = (bs == es ? e.local() : end(*bs));
    locit el = (bs == es ? e.local() : b.end_local(bs));
    init = nested_reduction(f, init, bl,el,other...);
    if(bs != es)
      bs = b.next_segment(bs); // ++bs;
  }
  while(bs != es) {
    // init = nested_reduction(f,init, begin(*bs), end(*bs), other...);
    init = nested_reduction(f,init, b.begin_local(bs), b.end_local(bs), other...);
    bs = b.next_segment(bs); // ++bs;
  }
  if (bs != b.end_segment()) {
    // locit bl = begin(*bs);
    locit bl = b.begin_local(bs);
    locit el = e.local();
    init = nested_reduction(f,init, bl, el, other...);
  }

  return init;
}


// branch depending on Iterator type
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_reduction(Algo f, Res init, It b, It e, Other... other)
{
  
  //std::cout << "nested reduction branch\n";
  return nested_reduction(typename is_segmented<It>::res(),
			  f, init, b,e, other...);
} 

}

#endif
