#include <vector>
#include "chunked-iterator.h"
#include "accumulate.h"


float prodvv_chunked_hier(std::vector<float> & v, int chunk_size)
{
  using namespace std;
  progress_observer obs;
  //int chunk_size = 1024;
  typedef chunked_iterator<vector<float>::iterator, progress_observer> chit;
  chit b(v.begin(),v.end(), chunk_size, obs);
  chit e(v.end(),  v.end(), chunk_size, obs);
  return accumulate_h(b,e,1.0f, multiplies<float>());
}
