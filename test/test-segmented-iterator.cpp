
#include "iterators/segmented-iterator.h"

#include "aux/print-hierarchical.h"
#include "hier-algorithms/accumulate.h"


#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>


int main() {
  using namespace std;

  enum { Optimize = 0, NoOptimise = 1 };
  typedef hierarchical::segmented_iterator<vector<vector<int> >::iterator, vector<int>::iterator, Optimize> segit; // Attn NoOptimise will not work for negative distances below

  vector<vector<int> > vv(2);
  vv[0] = {0,1,2};
  vv[1] = {3,4,5};
  segit b(vv.begin(),vv.end());
  segit e(vv.end(),  vv.end());
  assert(b == segit::sbegin(vv));
  assert(e == segit::send(vv));
  
  cout << "Flat:\n";
  print(b,e);
  cout << std::endl;

  cout << "Hierarchical:\n";
  print_seg(b,e);
  
  {
    segit b(vv.begin(),   vv.end(), vv.begin()->begin());
    segit e(vv.begin()+1, vv.end(), (vv.begin()+1)->begin());
    cout << "Hierarchical (b,e):\n";
    print_seg(b,e);
    cout << "Hierarchical (b,b):\n";
    print_seg(b,b);
    cout << "Hierarchical (e,e):\n";
    print_seg(e,e);
    
    cout << "Flat:\n"; 
    print(b,e);
    print(b,b);
    print(e,e);

  }
  
  {
    segit b(vv.begin(),   vv.end(), vv.begin()->begin());
    segit e(vv.begin(),   vv.end(), vv.begin()->begin()+1);
    cout << "Hierarchical (b,e):\n";
    print_seg(b,e);
    cout << "Hierarchical (b,b):\n";
    print_seg(b,b);
    cout << "Hierarchical (e,e):\n";
    print_seg(e,e);

    cout << "Flat:\n"; 
    print(b,e);
    print(b,b);
    print(e,e);
  }

  cout << "\n";
  cout << "Flat:\n";
  print(b,e);
  cout << "\n";
  using std::distance;
  using std::advance;
  
  int dist = distance(b,e);
  cout << "distance(b,e) = " << dist << "\n";
  cout << "distance(e,b) = " << distance(e,b) << "\n";
  cout << "distance(b,b) = " << distance(b,b) << "\n";
  cout << "distance(e,e) = " << distance(e,e) << "\n";

 
  auto b1 = b;
  cout << "b1-b = " << distance(b,b1) << "  "
       << "b-b1 = " << distance(b1,b)
       << "\n";;

  int i = 0;
  auto b2 = b;
  while(b1 != e) {
    cout << "b1-b = " << distance(b,b1) << "  "
	 << "b-b1 = " << distance(b1,b);
    cout << "  i=" << i << " *b1 = " << *b1
	 << " b1=" << b1
	 << " b2=" << b2
	 << "\n";
    advance(b1, 1); ++b2;
    ++i;
  }
  if(b1 == e)
    cout << "b1 == e: b1 = " << b1 << " b2=" << b2<< "\n";
  
  while(b1 != b) {
    i--;
    advance(b1,-1);
    cout << "  i=" << i <<  " *b1 = " << *b1 << "\n";
  }

  i = 0;
  while(b1 != e) {
    advance(b1,3);
    i += 3;
    if(b1 != e)
      cout << "  i=" << i <<  " *b1 = " << *b1 << "\n";
  }

  while(b1 != b) {
    i -= 3;
    advance(b1,-3);
    cout << "  i=" << i <<  " *b1 = " << *b1 << "\n";
  }


  cout << "Testing upper_bound: " << endl;
  vector<int> vf = {0,1,2,3,4,5};
  cout << "end: " << e << "\n";
  for(int bd = 0; bd <= 6; ++bd) {
    cout << "upper_bound(" << bd << ") = ";

    auto ibd = upper_bound(b,e,bd);
    if(ibd == e)
      cout << "e";
    else
      cout << *ibd << " " << ibd;

    cout << " (flat: ";
    auto ibdf = std::upper_bound(vf.begin(),vf.end(),bd);
    if(ibdf == vf.end())
      cout << "e" << ")";
    else
      cout << *ibdf << ")";

    cout  << "\n";
  }

  
  cout << "Testing lower_bound: " << endl;
  for(int bd = 0; bd <= 6; ++bd) {
    cout << "lower_bound(" << bd << ") = ";
    auto ibd = std::lower_bound(b,e,bd);
    if(ibd !=e)
      cout << *ibd;
    else
      cout << "e";
    cout << "\n";
  }
  
} 

