#ifndef HIERARCHICAL_ITERATOR_TRAITS_H
#define HIERARCHICAL_ITERATOR_TRAITS_H

// #include <iterator>

namespace hierarchical {
  
struct false_type {};
struct true_type  {};

template<class I> struct is_segmented { typedef false_type res; };

}

#endif
