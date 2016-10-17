#include <vector>
#include "segmented-iterator.h"
#include "accumulate.h"


float sumvv_hier(std::vector<std::vector<float> > & vv)
{
  using namespace std;
  typedef vector<float> vf;
  typedef segmented_iterator<vector<vf>::iterator, vf::iterator> segit;
  segit b(vv.begin(),vv.end());
  segit e(vv.end(), vv.end());
  return accumulate_h(b,e,0.0f, plus<float>());
}
