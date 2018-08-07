#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

struct Metavar;

struct metavar {
  int i;
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

struct Metavar {
  // {'tok': 'token', 'type': 'token'}
  const token tok;
  const token type;

  Metavar() : tok(token()), type(token()) {}

  Metavar(const token &tok, const token &type) : tok(tok), type(type) {}

  static metavar create(const token &tok, const token &type);

  static metavar get_or_create(const token &x);
  metavar save() const { return get_or_create(tok); }

  static token type_of_metavar_token(token t) {
    auto pos = t->s.find('$');
    return Token::get_or_create(t->s.substr(0, pos));
  }
  Metavar(token t) : type(Metavar::type_of_metavar_token(t)), tok(t) {}

  static metavar create(token t) {
    return create(t, Metavar::type_of_metavar_token(t));
  }
};

struct MetavarIndex {

  // {'getter': 'tok', 'unique': True, 'type': 'token'}
  static std::unordered_map<token, metavar> lookup_by_tok_index;
  static metavar lookup_by_tok(const token &x);

  // {'getter': 'type', 'type': 'token'}
  using lookup_by_type_index_type = std::unordered_multimap<token, metavar>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;
  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const token &x);

  static metavar index(metavar);
  static std::vector<Metavar> all_Metavars;
};

std::ostream &operator<<(std::ostream &os, const Metavar &);
std::ostream &operator<<(std::ostream &os, const metavar &);
