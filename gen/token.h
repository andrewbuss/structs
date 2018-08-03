#pragma once

#include "hashutils.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using token = int;

struct Token {
  // {'s': 'std::string'}
  const std::string s;

  static const Token &get(token t) { return all_tokens[t]; }

  Token() : s(std::string()) {}

  Token(const std::string &s) : s(s) {}

  static token create(const std::string &s) { return Token(s).save(); }

  token save() const {
    all_tokens.push_back(Token{s});
    return all_tokens.size() - 1;
  }

  static std::vector<Token> all_tokens;

  static token get_or_create(const std::string &x) {
    auto t = lookup_by_s(x);
    if (!t)
      return create(x);
    return t;
  }

  // {'getter': 's', 'unique': True, 'type': 'std::string'}
  static std::unordered_map<std::string, token> lookup_by_s_index;
  static token lookup_by_s(const std::string &x) {
    auto it = lookup_by_s_index.find(x);
    if (it == lookup_by_s_index.end())
      return 0;
    return it->second;
  }

  static token index(const token t) {
    const auto obj = all_tokens[t];
    const auto &obj_s = obj.s;
    lookup_by_s_index[obj_s] = t;
    return t;
  }
};

std::ostream &operator<<(std::ostream &os, const Token &t);
