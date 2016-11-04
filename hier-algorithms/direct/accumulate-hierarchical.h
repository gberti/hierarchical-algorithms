#ifndef ACCUMULATE_HIERARCHICAL_H
#define ACCUMULATE_HIERARCHICAL_H

#include "hierarchical-iterator-traits.h"
#include <numeric>

/* Direct implementation of hierarchical accumulate.
   This header is included for didactical purposes.
 */

template<class InputIterator, class T, class Op>
T accumulate_hier(InputIterator b, InputIterator e, T init, Op op);

template<class InputIterator, class T, class Op>
T accumulate_hier(hierarchical::false_type,
		  InputIterator b, InputIterator e, T init, Op op)
{
  // std::cerr << "accumulate_hier: Flat code\n";
  return std::accumulate(b,e,init,op);
}

template<class InputIterator, class T, class Op>
T accumulate_hier(hierarchical::true_type,
		  InputIterator b, InputIterator e, T init, Op op)
{
  // std::cerr << "accumulate_hier: Hierarchical code\n";
  
  using std::begin;
  using std::end;
  typedef typename InputIterator::segment_iterator segit;
  typedef typename InputIterator::local_iterator   locit;
  segit bs = b.segment(), es = e.segment(); // es = last or one-past-last segment
  T res(init);
  // must check for end, otherwise b.local() is not defined
  if (bs != b.end_segment()) {
    locit bl = b.local();
    // locit el = (bs == es ? e.local() : end(*bs)); // FIXME: Assumes segit::value_type is a container / range type
    locit el = (bs == es ? e.local() : b.end_local(bs)); 
    res = accumulate_hier(bl, el,res,op);
    if(bs == es)
      return res;
    // ++bs;
     bs = b.next_segment(bs);
  }
  // no check for end - if bs != es, then bs != end
  while(bs != es) {
    // res = accumulate_hier(begin(*bs), end(*bs), res, op);
    res = accumulate_hier(b.begin_local(bs), b.end_local(bs), res, op);
    bs = b.next_segment(bs);
    //++bs;
  }
  
  if (bs != b.end_segment()) {
    locit bl = b.begin_local(bs);
    locit el = e.local();
    res = accumulate_hier(bl,el, res, op);
  }

  return res;
}


template<class InputIterator, class T, class Op>
T accumulate_hier(InputIterator b, InputIterator e, T init, Op op)
{ return accumulate_hier(typename hierarchical::is_segmented<InputIterator>::res(),
			 b,e,init,op);
}

#endif
