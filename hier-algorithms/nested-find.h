#ifndef NESTED_FIND_ALGORITHMS_H
#define NESTED_FIND_ALGORITHMS_H

#include "hierarchical-iterator-traits.h"

/*! Generic implementation of a generic find type algorithm

 */
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_find(Algo f, Res init, It b, It e, Other... other);

// bottom case for flat sequence
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_find(false_type, Algo f, Res init, It b, It e, Other... other)
{
  std::cout << "nested find flat.\n";
  return f(init,b,e, other...);
}

// nested case
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_find(true_type,  Algo f, Res init, It b, It e, Other... other)
{
  std::cout << "nested find hier.\n";
  using std::begin;
  using std::end;
  typedef typename It::segment_iterator segit;
  typedef typename It::local_iterator   locit;
  segit bs = b.segment(), es = e.segment(); // es = last or one-past-last segmen
  if (bs != b.end_segment()) {
    locit bl = b.local();
    //locit el = (bs == es ? e.local() : end(*bs));
    locit el = (bs == es ? e.local() : b.end_local(bs));
    init = nested_find(f, init, bl,el,other...);
    if(f.complete(init))
      return init;
    if(bs != es)
      bs = b.next_segment(bs); // ++bs;
  }
  while(bs != es) {
    // init = nested_reduction(f,init, begin(*bs), end(*bs), other...);
    init = nested_find(f,init, b.begin_local(bs), b.end_local(bs), other...);
    if(f.complete(init))
      return init;
    bs = b.next_segment(bs); // ++bs;
  }
  if (bs != b.end_segment()) {
    // locit bl = begin(*bs);
    locit bl = b.begin_local(bs);
    locit el = e.local();
    init = nested_find(f,init, bl, el, other...);
  }

  return init;
}


// branch depending on Iterator type
template<typename Algo, typename Res, typename It, typename ... Other>
Res nested_find(Algo f, Res init, It b, It e, Other... other)
{
  
  std::cout << "nested find branch\n";
  return nested_find(typename is_segmented<It>::res(),
		     f, init, b,e, other...);
} 


#endif
