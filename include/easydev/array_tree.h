#ifndef ARRAY_TREE_ARRAY_TREE_H_
#define ARRAY_TREE_ARRAY_TREE_H_
#include <vector>
template <typename T>  // T can be int, long and so on
class ArrayTree {
  T n;
  std::vector<T> df;
  std::vector<T> mf;
  T lowbit(T x) { return (-x) & x; }
  T ask(T x) {
    T res = 0;
    for (T i = x + 1; i > 0; i -= lowbit(i)) {
      res += df[i] * (x + 2) - mf[i];
    }
    return res;
  }
  void add(T x, T v) {
    for (T i = x + 1; i <= n; i += lowbit(i)) {
      df[i] += v;
      mf[i] += v * (x + 1);
    }
  }

 public:
  ArrayTree(T _n) : n(_n + 1), df(n, 0), mf(n, 0) {}
  ArrayTree(const std::vector<T>& v) {
    new (this) ArrayTree(v.size());
    if (v.size() > 0) {
      add(0, v[0]);
      for (T i = 1; i < n - 1; i++) {
        add(i, v[i] - v[i - 1]);
      }
    }
  }
  T getSize(void) { return n - 1; }
  void update(T l, T r, T v) {
    add(l, v);
    add(r + 1, -v);
  }
  void update(T x, T v) { update(x, x, v); }
  T query(T l, T r) { return ask(r) - ask(l - 1); }
  T query(T x) { return query(x, x); }
};
#endif