#ifndef PRINT_HIERARCHICAL_H
#define PRINT_HIERARCHICAL_H

#include "hier-algorithms/hierarchical-iterator-traits.h"
#include <iostream>


template<class InputIterator>
void print(InputIterator b, InputIterator e)
{
  while(b != e) {
    std::cout << *b << " " << std::flush;
    ++b;
  }
}

template<class InputIterator>
void print_seg(InputIterator b, InputIterator e);


template<class InputIterator>
void print_seg(InputIterator b, InputIterator e, false_type)
{
  print(b,e);
}

template<class InputIterator>
void print_seg(InputIterator b, InputIterator e, true_type)
{
  typedef typename InputIterator::segment_iterator segit;
  typedef typename InputIterator::local_iterator   locit;
  segit bs = b.segment(), es = e.segment(); // es = last or one-past-last segment
  
  if(bs != b.end_segment()) {
    locit bl = b.local();
    locit el = (bs == es ? e.local() : b.end_local(bs));  // (bs == es ? e.local() : bs->end());
    // suppress execution if [bl,el) is empty (happens only if [b,e) is empty)
    if(bl != el) {
      std::cout << "Segment: ";
      print_seg(bl, el);
      std::cout << std::endl; 
    }
    if(bs == es)
      return;
    //++bs;
    bs = b.next_segment(bs);
  }
  
  while(bs != es) {
    std::cout << "Segment: ";
    print_seg(b.begin_local(bs),b.end_local(bs));  //(bs->begin(), bs->end());
    std::cout << std::endl;
    // ++bs;
    bs = b.next_segment(bs);
  }
  if(bs != b.end_segment()) {
    // now bs == es != b.end_segment()
    locit bl = b.begin_local(bs); // bs->begin();
    locit el = e.local();
    // suppress execution if [bl,el) is empty: This occurs whenever e is on a segment boundary!
    if(bl != el) {
      std::cout << "Segment: ";
      print_seg(bl, el);
      std::cout << std::endl;
    }
  }
}

template<class InputIterator>
void print_seg(InputIterator b, InputIterator e)
{ print_seg(b,e, typename is_segmented<InputIterator>::res()); }



#endif
