#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

struct Judgment;

struct judgment {
  int i;
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

struct Judgment {
  // {'cs': 'std::vector<token>', 's': 'shape', 'n': 'naming'}
  const std::vector<token> cs;
  const shape s;
  const naming n;
  // {'body': '{ if(cs.size() > 0) return cs[0]; else return token(); }',
  // 'type': 'token'}
  token type() const;

  Judgment() : cs(std::vector<token>()), s(shape()), n(naming()) {}

  Judgment(const std::vector<token> &cs, const shape &s, const naming &n)
      : cs(cs), s(s), n(n) {}

  static judgment create(const std::vector<token> &cs, const shape &s,
                         const naming &n);

  static judgment get_or_create(const std::vector<token> &x);
  judgment save() const { return get_or_create(cs); }

  judgment get_or_create(std::vector<std::string> ss) {
    std::vector<token> ts;
    for (const auto &s : ss) {
      ts.push_back(Token::get_or_create(s));
    }
    return get_or_create(ts);
  }
};

struct JudgmentIndex {

  // {'getter': 'n', 'type': 'naming'}
  using lookup_by_naming_index_type = std::unordered_multimap<naming, judgment>;
  static lookup_by_naming_index_type lookup_by_naming_index;
  using lookup_by_naming_index_iterator =
      lookup_by_naming_index_type::const_iterator;
  static std::pair<lookup_by_naming_index_iterator,
                   lookup_by_naming_index_iterator>
  lookup_by_naming(const naming &x);

  // {'getter': 's', 'type': 'shape'}
  using lookup_by_shape_index_type = std::unordered_multimap<shape, judgment>;
  static lookup_by_shape_index_type lookup_by_shape_index;
  using lookup_by_shape_index_iterator =
      lookup_by_shape_index_type::const_iterator;
  static std::pair<lookup_by_shape_index_iterator,
                   lookup_by_shape_index_iterator>
  lookup_by_shape(const shape &x);

  // {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
  static std::unordered_map<std::vector<token>, judgment> lookup_by_cs_index;
  static judgment lookup_by_cs(const std::vector<token> &x);

  // {'getter': 'type()', 'type': 'token'}
  using lookup_by_type_index_type = std::unordered_multimap<token, judgment>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;
  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const token &x);

  // {'getter': 'cs', 'type': 'token', 'iterable': True}
  using lookup_by_token_index_type = std::unordered_multimap<token, judgment>;
  static lookup_by_token_index_type lookup_by_token_index;
  using lookup_by_token_index_iterator =
      lookup_by_token_index_type::const_iterator;
  static std::pair<lookup_by_token_index_iterator,
                   lookup_by_token_index_iterator>
  lookup_by_token(const token &x);

  static judgment index(judgment);
  static std::vector<Judgment> all_Judgments;
};

std::ostream &operator<<(std::ostream &os, const Judgment &);
std::ostream &operator<<(std::ostream &os, const judgment &);
