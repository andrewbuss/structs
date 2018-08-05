

#include <judgment.h>
#include <rule.h>

#include <token.h>

#include <set>

#include <unordered_map>

std::vector<Rule> all_rules{{}};
const Rule *rule::operator->() const { return all_rules.data() + i; }
const Rule &rule::operator*() const { return all_rules[i]; }

RuleIndex::lookup_by_signature_index_type RuleIndex::lookup_by_signature_index;

RuleIndex::lookup_by_conclusion_index_type
    RuleIndex::lookup_by_conclusion_index;

RuleIndex::lookup_by_conclusion_type_index_type
    RuleIndex::lookup_by_conclusion_type_index;

std::unordered_map<token, rule> RuleIndex::lookup_by_label_index;

// {'body': '{ return conclusion->type(); }', 'type': 'token'}
token Rule::conclusion_type() const {
  { return conclusion->type(); }
}

rule Rule::create(
    const std::unordered_map<token, int> &arity, const judgment &conclusion,
    const std::unordered_map<token, std::vector<judgment>> &conditions,
    const token &label) {
  all_rules.push_back({arity, conclusion, conditions, label});
  rule r = {(int)all_rules.size() - 1};
  return RuleIndex::index(r);
}

// {'getter': 'arity', 'type': 'std::unordered_map<token, int>'}
std::pair<RuleIndex::lookup_by_signature_index_iterator,
          RuleIndex::lookup_by_signature_index_iterator>
RuleIndex::lookup_by_signature(const std::unordered_map<token, int> &x) {
  return lookup_by_signature_index.equal_range(x);
}

// {'getter': 'conclusion', 'type': 'judgment'}
std::pair<RuleIndex::lookup_by_conclusion_index_iterator,
          RuleIndex::lookup_by_conclusion_index_iterator>
RuleIndex::lookup_by_conclusion(const judgment &x) {
  return lookup_by_conclusion_index.equal_range(x);
}

// {'getter': 'conclusion_type()', 'type': 'token'}
std::pair<RuleIndex::lookup_by_conclusion_type_index_iterator,
          RuleIndex::lookup_by_conclusion_type_index_iterator>
RuleIndex::lookup_by_conclusion_type(const token &x) {
  return lookup_by_conclusion_type_index.equal_range(x);
}

// {'getter': 'label', 'unique': True, 'type': 'token'}
rule RuleIndex::lookup_by_label(const token &x) {
  auto it = lookup_by_label_index.find(x);
  if (it == lookup_by_label_index.end())
    return rule();
  return it->second;
}

rule RuleIndex::index(const rule r) {
  const auto &obj = *r;
  const auto &obj_arity = obj.arity;
  lookup_by_signature_index.emplace(obj_arity, r);
  const auto &obj_conclusion = obj.conclusion;
  lookup_by_conclusion_index.emplace(obj_conclusion, r);
  const auto &obj_conclusion_type = obj.conclusion_type();
  lookup_by_conclusion_type_index.emplace(obj_conclusion_type, r);
  const auto &obj_label = obj.label;
  lookup_by_label_index[obj_label] = r;
  return r;
}
/*
std::ostream& operator<<(std::ostream& os, const Rule& r) {
  os << "Rule{";
  os <<  r.arity;os <<  ", " <<  r.conclusion;os <<  ", " <<  r.conditions;os <<
", " <<  r.label;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const rule& r) {
  return os << r.i;
}
*/
