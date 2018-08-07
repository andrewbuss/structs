#pragma once

#include <functional>

struct Token;

struct token {
  int i = 0;
  const Token &operator*() const;
  operator bool() const { return i != 0; }
  token() : i(0) {}
  token(int i) : i(i) {}
  token operator=(const token &other) { return i = other.i; }
  bool operator==(const token &other) const { return i == other.i; }
  bool operator!=(const token &other) const { return i != other.i; }
  const Token *operator->() const;
};

namespace std {
template <> struct hash<token> {
  size_t operator()(const token &k) const { return k.i; }
};
template <> struct hash<const token> {
  size_t operator()(const token &k) const { return k.i; }
};
} // namespace std
