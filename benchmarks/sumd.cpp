#include <deque>
#include <numeric>

float sum(std::deque<float>& d)
{
  return std::accumulate(d.begin(), d.end(), 0.0f);
}

