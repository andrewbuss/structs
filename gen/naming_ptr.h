#pragma once

#include <unordered_map>

struct Naming;

struct naming {
  int i;
  const Naming &operator*() const;
  operator bool() const { return i != 0; }
  naming() : i(0) {}
  naming(int i) : i(i) {}
  naming operator=(const naming &other) { return i = other.i; }
  bool operator==(const naming &other) const { return i == other.i; }
  bool operator!=(const naming &other) const { return i != other.i; }
  const Naming *operator->() const;
};

namespace std {
template <> struct hash<naming> {
  size_t operator()(const naming &k) const { return k.i; }
};
template <> struct hash<const naming> {
  size_t operator()(const naming &k) const { return k.i; }
};
} // namespace std
