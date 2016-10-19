#ifndef FIND_HIERARCHICAL_GENERIC_H
#define FIND_HIERARCHICAL_GENERIC_H

#include "nested-find.h"
#include <algorithm>

namespace hierarchical {

  
struct wrap_find {

  struct state_type {
    bool found;
    state_type(bool f = false) : found(f) {}
  };

  
  template<class It>
  struct find_result {
    It found;
    It e;
    state_type st;
    find_result() {}
    find_result(It f, It e) : found(f), e(e), st(f != e) {}
    state_type state() const { return st; }
    bool complete()   const { return result() != e; }
    bool has_result() const { return complete(); }
    // TODO: sollten alle init-Argumente haben
    It   result()  const { return found; }

    template<class Minor, class Major>
    void update(find_result<Minor> res_minor, Major seg, Major segend) 
    { 
      if(res_minor.complete()) {
	found = It(seg,segend,res_minor.found); // we could actually pass a functor lift 
	// found = lift(res_minor.found), lift = [&](Minor m) { return SetIt(S,m) }
        // so we don't have to know about how to construct It from a lower level iterator
	st = state_type(true);
      }
    }
  };

  static state_type initial_state() { return state_type(); }

  template<class It>
  static It result    (find_result<It>  f) { return f.result(); }
  
  template<class It>
  static bool complete(find_result<It>  f) { return f.complete(); }
  
  template<typename I, typename T>
  find_result<I> operator()(state_type init, I b, I e, T t) const
  {
    I found = std::find(b,e,t);
    return find_result<I>(found,e);
  }

  template<typename I>
  find_result<I> dummy(I b, I e) const { return find_result<I>(b,e);}

  template<class It>
  using result_type = find_result<It>;
};

template<class It, class T>
It find(It b, It e, T t)
{
  //typedef wrap_find::find_result<It> init_type;
  typedef wrap_find::result_type<It> result_type;
  result_type res =nested_find(wrap_find(),
			       wrap_find::initial_state(), b,e, t);
  if(res.has_result())
    return wrap_find::result(res);
  else
    return e;
} 

}
  

#endif
