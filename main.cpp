#include <iostream>
#include "mymatrix.h"

//mymatrix(int R, int C);

int main() {
  mymatrix<int>  M;
 
  int R = 4;
  int C = 4;
 
  //
  // now let's update the values:
  //
  for (int r = 0; r < R; ++r)
    for (int c = 0; c < C; ++c)
      M(r, c) = (-9 * r * C) + (2*c) - 1;
  //
  // let's now let's copy the matrix, and make sure it's a copy:
  //

  mymatrix<int> M2 = M;
  M2._output();

return 0;

}