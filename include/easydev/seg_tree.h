#ifndef SEGMENT_TREE_H__
#define SEGMENT_TREE_H__
#include <iostream>
#include <vector>
template <typename T>
class SegmentTree {
  std::vector<T> a;
  std::vector<T> c;
  std::vector<T> lazy;
  T size;
  void buildHelper(T rt, T l, T r);
  void buildHelper(T rt, T l, T r, const std::vector<T> &v);
  void updateHelper(T rt, T l, T r, T k);
  void pushdown(T rt, T l, T r);
  void modifyHelper(T rt, T l, T r, T s, T t, T k);
  T ask(T rt, T l, T r, T s, T t);

 public:
  T getSize(void) { return size; }
  SegmentTree(T n);
  SegmentTree(const std::vector<T> &data);
  void update(T l, T r, T val);
  T query(T l, T r);
};
template <typename T>
void SegmentTree<T>::buildHelper(T rt, T l, T r) {
  if (l == r) {
    a[rt] = 0;
    return;
  }
  T mid = (l + r) >> 1;
  buildHelper(rt << 1, l, mid);
  buildHelper(rt << 1 | 1, mid + 1, r);
  a[rt] = a[rt << 1] + a[rt << 1 | 1];
}
template <typename T>
void SegmentTree<T>::buildHelper(T rt, T l, T r, const std::vector<T> &v) {
  if (l == r) {
    a[rt] = v[l - 1];
    return;
  }
  T mid = (l + r) >> 1;
  buildHelper(rt << 1, l, mid, v);
  buildHelper(rt << 1 | 1, mid + 1, r, v);
  a[rt] = a[rt << 1] + a[rt << 1 | 1];
}
template <typename T>
SegmentTree<T>::SegmentTree(T n) {
  size = n;
  a.resize(4 * n);
  lazy.resize(4 * n);
  buildHelper(1, 1, n);
}
template <typename T>
SegmentTree<T>::SegmentTree(const std::vector<T> &v) {
  size = v.size();
  a.resize(4 * size);
  lazy.resize(4 * size);
  buildHelper(1, 1, size, v);
}
template <typename T>
void SegmentTree<T>::updateHelper(T rt, T l, T r, T k) {
  a[rt] += k * (r - l + 1);
  lazy[rt] += k;
  return;
}
template <typename T>
void SegmentTree<T>::pushdown(T rt, T l, T r) {
  T mid = (l + r) >> 1;
  updateHelper(rt << 1, l, mid, lazy[rt]);
  updateHelper(rt << 1 | 1, mid + 1, r, lazy[rt]);
  lazy[rt] = 0;
}
template <typename T>
void SegmentTree<T>::modifyHelper(T rt, T l, T r, T s, T t, T k) {
  if (l >= s && r <= t) {
    a[rt] += (r - l + 1) * k;
    lazy[rt] += k;
    return;
  }
  pushdown(rt, l, r);
  T mid = (l + r) >> 1;
  if (s <= mid) {
    modifyHelper(rt << 1, l, mid, s, t, k);
  }
  if (t > mid) {
    modifyHelper(rt << 1 | 1, mid + 1, r, s, t, k);
  }
  a[rt] = a[rt << 1] + a[rt << 1 | 1];
}
template <typename T>
T SegmentTree<T>::ask(T rt, T l, T r, T s, T t) {
  if (l >= s && r <= t) return a[rt];
  pushdown(rt, l, r);
  T mid = (l + r) >> 1;
  if (s > mid) {
    return ask(rt << 1 | 1, mid + 1, r, s, t);
  }
  if (t <= mid) {
    return ask(rt << 1, l, mid, s, t);
  }
  return ask(rt << 1, l, mid, s, t) + ask(rt << 1 | 1, mid + 1, r, s, t);
}
template <typename T>
void SegmentTree<T>::update(T l, T r, T v) {
  modifyHelper(1, 1, size, l + 1, r + 1, v);
}
template <typename T>
T SegmentTree<T>::query(T l, T r) {
  return ask(1, 1, size, l + 1, r + 1);
}
#endif