#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

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

struct Naming {
  // {'cs': 'std::vector<token>', 's': 'shape', 'n': 'naming'}
  const std::vector<token> cs;
  const shape s;
  const naming n;
  // {'body': '{ if(cs.size() > 0) return cs[0]; else return token(); }',
  // 'type': 'token'}
  token type() const;

  Naming() : cs(std::vector<token>()), s(shape()), n(naming()) {}

  Naming(const std::vector<token> &cs, const shape &s, const naming &n)
      : cs(cs), s(s), n(n) {}

  static naming create(const std::vector<token> &cs, const shape &s,
                       const naming &n);

  static naming get_or_create(const std::vector<token> &x);
  naming save() const { return get_or_create(cs); }

  judgment get_or_create(std::vector<std::string> ss) {
    std::vector<token> ts;
    for (const auto &s : ss) {
      ts.push_back(Token::get_or_create(s));
    }
    return get_or_create(ts);
  }
};

struct NamingIndex {

  // {'getter': 'n', 'type': 'naming'}
  using lookup_by_naming_index_type = std::unordered_multimap<naming, naming>;
  static lookup_by_naming_index_type lookup_by_naming_index;
  using lookup_by_naming_index_iterator =
      lookup_by_naming_index_type::const_iterator;
  static std::pair<lookup_by_naming_index_iterator,
                   lookup_by_naming_index_iterator>
  lookup_by_naming(const naming &x);

  // {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
  static std::unordered_map<std::vector<token>, naming> lookup_by_cs_index;
  static naming lookup_by_cs(const std::vector<token> &x);

  // {'getter': 'type()', 'type': 'token'}
  using lookup_by_type_index_type = std::unordered_multimap<token, naming>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;
  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const token &x);

  // {'getter': 's', 'type': 'shape'}
  using lookup_by_shape_index_type = std::unordered_multimap<shape, naming>;
  static lookup_by_shape_index_type lookup_by_shape_index;
  using lookup_by_shape_index_iterator =
      lookup_by_shape_index_type::const_iterator;
  static std::pair<lookup_by_shape_index_iterator,
                   lookup_by_shape_index_iterator>
  lookup_by_shape(const shape &x);

  // {'getter': 'cs', 'type': 'token', 'iterable': True}
  using lookup_by_token_index_type = std::unordered_multimap<token, naming>;
  static lookup_by_token_index_type lookup_by_token_index;
  using lookup_by_token_index_iterator =
      lookup_by_token_index_type::const_iterator;
  static std::pair<lookup_by_token_index_iterator,
                   lookup_by_token_index_iterator>
  lookup_by_token(const token &x);

  static naming index(naming);
  static std::vector<Naming> all_Namings;
};

std::ostream &operator<<(std::ostream &os, const Naming &);
std::ostream &operator<<(std::ostream &os, const naming &);
