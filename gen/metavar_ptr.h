#pragma once

#include <functional>

struct Metavar;

struct metavar {
  int i = 0;
  const Metavar &operator*() const;
  operator bool() const { return i != 0; }
  metavar() : i(0) {}
  metavar(int i) : i(i) {}
  metavar operator=(const metavar &other) { return i = other.i; }
  bool operator==(const metavar &other) const { return i == other.i; }
  bool operator!=(const metavar &other) const { return i != other.i; }
  const Metavar *operator->() const;
};

namespace std {
template <> struct hash<metavar> {
  size_t operator()(const metavar &k) const { return k.i; }
};
template <> struct hash<const metavar> {
  size_t operator()(const metavar &k) const { return k.i; }
};
} // namespace std
