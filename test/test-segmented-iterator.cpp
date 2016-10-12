
#include "aux/print-hierarchical.h"
#include "hier-algorithms/accumulate.h"

#include "iterators/segmented-iterator.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>


int main() {
  using namespace std;

  typedef segmented_iterator<vector<vector<int> >::iterator, vector<int>::iterator> segit;

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

}
