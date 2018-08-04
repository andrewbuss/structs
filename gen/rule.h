#pragma once

#include "hashutils.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <judgment.h>

#include <set>

using rule = int;

struct Rule {
  // {'conclusion': 'judgment', 'conditions': 'std::vector<judgment>', 'label':
  // 'token'}
  const judgment conclusion;
  const std::vector<judgment> conditions;
  const token label;
  // {'body': '{ return {}; }', 'type': 'const std::multiset<token>'}
  const std::multiset<token> signature() const {
    return {};
  } // {'body': '{ return Judgment::get(conclusion).type(); }', 'type': 'token'}
  token conclusion_type() const { return Judgment::get(conclusion).type(); }
  static const Rule &get(rule r) { return all_rules[r]; }

  Rule()
      : conclusion(judgment()), conditions(std::vector<judgment>()),
        label(token()) {}

  Rule(const judgment &conclusion, const std::vector<judgment> &conditions,
       const token &label)
      : conclusion(conclusion), conditions(conditions), label(label) {}

  static rule create(const judgment &conclusion,
                     const std::vector<judgment> &conditions,
                     const token &label) {
    return Rule(conclusion, conditions, label).save();
  }

  rule save() const {
    all_rules.push_back(Rule{conclusion, conditions, label});
    return index(all_rules.size() - 1);
  }

  static std::vector<Rule> all_rules;

  // {'getter': 'signature()', 'type': 'std::multiset<token>'}
  using lookup_by_signature_index_type =
      std::unordered_multimap<std::multiset<token>, rule>;
  static lookup_by_signature_index_type lookup_by_signature_index;
  using lookup_by_signature_index_iterator =
      lookup_by_signature_index_type::const_iterator;

  static std::pair<lookup_by_signature_index_iterator,
                   lookup_by_signature_index_iterator>
  lookup_by_signature(const std::multiset<token> &x) {
    return lookup_by_signature_index.equal_range(x);
  }

  // {'getter': 'conclusion', 'type': 'judgment'}
  using lookup_by_conclusion_index_type =
      std::unordered_multimap<judgment, rule>;
  static lookup_by_conclusion_index_type lookup_by_conclusion_index;
  using lookup_by_conclusion_index_iterator =
      lookup_by_conclusion_index_type::const_iterator;

  static std::pair<lookup_by_conclusion_index_iterator,
                   lookup_by_conclusion_index_iterator>
  lookup_by_conclusion(const judgment &x) {
    return lookup_by_conclusion_index.equal_range(x);
  }

  // {'getter': 'conclusion_type()', 'type': 'token'}
  using lookup_by_conclusion_type_index_type =
      std::unordered_multimap<token, rule>;
  static lookup_by_conclusion_type_index_type lookup_by_conclusion_type_index;
  using lookup_by_conclusion_type_index_iterator =
      lookup_by_conclusion_type_index_type::const_iterator;

  static std::pair<lookup_by_conclusion_type_index_iterator,
                   lookup_by_conclusion_type_index_iterator>
  lookup_by_conclusion_type(const token &x) {
    return lookup_by_conclusion_type_index.equal_range(x);
  }

  // {'getter': 'label', 'type': 'token'}
  using lookup_by_label_index_type = std::unordered_multimap<token, rule>;
  static lookup_by_label_index_type lookup_by_label_index;
  using lookup_by_label_index_iterator =
      lookup_by_label_index_type::const_iterator;

  static std::pair<lookup_by_label_index_iterator,
                   lookup_by_label_index_iterator>
  lookup_by_label(const token &x) {
    return lookup_by_label_index.equal_range(x);
  }

  // {'getter': 'conditions', 'type': 'std::vector<judgment>', 'iterable': True}
  using lookup_by_condition_index_type =
      std::unordered_multimap<std::vector<judgment>, rule>;
  static lookup_by_condition_index_type lookup_by_condition_index;
  using lookup_by_condition_index_iterator =
      lookup_by_condition_index_type::const_iterator;

  static std::pair<lookup_by_condition_index_iterator,
                   lookup_by_condition_index_iterator>
  lookup_by_condition(const std::vector<judgment> &x) {
    return lookup_by_condition_index.equal_range(x);
  }

  static rule index(const rule r) {
    const auto obj = all_rules[r];
    const auto &obj_signature = obj.signature();
    lookup_by_signature_index.emplace(obj_signature, r);
    const auto &obj_conclusion = obj.conclusion;
    lookup_by_conclusion_index.emplace(obj_conclusion, r);
    const auto &obj_conclusion_type = obj.conclusion_type();
    lookup_by_conclusion_type_index.emplace(obj_conclusion_type, r);
    const auto &obj_label = obj.label;
    lookup_by_label_index.emplace(obj_label, r);
    for (const auto &obj_conditions : obj.conditions)
      lookup_by_condition_index.emplace(obj_conditions, r);
    return r;
  }

  using application = int;
  application apply(rule, std::unordered_set<application> &);
};

std::ostream &operator<<(std::ostream &os, const Rule &r);
