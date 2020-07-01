// Author: 0710012 何權祐, 0710018 張宸愷
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct cache_content {
  bool v, dirty;
  unsigned int tag;
  unsigned int count = 0;
  // unsigned int	data[16];
};

class Cache {
 private:
  double log2(double n) {
    // log(n) / log(2) is log2.
    return log(n) / log(double(2));
  }

 public:
  const int K = 1024;
  int offset_bit;
  int set_bit;
  int index_bit;
  int line;
  int n_way;
  cache_content **cache;

  Cache(int cache_size, int block_size, int n_way) {
    offset_bit = (int)log2(block_size);
    set_bit = (int)log2(n_way);
    index_bit = (int)log2(cache_size / block_size);
    line = (cache_size >> offset_bit);  // rows
    Cache::n_way = n_way;

    cache = new cache_content *[line >> set_bit];
    for (int a = 0; a<line>> set_bit; ++a) {
      cache[a] = new cache_content[n_way];
    }

    for (int j = 0; j < (line >> set_bit); j++) {
      for (int i = 0; i < n_way; ++i) {
        cache[j][i].v = false;
        cache[j][i].dirty = false;
      }
    }
  }

  bool access(unsigned int x) {
    // cout << hex << x << " ";
    unsigned int tag, index;
    index = (x >> (offset_bit)) & ((line - 1) >> set_bit);
    tag = x >> (index_bit + offset_bit - set_bit);

    // unsigned int set_index = index >> set_bit;

    unsigned int least_used = 0;
    int least_index = 0;
    bool hit = false;

    for (int b = 0; b <= n_way; ++b) {
      if (b < n_way && cache[index][b].v && cache[index][b].tag == tag) {
        cache[index][b].v = true;  // hit
        cache[index][b].count = 0;
        hit = true;
        break;
      }

      else if (b == n_way) {
        for (int j = 0; j < n_way; ++j) {
          if (cache[index][j].v == false) {
            least_index = j;
            break;
          }
          if (least_used < cache[index][j].count) {
            least_used = cache[index][j].count;
            least_index = j;
          }
        }
        cache[index][least_index].count = 0;
        cache[index][least_index].v = true;
        cache[index][least_index].tag = tag;
      }
    }

    for (int b = 0; b < n_way; ++b) {
      cache[index][b].count++;
    }
    // cout <<dec<< miss << " " << accesses << endl;
  }

  ~Cache() {
    for (int a = 0; a<line>> set_bit; ++a) {
      delete[] cache[a];
    }
    delete[] cache;
  }
};

Cache sys1_1(512, 32, 8);
Cache sys2_1(512, 32, 8);
Cache sys3_1(128, 16, 8), sys3_2(4096, 128, 8);


int access_system1(unsigned int address) {
  bool read1 = sys1_1.read(address);
  if(read1)

}

int main(int argc, char **argv) {
  

  unsigned int baseA, baseB, baseC;
  int m, n, p;
  fstream fs;
  if (argc <= 2) {
    return -1;
  }

  fs.open(string(argv[1]), ios::in);



  fs.close();

  return 0;
}
