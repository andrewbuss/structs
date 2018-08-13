

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "type_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token_ptr.h>

#include <metavar.h>

struct Type {
  const token tok;
  mutable std::vector<metavar> mvs;

  Type() : tok(token()) {}

  Type(const token &tok) : tok(tok) {}

  static type create(const token &tok);

  static type get_or_create(const token &x);
  static type get_if_exists(const token &x);
  type save() const { return get_or_create(tok); }
};

struct TypeIndex {

  // {'getter': 'tok', 'unique': True, 'type': 'token'}
  static std::unordered_map<token, type> lookup_by_tok_index;
  static type lookup_by_tok(const token &x);

  static type index(type);
  static std::vector<Type> all_Types;
};

std::ostream &operator<<(std::ostream &os, const Type &);
std::ostream &operator<<(std::ostream &os, const type &);
