#include <judgment.h>
#include <rule.h>

#include <set>

std::vector<Rule> Rule::all_rules{{}};

Rule::lookup_by_signature_index_type Rule::lookup_by_signature_index;

Rule::lookup_by_conclusion_index_type Rule::lookup_by_conclusion_index;

Rule::lookup_by_conclusion_type_index_type
    Rule::lookup_by_conclusion_type_index;

Rule::lookup_by_label_index_type Rule::lookup_by_label_index;

Rule::lookup_by_condition_index_type Rule::lookup_by_condition_index;

std::ostream &operator<<(std::ostream &os, const Rule &r) {
  os << "Rule{";
  os << r.conclusion;
  os << ", " << r.conditions;
  os << ", " << r.label;
  return os << "}";
}
