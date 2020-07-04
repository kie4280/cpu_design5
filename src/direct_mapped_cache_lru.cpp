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

int read_system2(unsigned int address) {
  int cycles = 1;
  cache_content unused;
  bool read2 = sys2_1.read(address, unused);
  if (!read2) {
    cycles += 1 + 100 + 1 + 2;  // read mem and write cache
  }
  cycles += 2;  // read
  cycles += 1;  // send word

  return cycles;
}

int read_system3(unsigned int address) {
  int cycles = 1;
  cache_content unused1, unused2;
  bool read1 = sys3_1.read(address, unused1);
  bool read2 = sys3_2.read(address, unused2);
  if (!read1) {
    cycles += 4 * (1 + 10 + 1 + 1);  // read mem and write cache
    if (!read2) {
      cycles += 32 * (1 + 100 + 1 + 10);
    }
  }
  cycles += 1;  // read L1
  cycles += 1;  // send word
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

  int cycles_sys1 = 0, cycles_sys2 = 0, cycles_sys3 = 0;
  int exec_cycles = 2;

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      for (int k = 0; k < n; ++k) {
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        cycles_sys1 += read_system1(baseC + 4 * (i * p + j)) +
                       read_system1(baseA + 4 * (i * n + k)) +
                       read_system1(baseB + 4 * (k * p + j)) +
                       read_system1(baseC + 4 * (i * p + j));
        cycles_sys2 += read_system2(baseC + 4 * (i * p + j)) +
                       read_system2(baseA + 4 * (i * n + k)) +
                       read_system2(baseB + 4 * (k * p + j)) +
                       read_system2(baseC + 4 * (i * p + j));
        cycles_sys3 += read_system3(baseC + 4 * (i * p + j)) +
                       read_system3(baseA + 4 * (i * n + k)) +
                       read_system3(baseB + 4 * (k * p + j)) +
                       read_system3(baseC + 4 * (i * p + j));
        exec_cycles += 22;
        if (k == n - 1) exec_cycles += 2;
      }
      exec_cycles += 5;
      if (j == p - 1) exec_cycles += 2;
    }
    exec_cycles += 5;
  }
  exec_cycles += 3; // plus one after exit

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      output << matrixC[i][j] << " ";
    }
    output << endl;
  }

  output << exec_cycles << " " << cycles_sys1 << " " << cycles_sys2 << " "
       << cycles_sys3 << endl;

  fs.close();
  output.close();

  return 0;
}
