// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#ifndef SOLUTIONS_GCD_H_
#define SOLUTIONS_GCD_H_

// @include
long long GCD(long long x, long long y) {
  if (x == 0) {
    return y;
  } else if (y == 0) {
    return x;
  } else if (!(x & 1) && !(y & 1)) {  // x and y are even integers
    return GCD(x >> 1, y >> 1) << 1;
  } else if (!(x & 1) && y & 1) {  // x is even integer, and y is odd integer
    return GCD(x >> 1, y);
  } else if (x & 1 && !(y & 1)) {  // x is odd integer, and y is even integer
    return GCD(x, y >> 1);
  } else if (x > y) {  // both x and y are odd integers, and x > y
    return GCD(x - y, y);
  }
  return GCD(x, y - x);  // both x and y are odd integers, and x <= y
}
// @exclude

long long another_GCD(long long a, long long b) {
  if (b) {
    while ((a %= b) && (b %= a));
  }
  return a + b;
}
#endif  // SOLUTIONS_GCD_H_
