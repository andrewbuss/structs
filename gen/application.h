#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rule.h>

#include <list>

// {'getter': 'via', 'type': 'rule'}
//#define lookup_by_rule_index_type std::unordered_multimap<rule, application>
//#define lookup_by_rule_index_iterator
//lookup_by_rule_index_type::const_iterator

// {'getter': 'result', 'type': 'judgment'}
//#define lookup_by_result_index_type std::unordered_multimap<judgment,
//application> #define lookup_by_result_index_iterator
//lookup_by_result_index_type::const_iterator

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
  size_t operator()(const application k) const { return hash<int>{}(k.i); }
};
} // namespace std

struct Application {
  // {'via': 'rule', 'condition_proofs': 'std::unordered_set<application>',
  // 'result': 'judgment', 'args': 'std::vector<judgment>'}
  const rule via;
  const std::unordered_set<application> condition_proofs;
  const judgment result;
  const std::vector<judgment> args;
  // {'body': '{ if(via) return 0; else return result; }', 'type': 'judgment'}
  judgment hypothesis_or_empty() const;

  Application()
      : via(rule()), condition_proofs(std::unordered_set<application>()),
        result(judgment()), args(std::vector<judgment>()) {}

  Application(const rule &via,
              const std::unordered_set<application> &condition_proofs,
              const judgment &result, const std::vector<judgment> &args)
      : via(via), condition_proofs(condition_proofs), result(result),
        args(args) {}
  static application
  create(const rule &via,
         const std::unordered_set<application> &condition_proofs,
         const judgment &result, const std::vector<judgment> &args);

  static application get_or_create(const judgment &x);

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
};

std::ostream &operator<<(std::ostream &os, const Application &);
std::ostream &operator<<(std::ostream &os, const application &);
