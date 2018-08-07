

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "shape_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

#include <vector>

#include <cassert>

#include <metavar.h>

struct Shape {
  const std::vector<token> cs;
  // {'body': '{\n  if(cs.size() == 0) return type();\n  assert(cs[0]->mv);\n
  // return cs[0]->mv->typ;\n}', 'type': 'type'}
  type typ() const;

  Shape() : cs(std::vector<token>()) {}

  Shape(const std::vector<token> &cs) : cs(cs) {}

  static shape create(const std::vector<token> &cs);

  static shape get_or_create(const std::vector<token> &x);
  static shape get_if_exists(const std::vector<token> &x);
  shape save() const { return get_or_create(cs); }
};

struct ShapeIndex {

  // {'getter': 'typ()', 'type': 'type'}
  using lookup_by_type_index_type = std::unordered_multimap<type, shape>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;
  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const type &x);

  // {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
  static std::unordered_map<std::vector<token>, shape> lookup_by_cs_index;
  static shape lookup_by_cs(const std::vector<token> &x);

  // {'getter': 'cs', 'type': 'token', 'iterable': True}
  using lookup_by_token_index_type = std::unordered_multimap<token, shape>;
  static lookup_by_token_index_type lookup_by_token_index;
  using lookup_by_token_index_iterator =
      lookup_by_token_index_type::const_iterator;
  static std::pair<lookup_by_token_index_iterator,
                   lookup_by_token_index_iterator>
  lookup_by_token(const token &x);

  static shape index(shape);
  static std::vector<Shape> all_Shapes;
};

std::ostream &operator<<(std::ostream &os, const Shape &);
std::ostream &operator<<(std::ostream &os, const shape &);
