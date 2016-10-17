#include <vector>
#include <numeric>

float sum(std::vector<float>& v)
{
  return std::accumulate(v.begin(), v.end(), 0.0f);
}

