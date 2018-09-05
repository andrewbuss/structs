

#pragma once

#include "axiom_ptr.h"
#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <substitution.h>

#include <vector>

using axioms = std::vector<axiom>;

using substitution_substitutions = std::pair<substitution, substitutions>;

struct Axiom {
  const substitution sub;
  const substitutions hyps;
  // {'body': '{ return std::make_pair(sub, hyps); }', 'type':
  // 'substitution_substitutions'}
  substitution_substitutions sub_hyps() const;

  Axiom() : sub(substitution()), hyps(substitutions()) {}

  Axiom(const substitution &sub, const substitutions &hyps)
      : sub(sub), hyps(hyps) {}

  static axiom create(const substitution &sub, const substitutions &hyps);

  static axiom get_or_create(const substitution_substitutions &x);
  static axiom get_if_exists(const substitution_substitutions &x);
  axiom save() const { return get_or_create(sub_hyps()); }

  inline static axiom create(const substitution_substitutions &sub_hyps) {
    return create(sub_hyps.first, sub_hyps.second);
  }
  static axiom get_or_create(const substitution sub,
                             const substitutions &hyps) {
    return get_or_create(std::make_pair(sub, hyps));
  }
};

struct AxiomIndex {

  // {'getter': 'sub', 'type': 'substitution'}
  using lookup_by_sub_index_type = std::unordered_multimap<substitution, axiom>;
  static lookup_by_sub_index_type lookup_by_sub_index;
  using lookup_by_sub_index_iterator = lookup_by_sub_index_type::const_iterator;
  static std::pair<lookup_by_sub_index_iterator, lookup_by_sub_index_iterator>
  lookup_by_sub(const substitution &x);

  // {'getter': 'sub_hyps()', 'unique': True, 'type':
  // 'substitution_substitutions'}
  static std::unordered_map<substitution_substitutions, axiom>
      lookup_by_sub_hyps_index;
  static axiom lookup_by_sub_hyps(const substitution_substitutions &x);

  // {'getter': 'hyps', 'type': 'substitution', 'iterable': True}
  using lookup_by_hyp_index_type = std::unordered_multimap<substitution, axiom>;
  static lookup_by_hyp_index_type lookup_by_hyp_index;
  using lookup_by_hyp_index_iterator = lookup_by_hyp_index_type::const_iterator;
  static std::pair<lookup_by_hyp_index_iterator, lookup_by_hyp_index_iterator>
  lookup_by_hyp(const substitution &x);

  static axiom index(axiom);
  static std::vector<Axiom> all_Axioms;
};

std::ostream &operator<<(std::ostream &os, const Axiom &);
std::ostream &operator<<(std::ostream &os, const axiom &);
