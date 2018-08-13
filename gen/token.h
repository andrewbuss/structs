

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "token_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <vector>

#include <metavar_ptr.h>

#include <type.h>

struct Token {
  const std::string s;
  mutable metavar mv;
  mutable type typ;
  mutable type var_typ;
  // {'body': 'if(!typ)\n  typ = Type::get_or_create(this -
  // TokenIndex::all_Tokens.data());\nreturn typ;', 'type': 'type'}
  type as_type() const;

  Token() : s(std::string()) {}

  Token(const std::string &s) : s(s) {}

  static token create(const std::string &s);

  static token get_or_create(const std::string &x);
  static token get_if_exists(const std::string &x);
  token save() const { return get_or_create(s); }
};

struct TokenIndex {

  // {'getter': 's', 'unique': True, 'type': 'std::string'}
  static std::unordered_map<std::string, token> lookup_by_s_index;
  static token lookup_by_s(const std::string &x);

  static token index(token);
  static std::vector<Token> all_Tokens;
};

std::ostream &operator<<(std::ostream &os, const Token &);
std::ostream &operator<<(std::ostream &os, const token &);
