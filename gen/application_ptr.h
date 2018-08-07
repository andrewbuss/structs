#pragma once

#include <functional>

struct Application;

struct application {
  int i = 0;
  const Application &operator*() const;
  operator bool() const { return i != 0; }
  application() : i(0) {}
  application(int i) : i(i) {}
  application operator=(const application &other) { return i = other.i; }
  bool operator==(const application &other) const { return i == other.i; }
  bool operator!=(const application &other) const { return i != other.i; }
  const Application *operator->() const;
};

namespace std {
template <> struct hash<application> {
  size_t operator()(const application &k) const { return k.i; }
};
template <> struct hash<const application> {
  size_t operator()(const application &k) const { return k.i; }
};
} // namespace std
