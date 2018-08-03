#pragma once

#include "hashutils.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

using judgment = int;

struct Judgment {
  // {'cs': 'std::vector<token>'}
  const std::vector<token> cs;
  // {'body': '{ return cs[0]; }', 'type': 'token'}
  token type() const { return cs[0]; }
  static const Judgment &get(judgment j) { return all_judgments[j]; }

  Judgment() : cs(std::vector<token>()) {}

  Judgment(const std::vector<token> &cs) : cs(cs) {}

  static judgment create(const std::vector<token> &cs) {
    return Judgment(cs).save();
  }

  judgment save() const {
    all_judgments.push_back(Judgment{cs});
    return all_judgments.size() - 1;
  }

  static std::vector<Judgment> all_judgments;

  static judgment get_or_create(const std::vector<token> &x) {
    auto j = lookup_by_cs(x);
    if (!j)
      return create(x);
    return j;
  }

  // {'getter': 'type()', 'type': 'token'}
  using lookup_by_type_index_type = std::unordered_multimap<token, judgment>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;

  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const token &x) {
    return lookup_by_type_index.equal_range(x);
  }

  // {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
  static std::unordered_map<std::vector<token>, judgment> lookup_by_cs_index;
  static judgment lookup_by_cs(const std::vector<token> &x) {
    auto it = lookup_by_cs_index.find(x);
    if (it == lookup_by_cs_index.end())
      return 0;
    return it->second;
  }

  // {'getter': 'cs', 'type': 'token', 'iterable': True}
  using lookup_by_token_index_type = std::unordered_multimap<token, judgment>;
  static lookup_by_token_index_type lookup_by_token_index;
  using lookup_by_token_index_iterator =
      lookup_by_token_index_type::const_iterator;

  static std::pair<lookup_by_token_index_iterator,
                   lookup_by_token_index_iterator>
  lookup_by_token(const token &x) {
    return lookup_by_token_index.equal_range(x);
  }

  static judgment index(const judgment j) {
    const auto obj = all_judgments[j];
    const auto &obj_type = obj.type();
    lookup_by_type_index.emplace(obj_type, j);
    const auto &obj_cs = obj.cs;
    lookup_by_cs_index[obj_cs] = j;
    for (const auto &obj_cs : obj.cs)
      lookup_by_token_index.emplace(obj_cs, j);
    return j;
  }
};

std::ostream &operator<<(std::ostream &os, const Judgment &j);
