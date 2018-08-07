#pragma once

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

struct Application;

struct application {
  int i;
  const Application &operator*() const;
  operator bool() const { return i != 0; }
  application() : i(0) {}
  application(int i) : i(i) {}
  application operator=(const application &other) { return i = other.i; }
  bool operator==(const application &other) const { return i == other.i; }
  bool operator!=(const application &other) const { return i != other.i; }
  const Application *operator->() const;
};

namespace std {
template <> struct hash<application> {
  size_t operator()(const application &k) const { return k.i; }
};
template <> struct hash<const application> {
  size_t operator()(const application &k) const { return k.i; }
};
} // namespace std

struct Application {
  // {'via': 'rule', 'conditions': 'std::unordered_map<token,
  // std::vector<application>>', 'result': 'judgment', 'args':
  // 'std::unordered_map<token, std::vector<application>>'}
  const rule via;
  const std::unordered_map<token, std::vector<application>> conditions;
  const judgment result;
  const std::unordered_map<token, std::vector<application>> args;
  // {'body': '{ if(via) return 0; else return result; }', 'type': 'judgment'}
  judgment hypothesis_or_empty() const;

  Application()
      : via(rule()),
        conditions(std::unordered_map<token, std::vector<application>>()),
        result(judgment()),
        args(std::unordered_map<token, std::vector<application>>()) {}

  Application(
      const rule &via,
      const std::unordered_map<token, std::vector<application>> &conditions,
      const judgment &result,
      const std::unordered_map<token, std::vector<application>> &args)
      : via(via), conditions(conditions), result(result), args(args) {}

  static application
  create(const rule &via,
         const std::unordered_map<token, std::vector<application>> &conditions,
         const judgment &result,
         const std::unordered_map<token, std::vector<application>> &args);

  static application get_or_create(const judgment &x);
  application save() const { return get_or_create(hypothesis_or_empty()); }

  Application(judgment j) : via(0), result(j) {}

  inline static application get_or_create_hypothesis(judgment j) {
    return get_or_create(j);
  }
  static application create(judgment j) { return create(rule(), {}, j, {}); }
  std::unordered_set<judgment> assumptions() const {
    std::unordered_set<judgment> rv;
    for (const auto &[type, conditions_of_type] : conditions) {
      for (const auto &c : conditions_of_type) {
        if (!c->via && c->conditions.size() == 0)
          continue;
        const auto &c_assumptions = c.assumptions();
        rv.insert(c_assumptions.begin(); c_assumptions.end());
      }
    }
    return rv;
  }
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
