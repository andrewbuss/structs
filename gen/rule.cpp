#include <judgment.h>
#include <rule.h>

std::vector<Rule> Rule::all_rules{{}};

std::ostream &operator<<(std::ostream &os, const Rule &r) {
  os << "Rule{";
  os << r.conditions;
  os << ", " << r.conclusion;
  return os << "}";
}
