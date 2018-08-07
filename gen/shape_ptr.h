#pragma once

#include <functional>

struct Shape;

struct shape {
  int i = 0;
  const Shape &operator*() const;
  operator bool() const { return i != 0; }
  shape() : i(0) {}
  shape(int i) : i(i) {}
  shape operator=(const shape &other) { return i = other.i; }
  bool operator==(const shape &other) const { return i == other.i; }
  bool operator!=(const shape &other) const { return i != other.i; }
  const Shape *operator->() const;
};

namespace std {
template <> struct hash<shape> {
  size_t operator()(const shape &k) const { return k.i; }
};
template <> struct hash<const shape> {
  size_t operator()(const shape &k) const { return k.i; }
};
} // namespace std
