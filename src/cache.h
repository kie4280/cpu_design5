#ifndef CACHE_STORAGE
#define CACHE_STORAGE

struct cache_content {
  bool v = false, dirty = false;
  unsigned int tag = 1234;
  unsigned int count = 0;
  // unsigned int	data[16];
};

class Cache {
 private:
  double log2(double n);

 public:
  const int K = 1024;
  int offset_bit;
  int set_bit;
  int index_bit;
  int line;
  int n_way;
  cache_content **cache;

  Cache(int cache_size, int block_size, int n_way);

  bool read(unsigned int x, cache_content &swapout);

  bool write(unsigned int x, cache_content &swapout);

  ~Cache();
};

#endif