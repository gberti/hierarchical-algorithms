#ifndef SEGMENTED_ITERATOR_H
#define SEGMENTED_ITERATOR_H

namespace hierarchical {
  template<class SIterator, class LocalIterator, int NoOpt = 0>
  struct segmented_iterator;

}

namespace std {
  template<class I> struct iterator_traits;
  
  template<class S, class L, int NoOpt>
  struct iterator_traits<hierarchical::segmented_iterator<S,L,NoOpt> >;

  /*! Overload of std::advance

  \note This code currently overloads advance and distance in namespace std.
  This is brittle as it relies on the sequence of includes 
  (header segmented-iterator.h must come before any use of std::advance and std::distance in a header), i.e. the overloads must be found in phase 1 of two-phase-lookup.
  
  A better solution would be to define them in the hierarchical-namespace 
  and find them in ADL (phase 2). 
  However, the standard algorithms currently do not support ADL for distance and advance,
  as they explicitely call std::advance and std::distance.

   */
  template<class S, class L>
  typename iterator_traits<hierarchical::segmented_iterator<S,L,0> >::difference_type
  distance(hierarchical::segmented_iterator<S,L,0> x,
	   hierarchical::segmented_iterator<S,L,0> y);
  
  template<class S, class L, class Distance>
  void advance(hierarchical::segmented_iterator<S,L,0> & x,
	       Distance n);
}


#include "hier-algorithms/hierarchical-iterator-traits.h"


#include <iterator>
#include <iostream>

namespace hierarchical {
  
/*! Linear iterator over a hierarchical sequence

A hierarchical sequence is a (major) sequence [SB, SE) 
 where the value type of *S in [SB,SE) is another (minor) sequence 
 (which may be hierarchical again). 
- A standard sequence is a single-layer hierarchical sequence.
- The bottom layer of a h.s. is the bottom layer of its minor sequence type, and is identical to the sequence itself for a single-layer sequence. 
- The value type of a hiearchical sequence

- A valid position is presented with (S, L(S)), 
  where L is a valid iterator in [begin(S), end(S)).
- The past-the end position is presented with (SE,<undefined>),
  where SE is the past the end iterator of the S-Range.

\todo Make this a std iterator (op-- and post-increment missing)

\todo Fix iterator_category

\todo Deduce the second parameter LocalIterator from SIterator::value_type
  (Problem: Stop recursion in case the bottom value_type should be a container,
  e.g. summing up 3D-vectors)

\todo The distance and advance function work only for two-level hierarchies, as the member function ssize() only returns the size of the second hierarchy level.
 This is related on the previous item: If the hierarchy can be deduced, then the size of the flat sequence can also be computed correctly.

\todo Initialising a range [b, e) would be easier than initialising 2 separate iterators, which must repeat the end iterators for segment and local.

\todo Rename in flattening_iterator

\todo re-cast as range (flat_view ?)
 */

  template<class SIterator, class LocalIterator, int NoOpt>
struct segmented_iterator {
  typedef SIterator     segment_iterator;
  typedef LocalIterator local_iterator;
  
  typedef segmented_iterator self;
  typedef std::iterator_traits<LocalIterator> loc;
  typedef typename loc::value_type        value_type; 
  typedef typename loc::reference         reference;
  typedef typename loc::difference_type   difference_type;
  typedef typename loc::pointer           pointer;
  typedef typename loc::iterator_category iterator_category; // not quite true, it's the weaker of this and SIterator's category. Also, it cannot be random_access as iterator difference is not constant (linear in the number of segments). So advance has a non-trivial specialization!
  
  SIterator      curr_seg, end_seg;
  LocalIterator  curr_loc, end_loc;
public:
  segmented_iterator(SIterator begin_segment, SIterator end_segment)
    : curr_seg(begin_segment), end_seg(end_segment)
  {
    update_local();
  }

  segmented_iterator(SIterator begin_segment,   SIterator end_segment,
		     LocalIterator begin_local)
    : curr_seg  (begin_segment), end_seg  (end_segment),
      curr_loc(begin_local),   end_loc(end(*begin_segment))
  { advance_til_valid(); }

  segment_iterator segment()      const { return curr_seg; }
  segment_iterator end_segment()  const { return end_seg; }
  local_iterator   local()        const { return curr_loc; }
  local_iterator   begin_local(segment_iterator s) const { return s->begin(); }
  local_iterator   end_local  (segment_iterator s) const { return s->end(); }
  segment_iterator next_segment(segment_iterator s) const
  { segment_iterator ns=s; ++ns; return ns; }

  // FIXME: This should return the size in terms of elements "beneath" s,
  // not the number of elements of the top-level sequence pointed to by s
  difference_type ssize(segment_iterator s) const {
    return s->size(); // OK only for 2-level hiearchies: *s is a flat sequence
  }
  
  value_type const& operator*() const { return *curr_loc; }
  value_type      & operator*()       { return *curr_loc; }

  /*! \pre state is valid
   */
  self& operator++() {
    ++curr_loc;
    if(curr_loc == end_loc) {
      ++curr_seg; 
      update_local();
    }
    return *this;
  }

    /*! true if both are at end of major sequence or both (major, minor) pairs are equal 
     */
  bool equal(self const& rhs) const {
    return (curr_seg == rhs.curr_seg)
      &&  (curr_seg == end_seg || curr_loc == rhs.curr_loc);
  }

  /*! true if *this < rhs
    \todo This assumes that both minor and major are RA!
  */
  bool less(self const& rhs) const {
    return (curr_seg <  rhs.curr_seg)
        || (curr_seg == rhs.curr_seg && curr_loc < rhs.curr_loc);
  }

  /*! Optimised distance for segmented iterators

     \note This works for a positive distance only (as required for bidirectional iterators). If both minor and major are RA, it could be implemented to work for negative distances as well.
   */
  difference_type distance1(self const& rhs) const {
    using std::distance;
    // std::cerr << "seg::distance()";
    /* We assume rhs is reachable from this, i.e. result >= 0
    if(! less(rhs))
      return rhs.distance(*this);
    */
    difference_type res = 0;
    if(curr_seg == rhs.curr_seg) {
      res = distance(local(), rhs.local());
    }
    else if (distance(curr_seg, rhs.curr_seg) > 0) {
      res += distance(local(), end_local(curr_seg));
      // handle all full segments S < SI < rhs.SI
      SIterator SI = next_segment(curr_seg);
      while(SI != rhs.curr_seg && SI != end_seg) {
	res += (*SI).size();
	SI = next_segment(SI);
      }
      // now SI == rhs.curr_seg
      if(SI != end_seg)
	res += distance(begin_local(SI), rhs.local());
    }
    else {
      res = -rhs.distance1(*this);
    }
    return res;
  }

  
  /*! Optimized advance

    \Note does not work for more than 2 hierarchy levels until ssize() works correctly 
   */
  void advance1(difference_type n) {
    // std::cerr << "seg::advance(" << n << "): this="; print(std::cerr);
    using std::advance; // enable ADL to find both std:: and customised versions
    using std::distance;

    if(n > 0 && curr_seg != end_seg) {  
      difference_type de = distance(curr_loc, end_local(curr_seg));
      while(curr_seg != end_seg && n >= de) {
	n -= de; // n >= 0
	curr_seg++;
	if(curr_seg != end_seg) {
	  curr_loc = begin_local(curr_seg); 
	  de = ssize(curr_seg);
	}
      }
      if(curr_seg != end_seg) {
	advance(curr_loc, n);
	end_loc = end_local(curr_seg); // *this is not in a valid state until this line
      }
    }
    else if(n < 0) {
      if(curr_seg == end_seg) {
	curr_seg--;
	curr_loc = end_local(curr_seg);
      }
      difference_type db = - std::distance(begin_local(curr_seg), curr_loc);
      while(n < db) {
	n -= db;
	curr_seg--;
	curr_loc = end_local(curr_seg);
	db = -ssize(curr_seg);
      }
      std::advance(curr_loc,n);
      end_loc = end_local(curr_seg);
    }
  }
  
    
  void update_local() {
    if(curr_seg != end_seg) {
      curr_loc = std::begin(*curr_seg);
      end_loc  = std::end  (*curr_seg);
    }
  }
  void advance_til_valid() {
    if(curr_seg != end_seg && curr_loc == end_loc) {
      ++curr_seg;
      update_local();
    }
  }
  void print(std::ostream& out) const {
    out << "([" << & (*curr_seg) << "," << &(* end_seg) << "]," << &(*    curr_loc) << ")";
  }

  template<class C>
  static self sbegin(C& c) { return self(c.begin(), c.end(), c.begin()->begin()); }
  template<class C>
  static self send  (C& c) { return self(c.end(),   c.end(), local_iterator()); }
  
};

  template<class S, class L, int N>
  std::ostream& operator<<(std::ostream& out, segmented_iterator<S,L,N> const& it)
{ it.print(out); return out; }

  template<class S, class L, int N>
bool operator==(segmented_iterator<S,L,N> const& lhs,
		segmented_iterator<S,L,N> const& rhs)
{ return lhs.equal(rhs); }

  template<class S, class L, int N>
  bool operator!=(segmented_iterator<S,L,N> const& lhs,
		  segmented_iterator<S,L,N> const& rhs)
{ return ! (lhs == rhs); }

}


namespace hierarchical {

  template<class S, class L, int N>
  struct is_segmented<segmented_iterator<S,L,N> > { typedef true_type res; };


}

namespace std {

  // INCOMPLETE
  template<class S, class L, int N>
  struct iterator_traits<hierarchical::segmented_iterator<S,L, N> >
  {
    typedef hierarchical::segmented_iterator<S,L> sit;
    typedef typename sit::value_type        value_type; 
    typedef typename sit::reference         reference;
    typedef typename sit::difference_type   difference_type;
    typedef typename sit::pointer           pointer;
    //typedef typename sit::iterator_category iterator_category;
    typedef forward_iterator_tag iterator_category; // avoid problems with std algorithms
  };

}

/* // This is how it should be done
namespace hierarchical {
  template<class S, class L>
  typename std::iterator_traits<segmented_iterator<S,L> >::difference_type
  distance(segmented_iterator<S,L> x,
	   segmented_iterator<S,L> y)
  { return x.distance1(y); }

  template<class S, class L, class Distance>
  void advance(segmented_iterator<S,L,0> & x,
	       Distance n)
  { x.advance1(n); }


}
*/

namespace std {

  template<class S, class L>
  typename iterator_traits<hierarchical::segmented_iterator<S,L,0> >::difference_type
  distance(hierarchical::segmented_iterator<S,L,0> x,
	   hierarchical::segmented_iterator<S,L,0> y)
  { return x.distance1(y); }

  template<class S, class L, class Distance>
  void advance(hierarchical::segmented_iterator<S,L,0> & x,
	       Distance n)
  { x.advance1(n); }
  
}

 
#endif
