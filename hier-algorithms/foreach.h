#ifndef FOREACH_HIERARCHICAL_H
#define FOREACH_HIERARCHICAL_H

#include "nested-reduce.h"
#include <algorithm>


namespace hierarchical {
  
struct wrap_foreach {
  template<typename Res, typename I, typename F>
  Res operator()(Res init, I b, I e, F f) const
  {
    std::for_each(b,e,f);
    return init;
  }
};

template<class It, class Op>
void for_each(It b, It e, Op op)
{
  int dummy_init = 0;
  nested_reduction(wrap_foreach(), dummy_init, b,e, op);
} 


}

#endif
