#pragma once

#include <functional>

struct Judgment;

struct judgment {
  int i = 0;
  const Judgment &operator*() const;
  operator bool() const { return i != 0; }
  judgment() : i(0) {}
  judgment(int i) : i(i) {}
  judgment operator=(const judgment &other) { return i = other.i; }
  bool operator==(const judgment &other) const { return i == other.i; }
  bool operator!=(const judgment &other) const { return i != other.i; }
  const Judgment *operator->() const;
};

namespace std {
template <> struct hash<judgment> {
  size_t operator()(const judgment &k) const { return k.i; }
};
template <> struct hash<const judgment> {
  size_t operator()(const judgment &k) const { return k.i; }
};
} // namespace std
