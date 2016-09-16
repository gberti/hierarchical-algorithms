#ifndef FIND_HIERARCHICAL_GENERIC_H
#define FIND_HIERARCHICAL_GENERIC_H

#include "nested-find.h"
#include <algorithm>

struct wrap_find {
  
  template<class It>
  struct find_result {
    It found;
    It e;
    find_result(It f, It e) : found(f), e(e) {}
    bool complete() const { return result() != e; }
    // TODO: sollten alle init-Argumente haben
    It   result()  const { return found; }
  };

  template<class It>
  static It result    (find_result<It>  f) { return f.result(); }
  
  template<class It>
  static bool complete(find_result<It>  f) { return f.complete(); }
  
  template<typename I, typename T>
  find_result<I> operator()(find_result<I> init, I b, I e, T t) const
  {
    return find_result<I>(std::find(b,e,t),e);
  }

  template<typename I>
  find_result<I> dummy(I b, I e) const { return find_result<I>(b,e);}
};

template<class It, class T>
It find_h(It b, It e, T t)
{
  typedef wrap_find::find_result<It> init_type;
  
  return wrap_find::result(nested_find(wrap_find(),
				       init_type(e,e), b,e, t));
} 

  

#endif
