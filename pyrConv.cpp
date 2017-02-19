#include <stdio.h>
#include <vector>

float sum_array(std::vector<float> a, float num_elements) {
  float sum = 0;
  for (float i = 0; i < num_elements; i++) {
    sum = sum + a[i];
  }
  return (sum);
}

// convolution algorithm
std::vector<float> conv(std::vector<float> A, std::vector<float> B, float lenA,
                      float lenB) {
  float i = 0, j = 0, k = 0;
  float tmp = 0;
  float nconv = lenA + lenB - 1;
  std::vector<float> C(nconv);

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

  // return convolution array
  return (C);
}

// pyramid process
std::vector<float> pyramid(std::vector<float> add, float size) {
  float i, j = 0;
  std::vector<float> sum;

  while (i < size) {
    sum.push_back(add[j] + add[j + 1]);
    j = j + 2;
    i++;
  }

  return (sum);
}

std::vector<float> pyrConv(std::vector<float> data, std::vector<float> taps) {
  float dataSize = static_cast<float>(data.size());
  std::vector<float> data1 = pyramid(data, dataSize);
  float data1Size = static_cast<float>(data1.size());
  std::vector<float> data2 = pyramid(data1, data1Size);
  float data2Size = static_cast<float>(data2.size());
  std::vector<float> data3 = pyramid(data2, data2Size);
  float data3Size = static_cast<float>(data3.size());
  //std::vector<float> data4 = pyramid(data3, data3Size);

  float tapsSize = static_cast<float>(taps.size());
  std::vector<float> conv1 = conv(data, taps, dataSize, tapsSize);
  std::vector<float> conv2 = conv(data1, taps, data1Size, tapsSize);
  std::vector<float> conv3 = conv(data2, taps, data2Size, tapsSize);
  std::vector<float> conv4 = conv(data3, taps, data3Size, tapsSize);

  float conv1Size = static_cast<float>(conv1.size());
  float conv2Size = static_cast<float>(conv2.size());
  float conv3Size = static_cast<float>(conv3.size());
  float conv4Size = static_cast<float>(conv4.size());

  std::vector<float> sum(conv1Size);

  float i = 0;
  for (i = 0; i < conv4Size; i++)
  {
    sum[i] = conv1[i] + conv2[i] + conv3[i] + conv4[i];
  }

  for (i = i; i < conv3Size; i++)
    sum[i] = conv1[i] + conv2[i] + conv3[i];

  for (i = i; i < conv2Size; i++)
    sum[i] = conv1[i] + conv2[i];

  for (i = i; i < conv1Size; i++)
    sum[i] = conv1[i];

  return sum;
}

int main() {
  std::vector<float> A(200); // data sample
  float value = 1;           // fill data sample with this value
  std::vector<float> B;   // taps

  float nconv; // convoluted array length
  int i;
  float tmp;

  // fill data sample
  for (i = 0; i < A.size(); i++) {
    if (i % 2 == 0)
    {A[i] = 10;}
    else  A[i] = value;
  }
// fill taps
B.push_back(0);
B.push_back(-1/3);
B.push_back(-2/3);
B.push_back(-1);
B.push_back(-2/3);
B.push_back(-1/3);
B.push_back(0);
B.push_back(1/3);
B.push_back(2/3);
B.push_back(1);
B.push_back(2/3);
B.push_back(1/3);
B.push_back(0);

  std::vector<float> C = pyrConv(A, B);

  // prfloat convolution array
  for (i = 0; i < static_cast<float>(C.size()); i++) {
    printf("%d\n", C[i]);
  }

  // prfloat sum of nconv
  float sum = sum_array(C, static_cast<float>(C.size()));
  printf("Sum:\t%d\n", sum);

  // prfloat length
  printf("Size:\t%d\n", C.size());

  return 0;
}
