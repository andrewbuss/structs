

#pragma once

#include "application_ptr.h"
#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rule.h>

#include <judgment.h>

#include <token.h>

#include <list>

#include <vector>

#include <unordered_map>

#include <unordered_set>

using Stack = std::unordered_map<type, std::vector<application>>;

struct Application {
  const rule via;
  const Stack conditions;
  const judgment result;
  const Stack args;
  // {'body': '{ if(via) return 0; else return result; }', 'type': 'judgment'}
  judgment hypothesis_or_empty()
      const; // {'body': '{\n  std::unordered_set<judgment> rv;\n  for(const
             // auto& [typ, conditions_of_type]: conditions) {\n    for(const
             // auto& c: conditions_of_type) {\n      if(!c->via &&
             // c->conditions.size() == 0) continue;\n      const auto&
             // c_assumptions = c->assumptions();\n
             // rv.insert(c_assumptions.begin(), c_assumptions.end());\n    }\n
             // }\n  return rv;\n}', 'type': 'std::unordered_set<judgment>'}
  std::unordered_set<judgment> assumptions() const;

  Application()
      : via(rule()), conditions(Stack()), result(judgment()), args(Stack()) {}

  Application(const rule &via, const Stack &conditions, const judgment &result,
              const Stack &args)
      : via(via), conditions(conditions), result(result), args(args) {}

  static application create(const rule &via, const Stack &conditions,
                            const judgment &result, const Stack &args);

  static application get_or_create(const judgment &x);
  static application get_if_exists(const judgment &x);
  application save() const { return get_or_create(hypothesis_or_empty()); }

  Application(judgment j) : via(0), result(j) {}

  inline static application get_or_create_hypothesis(judgment j) {
    return get_or_create(j);
  }
  static application create(judgment j) { return create(rule(), {}, j, {}); }
};

struct ApplicationIndex {

  // {'getter': 'via', 'type': 'rule'}
  using lookup_by_rule_index_type = std::unordered_multimap<rule, application>;
  static lookup_by_rule_index_type lookup_by_rule_index;
  using lookup_by_rule_index_iterator =
      lookup_by_rule_index_type::const_iterator;
  static std::pair<lookup_by_rule_index_iterator, lookup_by_rule_index_iterator>
  lookup_by_rule(const rule &x);

  // {'getter': 'hypothesis_or_empty()', 'unique': True, 'type': 'judgment'}
  static std::unordered_map<judgment, application> lookup_by_hypothesis_index;
  static application lookup_by_hypothesis(const judgment &x);

  // {'getter': 'result', 'type': 'judgment'}
  using lookup_by_result_index_type =
      std::unordered_multimap<judgment, application>;
  static lookup_by_result_index_type lookup_by_result_index;
  using lookup_by_result_index_iterator =
      lookup_by_result_index_type::const_iterator;
  static std::pair<lookup_by_result_index_iterator,
                   lookup_by_result_index_iterator>
  lookup_by_result(const judgment &x);

  static application index(application);
  static std::vector<Application> all_Applications;
};

std::ostream &operator<<(std::ostream &os, const Application &);
std::ostream &operator<<(std::ostream &os, const application &);
