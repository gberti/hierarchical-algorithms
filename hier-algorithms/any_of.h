#ifndef ANY_OF_HIERARCHICAL_GENERIC_H
#define ANY_OF_HIERARCHICAL_GENERIC_H


#include "nested-find.h"
#include <algorithm>

namespace hierarchical {
  
struct wrap_any_of {
  
  struct state_type {
    bool found;
    state_type(bool f = false) : found(f) {}
  };

  template<class It>
  struct result_type {
    state_type  st;   // result of any_of(b,e, p)
    result_type(bool r) : st(r) {}
    result_type(It b, It e) {}
    
    state_type state() const { return st; }
    // if any_of(b1,e1,p) returns true for a subrange [b1,el) \subset [b,e)
    // then any_of(b,e,p) is true as well
    bool complete() const { return st.found == true; }
    bool result()   const { return st.found; }

    template<class Minor, class Major>
    void update(result_type<Minor> res_minor, Major seg, Major segend) 
    {
      st = res_minor.state();
    }
    
  };
 
  template<class It>
  static bool result  (result_type<It> r) { return r.result(); }
   
  template<class It>
  static bool complete(result_type<It> r) { return r.complete(); }
  
  template<typename I, typename P>
  result_type<I> operator()(state_type init, I b, I e, P p) const
  {
    return result_type<I>(std::any_of(b,e,p));
  }

  



};

template<class It, class P>
bool any_of(It b, It e, P t)
{
  typedef wrap_any_of::state_type init_type;
  return wrap_any_of::result(nested_find(wrap_any_of(),
					 init_type(false), b,e, t));
} 


}

#endif
