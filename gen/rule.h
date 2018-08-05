#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <judgment.h>

#include <token.h>

#include <set>

#include <unordered_map>

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
  size_t operator()(const rule &k) const { return k.i; }
};
template <> struct hash<const rule> {
  size_t operator()(const rule &k) const { return k.i; }
};
} // namespace std

struct application;

struct Rule {
  // {'arity': 'std::unordered_map<token, int>', 'conclusion': 'judgment',
  // 'conditions': 'std::unordered_map<token, std::vector<judgment>>', 'label':
  // 'token'}
  const std::unordered_map<token, int> arity;
  const judgment conclusion;
  const std::unordered_map<token, std::vector<judgment>> conditions;
  const token label;
  // {'body': '{ return conclusion->type(); }', 'type': 'token'}
  token conclusion_type() const;

  Rule()
      : arity(std::unordered_map<token, int>()), conclusion(judgment()),
        conditions(std::unordered_map<token, std::vector<judgment>>()),
        label(token()) {}

  Rule(const std::unordered_map<token, int> &arity, const judgment &conclusion,
       const std::unordered_map<token, std::vector<judgment>> &conditions,
       const token &label)
      : arity(arity), conclusion(conclusion), conditions(conditions),
        label(label) {}

  static rule
  create(const std::unordered_map<token, int> &arity,
         const judgment &conclusion,
         const std::unordered_map<token, std::vector<judgment>> &conditions,
         const token &label);

  rule save() const { return create(arity, conclusion, conditions, label); }
};

struct RuleIndex {

  // {'getter': 'arity', 'type': 'std::unordered_map<token, int>'}
  using lookup_by_signature_index_type =
      std::unordered_multimap<std::unordered_map<token, int>, rule>;
  static lookup_by_signature_index_type lookup_by_signature_index;
  using lookup_by_signature_index_iterator =
      lookup_by_signature_index_type::const_iterator;
  static std::pair<lookup_by_signature_index_iterator,
                   lookup_by_signature_index_iterator>
  lookup_by_signature(const std::unordered_map<token, int> &x);

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

  // {'getter': 'label', 'unique': True, 'type': 'token'}
  static std::unordered_map<token, rule> lookup_by_label_index;
  static rule lookup_by_label(const token &x);

  static rule index(rule);
};

std::ostream &operator<<(std::ostream &os, const Rule &);
std::ostream &operator<<(std::ostream &os, const rule &);
