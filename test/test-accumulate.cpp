

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


  int i;
  i = std::accumulate(b,e,0);
  cout << "sum (flat) = " << i << "\n";
  i = accumulate_h(b,e,int(0),std::plus<int>());
  cout << "sum (hier) = " << i << "\n";
  

}
