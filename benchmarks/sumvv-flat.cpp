
#include "segmented-iterator.h"

#include <vector>
#include <numeric>


float sumvv_flat(std::vector<std::vector<float> > & vv)
{
  using namespace std;
  typedef vector<float> vf;
  typedef hierarchical::segmented_iterator<vector<vf>::iterator, vf::iterator> segit;
  segit b(vv.begin(),vv.end());
  segit e(vv.end(), vv.end());
  return std::accumulate(b,e,0.0f, plus<float>());
}
