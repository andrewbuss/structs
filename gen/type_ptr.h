#pragma once

#include <functional>

struct Type;

struct type {
  int i = 0;
  const Type &operator*() const;
  operator bool() const { return i != 0; }
  type() : i(0) {}
  type(int i) : i(i) {}
  type operator=(const type &other) { return i = other.i; }
  bool operator==(const type &other) const { return i == other.i; }
  bool operator!=(const type &other) const { return i != other.i; }
  const Type *operator->() const;
};

namespace std {
template <> struct hash<type> {
  size_t operator()(const type &k) const { return k.i; }
};
template <> struct hash<const type> {
  size_t operator()(const type &k) const { return k.i; }
};
} // namespace std
