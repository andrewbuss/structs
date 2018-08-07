#pragma once

#include <functional>

struct Rule;

struct rule {
  int i = 0;
  const Rule &operator*() const;
  operator bool() const { return i != 0; }
  rule() : i(0) {}
  rule(int i) : i(i) {}
  rule operator=(const rule &other) { return i = other.i; }
  bool operator==(const rule &other) const { return i == other.i; }
  bool operator!=(const rule &other) const { return i != other.i; }
  const Rule *operator->() const;
};

namespace std {
template <> struct hash<rule> {
  size_t operator()(const rule &k) const { return k.i; }
};
template <> struct hash<const rule> {
  size_t operator()(const rule &k) const { return k.i; }
};
} // namespace std
