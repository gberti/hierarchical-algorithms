#ifndef FIND_HIERARCHICAL_H
#define FIND_HIERARCHICAL_H

#include "hierarchical-iterator-traits.h"
#include <algorithm>

/*! Direct implementation of a hierarchical version of std::find. 
    This header it is included for didactical purposes.
 */

template<typename It, typename T>
It hier_find(It b, It e, T t);

// bottom case for flat sequence
template<typename It, typename T>
  It hier_find(hierarchical::false_type, It b, It e, T t)
{
  std::cout << "hier find flat.\n";
  return std::find(b,e,t);
}

// nested case
template<typename It, typename T>
It hier_find(hierarchical::true_type, It b, It e, T t)
{
  std::cout << "hier find nested.\n";
  using std::begin;
  using std::end;
  typedef typename It::segment_iterator segit;
  typedef typename It::local_iterator   locit;
  segit bs = b.segment(), es = e.segment(); // es = last or one-past-last segmen
  if (bs != b.end_segment()) {
    locit bl = b.local();
    //locit el = (bs == es ? e.local() : end(*bs));
    locit el = (bs == es ? e.local() : b.end_local(bs));
    locit rl  = hier_find(bl,el,t);
    if(rl != el) {
      It res(bs,es,rl);
      return res;
    }
    if(bs != es)
      bs = b.next_segment(bs); // ++bs;
  }
  while(bs != es) {
    locit bl = b.begin_local(bs);
    locit el = b.end_local(bs);
    locit rl = hier_find(bl, el, t);
    if(rl != el) {
      It res(bs,es,rl);
      return res;
    }
    bs = b.next_segment(bs); // ++bs;
  }
  if (bs != b.end_segment()) {
    // locit bl = begin(*bs);
    locit bl = b.begin_local(bs);
    locit el = e.local();
    locit rl = hier_find(bl, el, t);
    if(rl != el) {
      It res(bs,es,rl);
      return res;
    }
  }
  // still here? Nothing found.
  return e;
}


// branch depending on Iterator type
template<typename It, typename T>
It hier_find(It b, It e, T t)
{
  std::cout << "nested find branch\n";
  return hier_find(typename hierarchical::is_segmented<It>::res(),
		   b,e,t);
} 


#endif
