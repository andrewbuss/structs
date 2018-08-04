#pragma once

#include "hashutils.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rule.h>

#include <list>

using application = int;

struct Application {
  // {'via': 'rule', 'condition_proofs': 'std::unordered_set<application>',
  // 'result': 'judgment', 'args': 'std::vector<judgment>'}
  const rule via;
  const std::unordered_set<application> condition_proofs;
  const judgment result;
  const std::vector<judgment> args;
  // {'body': '{ if(via != 0) return 0; else return result; }', 'type':
  // 'judgment'}
  judgment hypothesis_or_empty() const {
    if (via != 0)
      return 0;
    else
      return result;
  }
  static const Application &get(application a) { return all_applications[a]; }

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
         const judgment &result, const std::vector<judgment> &args) {
    return Application(via, condition_proofs, result, args).save();
  }

  application save() const {
    all_applications.push_back(
        Application{via, condition_proofs, result, args});
    return index(all_applications.size() - 1);
  }

  static std::vector<Application> all_applications;

  static application get_or_create(const judgment &x) {
    auto a = lookup_by_hypothesis(x);
    if (!a)
      return create(x);
    return a;
  }

  // {'getter': 'via', 'type': 'rule'}
  using lookup_by_rule_index_type = std::unordered_multimap<rule, application>;
  static lookup_by_rule_index_type lookup_by_rule_index;
  using lookup_by_rule_index_iterator =
      lookup_by_rule_index_type::const_iterator;

  static std::pair<lookup_by_rule_index_iterator, lookup_by_rule_index_iterator>
  lookup_by_rule(const rule &x) {
    return lookup_by_rule_index.equal_range(x);
  }

  // {'getter': 'hypothesis_or_empty()', 'unique': True, 'type': 'judgment'}
  static std::unordered_map<judgment, application> lookup_by_hypothesis_index;
  static application lookup_by_hypothesis(const judgment &x) {
    auto it = lookup_by_hypothesis_index.find(x);
    if (it == lookup_by_hypothesis_index.end())
      return 0;
    return it->second;
  }

  // {'getter': 'result', 'type': 'judgment'}
  using lookup_by_result_index_type =
      std::unordered_multimap<judgment, application>;
  static lookup_by_result_index_type lookup_by_result_index;
  using lookup_by_result_index_iterator =
      lookup_by_result_index_type::const_iterator;

  static std::pair<lookup_by_result_index_iterator,
                   lookup_by_result_index_iterator>
  lookup_by_result(const judgment &x) {
    return lookup_by_result_index.equal_range(x);
  }

  static application index(const application a) {
    const auto obj = all_applications[a];
    const auto &obj_via = obj.via;
    lookup_by_rule_index.emplace(obj_via, a);
    const auto &obj_hypothesis_or_empty = obj.hypothesis_or_empty();
    lookup_by_hypothesis_index[obj_hypothesis_or_empty] = a;
    const auto &obj_result = obj.result;
    lookup_by_result_index.emplace(obj_result, a);
    return a;
  }

  Application(judgment j) : via(0), result(j) {}

  inline static application get_or_create_hypothesis(judgment j) {
    return get_or_create(j);
  }
  static application create(judgment j) { return Application(j).save(); }
};

std::ostream &operator<<(std::ostream &os, const Application &a);
