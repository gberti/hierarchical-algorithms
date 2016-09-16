#ifndef ACCUMULATE_FLAT_H
#define ACCUMULATE_FLAT_H

template<class InputIterator, class T, class Op>
T accumulate_flat(InputIterator b, InputIterator e, T init, Op op)
{
  for(; b != e; ++b) {
    init = op(init,*b);
  }
  return init;
  
}

#endif
