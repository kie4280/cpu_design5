// Author: 0710012 何權祐, 0710018 張宸愷

#include <fstream>
#include <iostream>
#include <string>
#include "cache.h"

using namespace std;


Cache sys1_1(512, 32, 8);
Cache sys2_1(512, 32, 8);
Cache sys3_1(128, 16, 8), sys3_2(4096, 128, 8);

int access_system1(unsigned int address) {
  int cycles = 1;
  bool read1 = sys1_1.check(address);
  if (read1) {
    cycles += 2; 
  }
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
