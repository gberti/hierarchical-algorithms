

float prod_cstyle(float* b, float* e)
{
  float res = 1.0;
  while(b != e) {
    res *= *b;
    ++b;
  }
  return res;
}
