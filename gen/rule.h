

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "rule_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <judgment.h>

#include <token.h>

#include <set>

#include <unordered_map>

using arity_t = std::unordered_map<token, int>;

struct application;

struct Rule {
  const arity_t arity;
  const judgment conclusion;
  const std::unordered_map<token, std::vector<judgment>> conditions;
  const token label;

  Rule()
      : arity(arity_t()), conclusion(judgment()),
        conditions(std::unordered_map<token, std::vector<judgment>>()),
        label(token()) {}

  Rule(const arity_t &arity, const judgment &conclusion,
       const std::unordered_map<token, std::vector<judgment>> &conditions,
       const token &label)
      : arity(arity), conclusion(conclusion), conditions(conditions),
        label(label) {}

  static rule
  create(const arity_t &arity, const judgment &conclusion,
         const std::unordered_map<token, std::vector<judgment>> &conditions,
         const token &label);

  rule save() const { return create(arity, conclusion, conditions, label); }
};

struct RuleIndex {

  // {'getter': 'arity', 'type': 'arity_t'}
  using lookup_by_signature_index_type = std::unordered_multimap<arity_t, rule>;
  static lookup_by_signature_index_type lookup_by_signature_index;
  using lookup_by_signature_index_iterator =
      lookup_by_signature_index_type::const_iterator;
  static std::pair<lookup_by_signature_index_iterator,
                   lookup_by_signature_index_iterator>
  lookup_by_signature(const arity_t &x);

  // {'getter': 'conclusion', 'type': 'judgment'}
  using lookup_by_conclusion_index_type =
      std::unordered_multimap<judgment, rule>;
  static lookup_by_conclusion_index_type lookup_by_conclusion_index;
  using lookup_by_conclusion_index_iterator =
      lookup_by_conclusion_index_type::const_iterator;
  static std::pair<lookup_by_conclusion_index_iterator,
                   lookup_by_conclusion_index_iterator>
  lookup_by_conclusion(const judgment &x);

  // {'getter': 'conclusion->shp->typ()', 'type': 'token'}
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
  static std::vector<Rule> all_Rules;
};

std::ostream &operator<<(std::ostream &os, const Rule &);
std::ostream &operator<<(std::ostream &os, const rule &);
