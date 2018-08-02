#pragma once

#include "hashutils.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <judgment.h>

using rule = int;

struct Rule {
  // {'conditions': 'std::unordered_set<judgment>', 'conclusion': 'judgment'}
  const std::unordered_set<judgment> conditions;
  const judgment conclusion;
  // {'body': {}, 'type': 'const std::unordered_map<token, int>'}
  const std::unordered_map<token, int> signature() const { return {}; }
  static const Rule &get(rule r) { return all_rules[r]; }

  Rule() : conditions(std::unordered_set<judgment>()), conclusion(judgment()) {}

  Rule(const std::unordered_set<judgment> &conditions,
       const judgment &conclusion)
      :

        conditions(conditions), conclusion(conclusion) {}

  static rule create(const std::unordered_set<judgment> &conditions,
                     const judgment &conclusion) {
    all_rules.push_back(Rule{conditions, conclusion});
    return index(all_rules.size() - 1);
  }

  static std::vector<Rule> all_rules;

  static rule index(const rule r) {
    const auto obj = all_rules[r];
    return r;
  }
};

std::ostream &operator<<(std::ostream &os, const Rule &r);
