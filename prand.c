// jdh 9-20-23
// pseudo-random number generator

#include <stdio.h>

int prand() {
  static int p = 1;
  const int a = 105491;
  const int b = 733;
  const int c = 6619;
  const int base = 100;
  int val;
  p = p*a + b;
  p = p % c;
  val = p % base;
  return val;
}
