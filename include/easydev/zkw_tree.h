#ifndef ZKWTree_H__
#define ZKWTree_H__
#include <vector>
template <typename T>
class ZKWTree {
 private:
  std::vector<T> sum;
  std::vector<T> tag;
  T M;
  T n;

 public:
  ZKWTree(const std::vector<T>& array) {
    new (this) ZKWTree(array.size());
    for (T i = 1; i < n; i++) {
      sum[M + i] = array[i];
    }
    for (T i = M - 1; i; i--) {
      sum[i] = sum[i << 1] + sum[i << 1 | 1];
    }
  }
  ZKWTree(const T& _n) {
    n = _n;
    M = 1;
    while (M < n + 2) {
      M <<= 1;
    }
    sum.resize(M << 1, 0);
    tag.resize(M << 1, 0);
  }
  T getSize(void) { return n; }
  void update(T l, T r, T del) {
    T len = 1, lc = 0, rc = 0;
    for (l = l + M - 1, r = r + M + 1; l ^ r ^ 1; l >>= 1, r >>= 1, len <<= 1) {
      if (~l & 1) {
        tag[l + 1] += del, lc += len;
      }
      if (r & 1) {
        tag[r - 1] += del, rc += len;
      }
      sum[l >> 1] += del * lc;
      sum[r >> 1] += del * rc;
    }
    for (lc += rc, l >>= 1; l; l >>= 1) {
      sum[l] += del * lc;
    }
  }
  T query(T l, T r) {
    T res = 0, len = 1, lc = 0, rc = 0;
    for (l = l + M - 1, r = r + M + 1; l ^ r ^ 1; l >>= 1, r >>= 1, len <<= 1) {
      if (~l & 1) {
        res += sum[l + 1] + tag[l + 1] * len, lc += len;
      }
      if (r & 1) {
        res += sum[r - 1] + tag[r - 1] * len, rc += len;
      }
      res += tag[l >> 1] * lc;
      res += tag[r >> 1] * rc;
    }
    for (lc += rc, l >>= 1; l; l >>= 1) {
      res += tag[l] * lc;
    }
    return res;
  }
};
#endif