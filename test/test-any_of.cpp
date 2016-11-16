
#include "aux/print-hierarchical.h"
#include "hier-algorithms/any_of.h"

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

  auto greater_3 = [](int i) { return i > 3; };
  
  bool any_greater_3   = std         ::any_of(b,e,greater_3);
  bool any_greater_3_h = hierarchical::any_of(b,e,greater_3);

  cout << "any_of > 3 (std)   = " <<  any_greater_3 << "\n";
  cout << "any_of > 3 (hier)  = " <<  any_greater_3_h << "\n";

  auto greater_7 = [](int i) { return i > 7; };
  bool any_greater_7   = std         ::any_of(b,e,greater_7);
  bool any_greater_7_h = hierarchical::any_of(b,e,greater_7);

  cout << "any_of > 7 (std)   = " <<  any_greater_7 << "\n";
  cout << "any_of > 7 (hier)  = " <<  any_greater_7_h << "\n";
  
}
