#ifndef NESTED_FIND_ALGORITHMS_H
#define NESTED_FIND_ALGORITHMS_H

#include "hierarchical-iterator-traits.h"

namespace hierarchical {

template<class A, class It>
struct result_type {
  typedef typename A::template result_type<It> type;
};

/*! Generic implementation of a generic find type algorithm

 */
template<typename Algo, typename T, typename It, typename ... Other>
//typename Algo::template result_type<It> 
typename result_type<Algo,It>::type
nested_find(Algo f, T init, It b, It e, Other... other);
//Res nested_find(Algo f, Res init, It b, It e, Other... other);

// bottom case for flat sequence
template<typename Algo, typename T, typename It, typename ... Other>
typename result_type<Algo,It>::type
nested_find(false_type, Algo f, T init, It b, It e, Other... other)
{
  std::cout << "nested find flat.\n";
  return f(init,b,e, other...);
}

// nested case
template<typename Algo, typename T, typename It, typename ... Other>
typename result_type<Algo,It>::type
nested_find(true_type,  Algo f, T init, It b, It e, Other... other)
{
  std::cout << "nested find hier.\n";
  using std::begin;
  using std::end;
  typedef typename It::segment_iterator segit;
  typedef typename It::local_iterator   locit;
  segit bs = b.segment(), es = e.segment(); // es = last or one-past-last segment
  typedef typename result_type<Algo,It>::type result_type;
  result_type res(b,e); // how to initialise?? result_type::initial_result(b,e)
  if (bs != b.end_segment()) {
    locit bl = b.local();
    //locit el = (bs == es ? e.local() : end(*bs));
    locit el = (bs == es ? e.local() : b.end_local(bs));
    res.update(nested_find(f, init, bl,el,other...), bs, es);
    if(f.complete(res))
      return res;
    if(bs != es)
      bs = b.next_segment(bs); // ++bs;
  }
  while(bs != es) {
    // In general, state can contain hier. info (iterators) and must be projected down
    // max_element -> state would contain the largest found element position in terms of hierarchcal iterator, must be mapped down to a local iterator
    //  ... but wait, this isn't in the right sequence
    // now the state parameter is not coupled any more to the result type!
    //res.update(nested_find(f, res.project_state(project()), b.begin_local(bs), b.end_local(bs), other...), bs);
    res.update(nested_find(f, res.state(), b.begin_local(bs), b.end_local(bs), other...), bs, es);
    if(f.complete(res))
      return res;
    bs = b.next_segment(bs); // ++bs;
  }
  if (bs != b.end_segment()) {
    // locit bl = begin(*bs);
    locit bl = b.begin_local(bs);
    locit el = e.local();
    res.update(nested_find(f, res.state(), bl, el, other...),bs, es);
  }

  return res;
}


// branch depending on Iterator type
template<typename Algo, typename T, typename It, typename ... Other>
typename result_type<Algo,It>::type
nested_find(Algo f, T init, It b, It e, Other... other)
{
  
  std::cout << "nested find branch\n";
  return nested_find(typename is_segmented<It>::res(),
		     f, init, b,e, other...);
} 

}

#endif
