// Author: 0710012 何權祐, 0710018 張宸愷
#include <math.h>
#include <stdio.h>

#include <iostream>

using namespace std;

struct cache_content {
  bool v;
  unsigned int tag;
  unsigned int count = 0;
  // unsigned int	data[16];
};

const int K = 1024;

double log2(double n) {
  // log(n) / log(2) is log2.
  return log(n) / log(double(2));
}

void simulate(int cache_size, int block_size, int n_way) {
  unsigned int tag, index, x;

  int offset_bit = (int)log2(block_size);
  int set_bit = (int)log2(n_way);
  int index_bit = (int)log2(cache_size / block_size);

  int line = (cache_size >> offset_bit);  // rows
  int accesses = 0, miss = 0;

  cache_content **cache = new cache_content *[line >> set_bit];
  for (int a = 0; a<line>> set_bit; ++a) {
    cache[a] = new cache_content[n_way];
  }

  // cout << "cache line: " << line << endl;

  for (int j = 0; j < (line >> set_bit); j++) {
    for (int i = 0; i < n_way; ++i) {
      cache[j][i].v = false;
    }
  }

  FILE *fp = fopen("test/LU.txt", "r");  // read file

  while (fp != 0 && fscanf(fp, "%x", &x) != EOF) {
    // cout << hex << x << " ";
    index = (x >> (offset_bit)) & ((line - 1)>>set_bit);
    tag = x >> (index_bit + offset_bit - set_bit);
    
    // unsigned int set_index = index >> set_bit;
    
    unsigned int least_used = 0;
    int least_index = 0;

    for (int b = 0; b <= n_way; ++b) {
      if (b < n_way && cache[index][b].v &&
          cache[index][b].tag == tag) {
        cache[index][b].v = true;  // hit
        cache[index][b].count = 0;
        
        break;
      }

      else if (b == n_way) {
        for (int j = 0; j < n_way; ++j) {
          if(cache[index][j].v == false) {
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
        miss++;
      }

      
    }

    for(int b=0; b<n_way; ++b) {
      cache[index][b].count++;
    }
    // cout <<dec<< miss << " " << accesses << endl;
    accesses++;
  }
  if (fp != 0) {
    fclose(fp);
  }

  // cout <<dec<< miss << " " << accesses << endl;
  cout << (double)miss / accesses;
  for (int a = 0; a<line>> set_bit; ++a) {
    delete[] cache[a];
  }
  delete[] cache;
}

int main(int argc, char **argv) {
  // Let us simulate 4KB cache with 16B blocks
  for (int a = 0; a <= 6; ++a) {
    for (int b = 0; b < 4; ++b) {
      simulate((4 << b * 2) * K, 64, 1 << a);
      cout << " ";
      // cout << (4<<b*2) << "           " <<endl;
    }
    cout << endl;
  }
}
