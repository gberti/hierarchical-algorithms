
#include "segmented-iterator.h"
#include "aux/timer.h"


#include <deque>
#include <vector>
#include <algorithm>





template<class It>
void fill(It b, It e) {
  int i = 1;
  while(b != e) {
    *b = 1.0; //+1.0/(i*i);
    ++i; ++b;
  }
}

void dummyv(std::vector<float>& v);
void dummyd(std::deque <float>& d);

float sum(std::vector<float>& v);
float sum(std::deque<float> & d);
float sumvv_flat(std::vector<std::vector<float> > & vv);
float prodvv_flat(std::vector<std::vector<float> > & vv);
float sumvv_hier(std::vector<std::vector<float> > & vv);
float sumvv_chunked_hier(std::vector<float>& v, int chunks_size);
float prodvv_hier(std::vector<std::vector<float> > & vv);
float prodvv_chunked_hier(std::vector<float>& v, int chunks_size);

float sum_cstyle(float* b, float* e);
float prod_cstyle(float* b, float* e);

int main() {
  
  // int chunk = 512/sizeof(float); // deque chunk size
  int chunk = 1024;
  int f = 500;
  int nchunks = (1024*f)/chunk;
  int N = chunk*nchunks;
  int Niter = (10000*100)/f;
  std::vector<float> v(N);
  std::deque<float>  d(N);
  float * a = new float[N];
  fill(v.begin(), v.end());
  fill(d.begin(), d.end());
  fill(a, a+N);

  typedef std::vector<float> vf;
  typedef hierarchical::segmented_iterator<std::vector<vf>::iterator, vf::iterator> segit;
  std::vector<vf> vv(nchunks);
  for(vf& v : vv) {
    v.resize(chunk);
  }
  segit b(vv.begin(),vv.end());
  segit e(vv.end(),  vv.end());
  fill(b,e);
  
  double tc = aux::start_timer();
  float sc = 0;
  for(int i = 0; i < Niter; ++i) {
    // sc += sum_cstyle(&(*v.begin()), &(*v.end()));
    sc += sum_cstyle(a,a+N);
  }
  aux::stop_timer(tc);

  double tpc = aux::start_timer();
  float pc = 1;
  for(int i = 0; i < Niter; ++i) {
    // sc += sum_cstyle(&(*v.begin()), &(*v.end()));
    pc *= prod_cstyle(a,a+N);
  }
  aux::stop_timer(tpc);

  

  double tv = aux::start_timer();
  float sv = 0;
  for(int i = 0; i < Niter; ++i) {
    //sv += std::accumulate(v.begin(), v.end(), 0.0f);
    sv += sum(v);
    // dummyv(v);
  }
  aux::stop_timer(tv);

  double td = aux::start_timer();
  float sd = 0;
  for(int i = 0; i < Niter; ++i) {
    // sd += std::accumulate(d.begin(), d.end(), 0.0f);
    sd += sum(d);
    dummyd(d);
  }
  aux::stop_timer(td);

  double tvv_hier = aux::start_timer();
  float svv_hier = 0;
  for(int i = 0; i < Niter; ++i) {
    svv_hier += sumvv_hier(vv);
    // dummyd(d);
  }
  aux::stop_timer(tvv_hier);

  double tvv_flat = aux::start_timer();
  float svv_flat = 0;
  for(int i = 0; i < Niter; ++i) {
    // sd += std::accumulate(d.begin(), d.end(), 0.0f);
    svv_flat += sumvv_flat(vv);
    // dummyd(d);
  }
  aux::stop_timer(tvv_flat);
  
  double tpvv_flat = aux::start_timer();
  float pvv_flat = 1.0;
  for(int i = 0; i < Niter; ++i) {
    // sd += std::accumulate(d.begin(), d.end(), 0.0f);
    pvv_flat *= prodvv_flat(vv);
    // dummyd(d);
  }
  aux::stop_timer(tpvv_flat);


  
  double tvv_chunked = aux::start_timer();
  float svv_chunked = 0;
  for(int i = 0; i < Niter; ++i) {
    svv_chunked += sumvv_chunked_hier(v, chunk*100);
    // dummyd(d);
  }
  aux::stop_timer(tvv_chunked);

  double tpvv_hier = aux::start_timer();
  float pvv_hier = 1;
  for(int i = 0; i < Niter; ++i) {
    pvv_hier *= prodvv_hier(vv);
    // dummyd(d);
  }
  aux::stop_timer(tpvv_hier);

  

  double tpvv_chunked = aux::start_timer();
  float pvv_chunked = 1;
  for(int i = 0; i < Niter; ++i) {
    pvv_chunked *= prodvv_chunked_hier(v, chunk*f);
    // dummyd(d);
  }
  aux::stop_timer(tpvv_chunked);

  

  
  std::cout << "Time C-array: " << tc       << " sum=" << sc << "\n"
	    << "Time vector:  " << tv       << " sum=" << sv << "\n"
	    << "Time deque:   " << td       << " sum=" << sd << "\n"
	    << "Time vv flat: " << tvv_flat << " sum=" << svv_flat << "\n"
    	    << "Time vv hier: " << tvv_hier << " sum=" << svv_hier << "\n"
    	    << "Time vv chunk: " << tvv_chunked << " sum=" << svv_chunked << "\n"
	    << "Time * C-array:  " << tpc           << " prod=" << pc << "\n"
    	    << "Time * vv flat:  " << tpvv_flat     << " prod=" << pvv_flat << "\n"
    	    << "Time * vv hier:  " << tpvv_hier     << " prod=" << pvv_hier << "\n"
    	    << "Time * vv chunk: " << tpvv_chunked  << " prod=" << pvv_chunked << "\n";
}
