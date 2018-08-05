#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Token;

struct token {
  int i;
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

struct Token {
  // {'s': 'std::string'}
  const std::string s;

  Token() : s(std::string()) {}

  Token(const std::string &s) : s(s) {}

  static token create(const std::string &s);

  static token get_or_create(const std::string &x);
  token save() const { return get_or_create(s); }
};

struct TokenIndex {

  // {'getter': 's', 'unique': True, 'type': 'std::string'}
  static std::unordered_map<std::string, token> lookup_by_s_index;
  static token lookup_by_s(const std::string &x);

  static token index(token);
};

std::ostream &operator<<(std::ostream &os, const Token &);
std::ostream &operator<<(std::ostream &os, const token &);
