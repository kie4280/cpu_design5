// Author: 0710012 何權祐, 0710018 張宸愷

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "cache.h"

using namespace std;

Cache sys1_1(512, 32, 8);
Cache sys2_1(512, 32, 8);
Cache sys3_1(128, 16, 8), sys3_2(4096, 128, 8);

int read_system1(unsigned int address) {
  int cycles = 1;  // send address
  cache_content unused;
  bool read1 = sys1_1.read(address, unused);
  if (!read1) {
    cycles += 8 * (1 + 100 + 1 + 2);  // read mem and write cache
  }

  cycles += 2;  // read

  cycles += 1;  // send word

  return cycles;
}

int run_system1(unsigned int baseA, unsigned int baseB, unsigned int baseC,
                int m, int n, int p) {
  int cycles = 0;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
    }
  }

  return cycles;
}

int main(int argc, char **argv) {
  unsigned int baseA, baseB, baseC;
  int m, n, p;

  fstream fs, output;

  if (argc <= 2) {
    return -1;
  }

  fs.open(string(argv[1]), ios::in);
  output.open(string(argv[2]), ios::out | ios::trunc);
  string ll;
  getline(fs, ll);
  sscanf(ll.c_str(), "%x %x %x %d %d %d", &baseA, &baseB, &baseC, &m, &n, &p);

  int **matrixA = new int *[m];
  for (int i = 0; i < m; ++i) {
    matrixA[i] = new int[n];
  }
  int **matrixB = new int *[n];
  for (int i = 0; i < n; ++i) {
    matrixB[i] = new int[p];
  }
  int **matrixC = new int *[m];
  for (int i = 0; i < m; ++i) {
    matrixC[i] = new int[p];
  }

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      fs >> matrixA[i][j];
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < p; ++j) {
      fs >> matrixB[i][j];
    }
  }
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      matrixC[i][j] = 0;
    }
  }

  int cycles_sys1 = 0;

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      for (int k = 0; k < n; ++k) {
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        cycles_sys1 += read_system1(baseC + 4*(i*p+j)) +
                       read_system1(baseA + 4*(i*n+k)) +
                       read_system1(baseB + 4*(k*p+j));
      };
    }
  }

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      cout << matrixC[i][j] << " ";
    }
    cout << endl;
  }

  cout << cycles_sys1 << endl;

  fs.close();
  output.close();

  return 0;
}
