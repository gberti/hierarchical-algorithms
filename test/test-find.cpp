
#include "aux/print-hierarchical.h"
#include "hier-algorithms/find.h"


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

  segit find_1   = std         ::find(b,e,1);
  segit find_1_h = hierarchical::find(b,e,1);

  cout << "*find_1 (std)  = " << *find_1   << "\n";
  cout << "*find_1 (hier) = " << *find_1_h << "\n";
  
}
