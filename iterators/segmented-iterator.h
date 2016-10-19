#ifndef SEGMENTED_ITERATOR_H
#define SEGMENTED_ITERATOR_H

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

\todo Deduce the second parameter LocalIterator from SIterator::value_type
  (Problem: Stop recursion in case the bottom value_type should be a container,
  e.g. summing up 3D-vectors)

\todo For both arguments random_access, the resulting iterator cannot be random_access. E.g. difference is linear in the number of segments between the iterators (except if the segments have fixed size) or the underlying container stores a prefix sum of the segment sizes. However, it is clearly better than bidirectional

\todo Initialising a range [b, e) would be easier than initialising 2 separate iterators, which must repeat the end iterators for segment and local.

\todo Rename in flattening_iterator
 */

template<class SIterator, class LocalIterator>
struct segmented_iterator {
  typedef SIterator     segment_iterator;
  typedef LocalIterator local_iterator;
  
  //using std::begin;
  //using std::end;

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

  segment_iterator segment() const { return curr_seg; }
  segment_iterator end_segment()   const { return end_seg; }
  local_iterator   local()        const { return curr_loc; }
  local_iterator   begin_local(segment_iterator s) const { return s->begin(); }
  local_iterator   end_local  (segment_iterator s) const { return s->end(); }
  segment_iterator next_segment(segment_iterator s) const
  { segment_iterator ns=s; ++ns; return ns; }
  
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

  
  bool equal(self const& rhs) const {
    /*
    std::cout << "op==: "
	      << "lhs=";     print(std::cout); std::cout << " ";
    std::cout << "rhs="; rhs.print(std::cout); std::cout << "\n";
    */
    return (curr_seg == rhs.curr_seg)
      &&  (curr_seg == end_seg || curr_loc == rhs.curr_loc);
  }
  //! true if *this < rhs
  bool less(self const& rhs) const {
    return (curr_seg <  rhs.curr_seg)
        || (curr_seg == rhs.curr_seg && curr_loc < rhs.curr_loc);
  }

  /*
  difference_type distance(self const& rhs) const {
    using std::distance;
    using std::size;
    if(! less(rhs))
      return rhs.distance(*this);
    difference_type res = 0;
    if(S == rhs.S)
      res = distance(s, rhs.s);
    else {
      distance(s, end(*S));
      // handle all full segments S < SI < rhs.SI
      SIterator SI = next(S);
      while(SI != rhs.S) {
	res += size(*SI);
      }
      // now SI == rhs.S
      if(SI != ES)
	res += distance(begin(*SI), rhs.s);
    }
    return res;
  }

  void advance(difference_type n) {
    if(n > 0) {
      difference_type de = distance(s, end(*S));
      if(n < de)
	advance(s,n);
      else {
	S++;
	n -= (de+1);
	while(S != ES && n >= 0) {
	  de = size(*S);
	  n -= de;
	  S++;
	}
	if(n < 0) {
	  S--;
	  n += de;
	}
	if(S != ES) 
	  s = begin(*S) + n;
	else
	  assert(n == 0);
      }
      
    }
  }
  */
    
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

template<class S, class L>
std::ostream& operator<<(std::ostream& out, segmented_iterator<S,L> const& it)
{ it.print(out); return out; }

template<class S, class L>
bool operator==(segmented_iterator<S,L> const& lhs,
		segmented_iterator<S,L> const& rhs)
{ return lhs.equal(rhs); }

template<class S, class L>
bool operator!=(segmented_iterator<S,L> const& lhs,
		segmented_iterator<S,L> const& rhs)
{ return ! (lhs == rhs); }

}


namespace hierarchical {

  template<class S, class L>
  struct is_segmented<segmented_iterator<S,L> > { typedef true_type res; };

}

namespace std {

  // INCOMPLETE
  template<class S, class L>
  struct iterator_traits<hierarchical::segmented_iterator<S,L> >
  {
    typedef forward_iterator_tag iterator_category;
  };
}

 
#endif
