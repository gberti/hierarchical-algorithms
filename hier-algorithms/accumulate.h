#ifndef ACCUMULATE_HIERARCHICAL_GENERIC_H
#define ACCUMULATE_HIERARCHICAL_GENERIC_H

#include "nested-reduce.h"
#include "accumulate-flat.h"


struct wrap_accumulate {
  template<typename Res, typename I, typename Op>
  Res operator()(Res init, I b, I e, Op op) const
  { return accumulate_flat(b,e,init,op); }

  
  template<typename Res>
  static bool complete(Res && init) { return false; }

  template<typename Res>
  static Res result(Res && init)        { return init; }
  
};

template<class It, class T, class Op>
T accumulate_h(It b, It e, T init, Op op)
{
  return nested_reduction(wrap_accumulate(), init, b,e, op);
} 

#endif
