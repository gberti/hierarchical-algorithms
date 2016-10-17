

float sum_cstyle(float* b, float* e)
{
  float res = 0.0;
  while(b != e) {
    res += *b;
    ++b;
  }
  return res;
}
