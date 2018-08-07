#pragma once

#include <unordered_map>

struct Constant;

struct constant {
  int i;
  const Constant &operator*() const;
  operator bool() const { return i != 0; }
  constant() : i(0) {}
  constant(int i) : i(i) {}
  constant operator=(const constant &other) { return i = other.i; }
  bool operator==(const constant &other) const { return i == other.i; }
  bool operator!=(const constant &other) const { return i != other.i; }
  const Constant *operator->() const;
};

namespace std {
template <> struct hash<constant> {
  size_t operator()(const constant &k) const { return k.i; }
};
template <> struct hash<const constant> {
  size_t operator()(const constant &k) const { return k.i; }
};
} // namespace std
