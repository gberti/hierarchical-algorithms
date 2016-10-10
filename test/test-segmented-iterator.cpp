
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
  int i;
  i = std::accumulate(b,e,0);
  cout << "sum (f) = " << i << "\n";
  i = accumulate_h(b,e,int(0),std::plus<int>());
  cout << "sum (h) = " << i << "\n";
  
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

    cout << "Sum:\n";
    cout << "[b,e): "
	 << " (f): " << std::accumulate(b,e,int(0))
	 << " (s): " << accumulate_h(b,e,int(0),std::plus<int>()) << "\n";
    cout << "[b,b): "
	 << " (f): " << std::accumulate(b,b,int(0))
	 << " (s): " << accumulate_h(b,b,int(0),std::plus<int>()) << "\n";
    cout << "[e,e): "
	 << " (f): " << std::accumulate(e,e,int(0))
	 << " (s): " << accumulate_h(e,e,int(0),std::plus<int>()) << "\n";
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
