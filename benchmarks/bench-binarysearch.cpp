
/*! \file Comparison of binary search algorithms of hierarchical sequences with and without optimised primitives

  The file compares the performance of lower_bound on an hierarchical sequence (vector of vector) 
  for different segment sizes to flat versions:
  (a) hierarchical, default implementation of advance and distance 
      (for bidirectional iterators, i.e. linear), 
  (b) hierarchical, optimized implementation of advance and distance
  (c) flat (vector)
  (d) flat (deque)

  \note This code currently relies on overloading advance and distance in namespace std, see the comments in segmented-iterator.h
  This is brittle as it relies on the sequence of includes 
  (header segmented-iterator.h must come before any use of std::advance and std::distance in a header)
 */

#include "iterators/segmented-iterator.h"

#include "aux/timer.h"

#include <vector>
#include <deque>
#include <algorithm>


int main()
{
  using std::vector;
  using std::deque;
  enum { Optimize = 0, NoOptimize = 1 };
  typedef hierarchical::segmented_iterator<vector<vector<int> >::iterator,
					   vector<int>::iterator, Optimize>   segit_opt;
  typedef hierarchical::segmented_iterator<vector<vector<int> >::iterator,
					   vector<int>::iterator, NoOptimize> segit_noopt;


 std::cout << "# Segments; # Elements per Segment; Flat vector (s); Flat deque (s); Optimised (s); Default (s)\n";
 
 int nloops = 4;
 int Nelems = 256*256;
 
 for(int Nsegs = 2; Nsegs <= 256; Nsegs *=2) {
   int SegSz = Nelems / Nsegs;
  
   vector<vector<int> > vv(Nsegs);
   vector<int>          vf(Nelems);
   deque<int>           df(Nelems);
   int n = 0;
   // a sorted hierarchical container
   for(int i = 0; i < Nsegs; ++i) {
     vv[i].resize(SegSz);
     for(int j = 0; j < SegSz; ++j, ++n) {
       vv[i][j] = n;
       vf[n]    = n;
       df[n]    = n;
     }
   }

   double sum = 0;

   auto bf = vf.begin();
   auto ef = vf.end();   
   double t_flat = aux::start_timer();
   for(int k = 0; k < nloops; ++k) {
     for(n = 0; n < Nelems-1; ++n)
       sum += *std::lower_bound(bf,ef,n);
   }
   aux::stop_timer(t_flat);
   std::cerr << "sum = " << sum << "\n";

   auto bdf = df.begin();
   auto edf = df.end();   
   double t_dflat = aux::start_timer();
   for(int k = 0; k < nloops; ++k) {
     for(n = 0; n < Nelems-1; ++n)
       sum += *std::lower_bound(bdf,edf,n);
   }
   aux::stop_timer(t_dflat);
   std::cerr << "sum = " << sum << "\n";

   segit_opt b_opt(vv.begin(),vv.end());
   segit_opt e_opt(vv.end(),  vv.end());
   double    t_opt = aux::start_timer();
   sum = 0;
   for(int k = 0; k < nloops; ++k) {
     for(n = 0; n < Nelems-1; ++n)
       sum += *std::lower_bound(b_opt,e_opt,n);
   }
   aux::stop_timer(t_opt);
   std::cerr << "sum = " << sum << "\n";
   
   
   segit_noopt bn(vv.begin(),vv.end());
   segit_noopt en(vv.end(),  vv.end());
   double t_noopt = aux::start_timer();
   sum = 0;
   for(int k = 0; k < nloops; ++k) {
     for(n = 0; n < Nelems-1; ++n)
       sum += *std::lower_bound(bn,en,n);
   }
   aux::stop_timer(t_noopt);
   std::cerr << "sum = " << sum << "\n";
   

  
   std::cout
     << Nsegs << ";" << SegSz << ";" << t_flat/nloops << ";" << t_dflat/nloops << ";" << t_opt/nloops << ";" << t_noopt/nloops
     << std::endl;
 }
 return 0;
  
}
