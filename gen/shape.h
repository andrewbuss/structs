

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "shape_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

#include <type.h>

#include <vector>

struct Shape {
  const std::vector<token> shp;
  // {'body': '{ return TypeIndex::lookup_by_tok(shp[0]); }', 'type': 'type'}
  type typ() const;

  Shape() : shp(std::vector<token>()) {}

  Shape(const std::vector<token> &shp) : shp(shp) {}

  static shape create(const std::vector<token> &shp);

  static shape get_or_create(const std::vector<token> &x);
  static shape get_if_exists(const std::vector<token> &x);
  shape save() const { return get_or_create(shp); }
};

struct ShapeIndex {

  // {'getter': 'shp', 'type': 'token', 'iterable': True}
  using lookup_by_tok_index_type = std::unordered_multimap<token, shape>;
  static lookup_by_tok_index_type lookup_by_tok_index;
  using lookup_by_tok_index_iterator = lookup_by_tok_index_type::const_iterator;
  static std::pair<lookup_by_tok_index_iterator, lookup_by_tok_index_iterator>
  lookup_by_tok(const token &x);

  // {'getter': 'typ()', 'type': 'type'}
  using lookup_by_typ_index_type = std::unordered_multimap<type, shape>;
  static lookup_by_typ_index_type lookup_by_typ_index;
  using lookup_by_typ_index_iterator = lookup_by_typ_index_type::const_iterator;
  static std::pair<lookup_by_typ_index_iterator, lookup_by_typ_index_iterator>
  lookup_by_typ(const type &x);

  // {'getter': 'shp', 'unique': True, 'type': 'std::vector<token>'}
  static std::unordered_map<std::vector<token>, shape> lookup_by_shp_index;
  static shape lookup_by_shp(const std::vector<token> &x);

  static shape index(shape);
  static std::vector<Shape> all_Shapes;
};

std::ostream &operator<<(std::ostream &os, const Shape &);
std::ostream &operator<<(std::ostream &os, const shape &);
