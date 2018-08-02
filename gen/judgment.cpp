

#include <judgment.h>

#include <token.h>

std::vector<Judgment> Judgment::all_judgments{{}};

Judgment::lookup_by_type_index_type Judgment::lookup_by_type_index;

std::unordered_map<std::vector<token>, judgment> Judgment::lookup_by_cs_index;

Judgment::lookup_by_token_index_type Judgment::lookup_by_token_index;

std::ostream &operator<<(std::ostream &os, const Judgment &j) {
  os << "Judgment{";
  os << j.cs;
  return os << "}";
}
