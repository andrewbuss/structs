#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <judgment.h>

#include <set>

struct Rule;

struct rule {
  int i;
  const Rule &operator*() const;
  operator bool() const { return i != 0; }
  rule() : i(0) {}
  rule(int i) : i(i) {}
  rule operator=(const rule &other) { return i = other.i; }
  bool operator==(const rule &other) const { return i == other.i; }
  bool operator!=(const rule &other) const { return i != other.i; }
  const Rule *operator->() const;
};

namespace std {
template <> struct hash<rule> {
  size_t operator()(const rule k) const { return hash<int>{}(k.i); }
};
} // namespace std

struct Rule {
  // {'conclusion': 'judgment', 'conditions': 'std::vector<judgment>', 'label':
  // 'token'}
  const judgment conclusion;
  const std::vector<judgment> conditions;
  const token label;
  // {'body': '{ return {}; }', 'type': 'const std::multiset<token>'}
  const std::multiset<token> signature()
      const; // {'body': '{ return conclusion->type(); }', 'type': 'token'}
  token conclusion_type() const;

  Rule()
      : conclusion(judgment()), conditions(std::vector<judgment>()),
        label(token()) {}

  Rule(const judgment &conclusion, const std::vector<judgment> &conditions,
       const token &label)
      : conclusion(conclusion), conditions(conditions), label(label) {}
  static rule create(const judgment &conclusion,
                     const std::vector<judgment> &conditions,
                     const token &label);

  using application = int;
  application apply(rule, std::unordered_set<application> &);
};

struct RuleIndex {

  // {'getter': 'signature()', 'type': 'std::multiset<token>'}
  using lookup_by_signature_index_type =
      std::unordered_multimap<std::multiset<token>, rule>;
  static lookup_by_signature_index_type lookup_by_signature_index;
  using lookup_by_signature_index_iterator =
      lookup_by_signature_index_type::const_iterator;
  static std::pair<lookup_by_signature_index_iterator,
                   lookup_by_signature_index_iterator>
  lookup_by_signature(const std::multiset<token> &x);

  // {'getter': 'conclusion', 'type': 'judgment'}
  using lookup_by_conclusion_index_type =
      std::unordered_multimap<judgment, rule>;
  static lookup_by_conclusion_index_type lookup_by_conclusion_index;
  using lookup_by_conclusion_index_iterator =
      lookup_by_conclusion_index_type::const_iterator;
  static std::pair<lookup_by_conclusion_index_iterator,
                   lookup_by_conclusion_index_iterator>
  lookup_by_conclusion(const judgment &x);

  // {'getter': 'conclusion_type()', 'type': 'token'}
  using lookup_by_conclusion_type_index_type =
      std::unordered_multimap<token, rule>;
  static lookup_by_conclusion_type_index_type lookup_by_conclusion_type_index;
  using lookup_by_conclusion_type_index_iterator =
      lookup_by_conclusion_type_index_type::const_iterator;
  static std::pair<lookup_by_conclusion_type_index_iterator,
                   lookup_by_conclusion_type_index_iterator>
  lookup_by_conclusion_type(const token &x);

  // {'getter': 'label', 'type': 'token'}
  using lookup_by_label_index_type = std::unordered_multimap<token, rule>;
  static lookup_by_label_index_type lookup_by_label_index;
  using lookup_by_label_index_iterator =
      lookup_by_label_index_type::const_iterator;
  static std::pair<lookup_by_label_index_iterator,
                   lookup_by_label_index_iterator>
  lookup_by_label(const token &x);

  // {'getter': 'conditions', 'type': 'std::vector<judgment>', 'iterable': True}
  using lookup_by_condition_index_type =
      std::unordered_multimap<std::vector<judgment>, rule>;
  static lookup_by_condition_index_type lookup_by_condition_index;
  using lookup_by_condition_index_iterator =
      lookup_by_condition_index_type::const_iterator;
  static std::pair<lookup_by_condition_index_iterator,
                   lookup_by_condition_index_iterator>
  lookup_by_condition(const std::vector<judgment> &x);

  static rule index(rule);
};

std::ostream &operator<<(std::ostream &os, const Rule &);
std::ostream &operator<<(std::ostream &os, const rule &);
