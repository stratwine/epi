#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <limits.h>

using namespace std;

/*
template <typename T>
T find_kth_in_two_sorted_arrays(
  const vector<T> &A, const vector<T> &B, int k) {
  // Not enough elements in A and B
  if (k > A.size() + B.size()) {
    return -1;  // no k-th element
  }

  int l_A = 0, r_A = min(static_cast<int>(A.size() - 1), k - 1);
  int l_B = 0, r_B = min(static_cast<int>(B.size() - 1), k - 1);
  while (true) {
    // Averagely get k - 1 elements from A and B
    int num_A = r_A - l_A + 1, num_B = r_B - l_B + 1;
    int i = num_A * (k - 1) / (num_A + num_B);
    int j = (k - 1) - i;

    T A_i_1 = (l_A + i == 0 ? numeric_limits<int>::min() : A[l_A + i - 1]);
    T A_i = (l_A + i == A.size() ? numeric_limits<int>::max() : A[l_A + i]);
    T B_j_1 = (l_B + j == 0 ? numeric_limits<int>::min() : B[l_B + j - 1]);
    T B_j = (l_B + j == B.size() ? numeric_limits<int>::max() : B[l_B + j]);

    if (B_j_1 <= A_i && A_i <= B_j) {
      return A_i;  // A_i is the k-th element
    } else if (A_i_1 <= B_j && B_j <= A_i) {
      return B_j;  // B_j is the k-th element
    }

    if (A_i < B_j) {
      // Exclude the elements <= A_i and >= B_j
      l_A += i + 1, k -= (i + 1);
    } else {
      // Exclude the elements <= B_j and >= A_i
      l_B += j + 1, k -= (j + 1);
    }
  }
}
*/
// @include
template <typename T>
T find_kth_in_two_sorted_arrays(const vector<T> &A, const vector<T> &B, 
                                const int &k) {
  // Lower bound of elements we will choose in A
  int l = max(0, static_cast<int>(k - B.size()));
  // Upper bound of elements we will choose in A
  int u = min(static_cast<int>(A.size()), k);

  while (l < u) {
    int x = l + ((u - l) >> 1);
    T A_x_1 = (x <= 0 ? numeric_limits<T>::min() : A[x - 1]);
    T A_x = (x >= A.size() ? numeric_limits<T>::max() : A[x]);
    T B_k_x_1 = (k - x <= 0 ? numeric_limits<T>::min() : B[k - x - 1]);
    T B_k_x = (k - x >= B.size() ? numeric_limits<T>::max() : B[k - x]);

    if (A_x < B_k_x_1) {
      l = x + 1;
    } else if (A_x_1 > B_k_x) {
      u = x - 1;
    } else {
      // B[k - x - 1] <= A[x] && A[x - 1] < B[k - x]
      return max(A_x_1, B_k_x_1);
    }
  }

  T A_l_1 = l <= 0 ? numeric_limits<T>::min() : A[l - 1];
  T B_k_l_1 = k - l - 1 < 0 ? numeric_limits<T>::min() : B[k - l - 1];
  return max(A_l_1, B_k_l_1);
}
// @exclude

template <typename T>
T check_answer(const vector<T> &A, const vector<T> &B, const int &k) {
  int i = 0, j = 0, count = 0;
  T ret;
  while ((i < A.size() || j < B.size()) && count < k) {
    if (i < A.size() && j < B.size()) {
      if (A[i] < B[j]) {
        ret = A[i];
        ++i;
      } else {
        ret = B[j];
        ++j;
      }
    } else if (i < A.size()) {
      ret = A[i];
      ++i;
    } else {
      ret = B[j];
      ++j;
    }
    ++count;
  }
  return ret;
}

int main(int argc, char *argv[]) {
  // AA: handwritten test
  vector<int> A0;
  vector<int> B0;
  A0.emplace_back(0);
  A0.emplace_back(1);
  A0.emplace_back(2);
  A0.emplace_back(3);
  B0.emplace_back(0);
  B0.emplace_back(1);
  B0.emplace_back(2);
  B0.emplace_back(3);
  cout << "AA hand test output " << find_kth_in_two_sorted_arrays(A0, B0, 1) << endl;
  assert( 0 == find_kth_in_two_sorted_arrays(A0, B0, 1));

  srand(time(nullptr));
  // Random test 10000 times
  for (int times = 0; times < 10000; ++times) {
    vector<int> A, B;
    int n, m, k;
    if (argc == 3) {
      n = atoi(argv[1]), m = atoi(argv[2]);
      k = 1 + rand() % (n + m);
    } else if (argc == 4) {
      n = atoi(argv[1]), m = atoi(argv[2]), k = atoi(argv[3]);
    } else {
      n = 1 + rand() % 10000, m = 1 + rand() % 10000;
      k = 1 + rand() % (n + m);
    }
    for (size_t i = 0; i < n; ++i) {
      A.emplace_back(rand() % 100000);
    }
    for (size_t i = 0; i < m; ++i) {
      B.emplace_back(rand() % 100000);
    }
    sort(A.begin(), A.end()), sort(B.begin(), B.end());
    /*
    for (int i = 0; i < A.size(); ++i) {
      cout << A[i] << ' ';
    }
    cout << endl;
    for (int j = 0; j < B.size(); ++j) {
      cout << B[j] << ' ';
    }
    cout << endl;
    */
    int ans = find_kth_in_two_sorted_arrays(A, B, k);
    cout << k << "th = " << ans << endl;
    assert(ans == check_answer(A, B, k));
    A.clear(), B.clear();
  }
  return 0;
}
