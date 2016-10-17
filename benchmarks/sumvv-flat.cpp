#include <vector>
#include "segmented-iterator.h"
#include "accumulate-flat.h"


float sumvv_flat(std::vector<std::vector<float> > & vv)
{
  using namespace std;
  typedef vector<float> vf;
  typedef segmented_iterator<vector<vf>::iterator, vf::iterator> segit;
  segit b(vv.begin(),vv.end());
  segit e(vv.end(), vv.end());
  return accumulate_flat(b,e,0.0f, plus<float>());
}
