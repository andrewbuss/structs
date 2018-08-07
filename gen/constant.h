

#pragma once

#include "constant_ptr.h"
#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

struct Constant {
  // {'tok': 'token'}
  const token tok;

  Constant() : tok(token()) {}

  Constant(const token &tok) : tok(tok) {}

  static constant create(const token &tok);

  static constant get_or_create(const token &x);
  static constant get_if_exists(const token &x);
  constant save() const { return get_or_create(tok); }
};

struct ConstantIndex {

  // {'getter': 'tok', 'unique': True, 'type': 'token'}
  static std::unordered_map<token, constant> lookup_by_tok_index;
  static constant lookup_by_tok(const token &x);

  static constant index(constant);
  static std::vector<Constant> all_Constants;
};

std::ostream &operator<<(std::ostream &os, const Constant &);
std::ostream &operator<<(std::ostream &os, const constant &);
