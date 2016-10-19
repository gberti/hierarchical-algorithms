
#include "aux/print-hierarchical.h"
#include "hier-algorithms/foreach.h"

#include "iterators/segmented-iterator.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>


int main() {
  using namespace std;

  typedef hierarchical::segmented_iterator<vector<vector<int> >::iterator,
					   vector<int>::iterator> segit;

  vector<vector<int> > vv(2);
  vv[0] = {1,2,3};
  vv[1] = {4,5,6};
  segit b(vv.begin(),vv.end());
  segit e(vv.end(), vv.end());
  assert(b == segit::sbegin(vv));
  assert(e == segit::send(vv));
  
  cout << "Flat:\n";
  print(b,e);
  cout << std::endl;

  cout << "Hierarchical:\n";
  print_seg(b,e);
  int i = 0;
  auto f = [&i](int k) { i += k; };
  std::for_each(b,e,f);
  cout << "sum (f) = " << i << "\n";
  i = 0;
  hierarchical::for_each(b,e,f);
  cout << "sum (h) = " << i << "\n";
  
 
}
