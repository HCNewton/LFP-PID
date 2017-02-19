#include <stdio.h>

// calculate all values within an array
int sum_array(int a[], int num_elements) {
  int sum = 0;
  for (int i = 0; i < num_elements; i++) {
    sum = sum + a[i];
  }
  return (sum);
}

// convolution algorithm
int main() {
  int A[200];            // data sample
  int value = 1;         // fill data sample with this value
  int B[3] = {1, 0, -1}; // taps
  int lenA = 200;        // length of sample array
  int lenB = 3;          // number of taps

  int nconv; // convoluted array length
  int i, j, k;
  int tmp;

  // fill data sample
  for (i = 0; i < lenA; i++) {
    if (i % 2 == 0)
    {A[i] = 10;}
    else A[i] = value;
  }

  // allocated convolution array
  nconv = lenA + lenB - 1;
  int C[nconv];

  // convolution process
  for (i = 0; i < nconv; i++) {
    k = i;
    tmp = 0;
    for (j = 0; j < lenB; j++) {
      if (k >= 0 && k < lenA) { // no values outside the array
        tmp = tmp + (A[k] * B[j]);
      }

      k = k - 1;
      C[i] = tmp;
    }
  }

  // print convolution array
  for (i = 0; i < (sizeof(C) / sizeof(C[0])); i++) {
    printf("%d\n", C[i]);
  }
  // print sum of nconv
  int sum = sum_array(C, (sizeof(C) / sizeof(C[0])));
  printf("Sum:\t%d\n", sum);
  // print length of array
  printf("Size:\t%d\n", (sizeof(C) / sizeof(C[0])));

  return 0;
}
