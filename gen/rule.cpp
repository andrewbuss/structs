

#include <rule.h>

std::vector<Rule> RuleIndex::all_Rules{{}};
const Rule *rule::operator->() const { return RuleIndex::all_Rules.data() + i; }
const Rule &rule::operator*() const { return RuleIndex::all_Rules[i]; }
rule rule::null = 0;

RuleIndex::lookup_by_signature_index_type RuleIndex::lookup_by_signature_index;

RuleIndex::lookup_by_conclusion_index_type
    RuleIndex::lookup_by_conclusion_index;

RuleIndex::lookup_by_conclusion_type_index_type
    RuleIndex::lookup_by_conclusion_type_index;

std::unordered_map<token, rule> RuleIndex::lookup_by_label_index;

rule Rule::create(
    const Arity &arity, const judgment &conclusion,
    const std::unordered_map<token, std::vector<judgment>> &conditions,
    const token &label) {
  RuleIndex::all_Rules.push_back({arity, conclusion, conditions, label});
  rule r{(int)RuleIndex::all_Rules.size() - 1};

  return RuleIndex::index(r);
}

// {'getter': 'arity', 'type': 'Arity'}
std::pair<RuleIndex::lookup_by_signature_index_iterator,
          RuleIndex::lookup_by_signature_index_iterator>
RuleIndex::lookup_by_signature(const Arity &x) {
  return lookup_by_signature_index.equal_range(x);
}

// {'getter': 'conclusion', 'type': 'judgment'}
std::pair<RuleIndex::lookup_by_conclusion_index_iterator,
          RuleIndex::lookup_by_conclusion_index_iterator>
RuleIndex::lookup_by_conclusion(const judgment &x) {
  return lookup_by_conclusion_index.equal_range(x);
}

// {'getter': 'conclusion->shp->typ()', 'type': 'token'}
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
  const auto &obj_conclusion__shp__typ = obj.conclusion->shp->typ();
  lookup_by_conclusion_type_index.emplace(obj_conclusion__shp__typ, r);
  const auto &obj_label = obj.label;
  lookup_by_label_index[obj_label] = r;
  return r;
}

std::ostream &operator<<(std::ostream &os, const Rule &r) {
  os << "Rule{";
  os << r.arity;
  os << ", " << r.conclusion;
  os << ", " << r.conditions;
  os << ", " << r.label;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const rule &r) {
  return os << *(r) << "@" << r.i;
}
