

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "proof_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <axiom.h>

#include <vector>

using axiom_axioms = std::pair<axiom, axioms>;

struct Proof {
  const axiom via;
  const axioms args;
  // {'body': '{ return std::make_pair(via, args); }', 'type': 'axiom_axioms'}
  axiom_axioms via_args() const;

  Proof() : via(axiom()), args(axioms()) {}

  Proof(const axiom &via, const axioms &args) : via(via), args(args) {}

  static proof create(const axiom &via, const axioms &args);

  proof save() const { return create(via, args); }
};

struct ProofIndex {

  // {'getter': 'via_args()', 'unique': True, 'type': 'axiom_axioms'}
  static std::unordered_map<axiom_axioms, proof> lookup_by_via_args_index;
  static proof lookup_by_via_args(const axiom_axioms &x);

  // {'getter': 'via', 'type': 'axiom'}
  using lookup_by_via_index_type = std::unordered_multimap<axiom, proof>;
  static lookup_by_via_index_type lookup_by_via_index;
  using lookup_by_via_index_iterator = lookup_by_via_index_type::const_iterator;
  static std::pair<lookup_by_via_index_iterator, lookup_by_via_index_iterator>
  lookup_by_via(const axiom &x);

  // {'getter': 'args', 'type': 'axiom', 'iterable': True}
  using lookup_by_arg_index_type = std::unordered_multimap<axiom, proof>;
  static lookup_by_arg_index_type lookup_by_arg_index;
  using lookup_by_arg_index_iterator = lookup_by_arg_index_type::const_iterator;
  static std::pair<lookup_by_arg_index_iterator, lookup_by_arg_index_iterator>
  lookup_by_arg(const axiom &x);

  static proof index(proof);
  static std::vector<Proof> all_Proofs;
};

std::ostream &operator<<(std::ostream &os, const Proof &);
std::ostream &operator<<(std::ostream &os, const proof &);
