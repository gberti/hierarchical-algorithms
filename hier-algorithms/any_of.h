#ifndef ANY_OF_HIERARCHICAL_GENERIC_H
#define ANY_OF_HIERARCHICAL_GENERIC_H

#include "nested-find.h"
#include <algorithm>

struct wrap_any_of {
  
  struct result_type {
    bool res;   // result of any_of(b,e, p)
    result_type(bool r) :  res(r) {}
    // if any_of(b1,e1,p) returns true for a subrange [b1,el) \subset [b,e)
    // then any_of(b,e,p) is true as well
    bool complete() const { return res == true; }
    bool result()  const  { return res; }
  };

  static bool result  (result_type r) { return r.result(); }
  static bool complete(result_type r) { return r.complete(); }
  
  template<typename I, typename P>
  result_type operator()(result_type init, I b, I e, P p) const
  {
    return result_type(std::any_of(b,e,p));
  }

};

template<class It, class P>
bool any_of_h(It b, It e, P t)
{
  typedef wrap_any_of::result_type init_type;
  return wrap_any_of::result(nested_find(wrap_any_of(),
					 init_type(false), b,e, t));
} 

  

#endif
