#ifndef CHUNKED_ITERATOR_H
#define CHUNKED_ITERATOR_H

#include "hierarchical-iterator-traits.h"

#include <iterator>
#include <iostream>

/*! "Plug-in" for chunked_iterator
 */
struct progress_observer
{
  int total_size, curr_chunk_size, num_chunks, curr_chunk, elements_handled;
  progress_observer()
    : total_size(0),
      curr_chunk_size(0),
      num_chunks(0),
      curr_chunk(0),
      elements_handled(0)
  {}
  
  void chunks_created(int chunk_size, int total_sz)
  {
    curr_chunk_size = chunk_size;
    total_size = total_sz;
    num_chunks = total_size / chunk_size;
    num_chunks += (num_chunks * chunk_size < total_size ? 1 : 0);
  }
  void new_chunk(int ssz) {
    curr_chunk_size = ssz;
    ++curr_chunk;
  }
  void end_chunk() {
    elements_handled += curr_chunk_size;
    std::cerr << "Handled " << elements_handled
	      << " elements of " << total_size
	      << " chunk #" << curr_chunk
	      << std::endl;
  }
};


/*! Iterator over the chunks of a sequence

 */
/*
template<class FlatIt>
class chunk_iterator {
  
  typedef FlatIt flat_iterator;
  typedef range<flat_iterator> value_type;

  flat_iterator b,e,e_sequence;
  int chunk_size;
public:
  

  value_type operator*() const
  { return valuetype(b,e); }

  self& operator++() {
    if(e != e_sequence) {
      b += cs;
      e = (e+cs <= e_sequence ? e+cs : e_sequence);
    }
    else {
      b = e_sequence;
    }
    return *this;
  }

  
};

*/

/*! chunked_iterator divides a sequence into equal-size chunks 
    
    chunked_iterator implements the segmented_iterator interface

    INCOMPLETE.

 */

template<class FlatIt, class Observer>
class chunked_iterator {
public:
  typedef chunked_iterator<FlatIt,Observer> self;
  
  typedef FlatIt segment_iterator;
  typedef FlatIt local_iterator;

  typedef std::iterator_traits<local_iterator> lt;
  typedef typename lt::value_type value_type;
  typedef progress_observer       observer_type;
  
private:
  typedef typename lt::difference_type difference_type; 
  difference_type chunk_size;
  mutable observer_type obs;
  segment_iterator curr_seg,   end_seg;
  local_iterator   curr_loc, end_loc;
public:
  chunked_iterator(segment_iterator b, segment_iterator e, int cs, Observer f)
    : curr_seg(b), end_seg(e), chunk_size(cs), obs(f)
  {
    update_local();
    obs.chunks_created(chunk_size, end_seg - curr_seg);
  }

  //  Hierarchical functions
  segment_iterator segment()      const { return curr_seg; }
  segment_iterator end_segment()  const { return end_seg; }
  local_iterator   local()        const { return curr_loc; }

  local_iterator begin_local(segment_iterator s) const { return s; }
  local_iterator end_local  (segment_iterator s) const {
    return s + std::min(chunk_size, std::distance(s,end_seg));
  }
  segment_iterator next_segment(segment_iterator s) const
  {
    obs.end_chunk();
    int sz = std::min(chunk_size, std::distance(s,end_seg));
    if(sz != 0)
      obs.new_chunk(sz);    
    return end_local(s);
  }

  // Flat functions
  value_type const& operator*() const { return *curr_loc; }
  value_type      & operator*()       { return *curr_loc; }

    
  self& operator++() {
    ++curr_loc;
    if(curr_loc == end_loc) {
      obs.end_chunk();
      curr_seg = (curr_seg + chunk_size <= end_seg ? curr_seg + chunk_size : end_seg); 
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
      &&   (curr_seg == end_seg || curr_loc == rhs.curr_loc);
  }
  //! true if *this < rhs
  bool less(self const& rhs) const {
    return (curr_seg <  rhs.curr_seg)
        || (curr_seg == rhs.curr_seg && curr_loc < rhs.curr_loc);
  }


private:

  void update_local() {
    if(curr_seg != end_seg) {
      curr_loc = curr_seg;
      end_loc  = std::min(end_seg, curr_seg + chunk_size); // (curr_seg + chunksize <= end_seg ? curr_seg + chunksize : end_seg);
      obs.new_chunk(end_loc - curr_loc);
    }
  }
  
};

template<class FlatIt, class Obs>
bool operator==(chunked_iterator<FlatIt,Obs> const& lhs,
		chunked_iterator<FlatIt,Obs> const& rhs)
{ return lhs.equal(rhs); }


template<class FlatIt, class Obs>
bool operator!=(chunked_iterator<FlatIt,Obs> const& lhs,
		chunked_iterator<FlatIt,Obs> const& rhs)
{ return ! (lhs == rhs); }

template<class FlatIt, class Obs>
struct is_segmented<chunked_iterator<FlatIt,Obs> > { typedef true_type res; };


#endif
