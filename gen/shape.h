#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Shape;

struct shape {
  int i;
  const Shape &operator*() const;
  operator bool() const { return i != 0; }
  shape() : i(0) {}
  shape(int i) : i(i) {}
  shape operator=(const shape &other) { return i = other.i; }
  bool operator==(const shape &other) const { return i == other.i; }
  bool operator!=(const shape &other) const { return i != other.i; }
  const Shape *operator->() const;
};

namespace std {
template <> struct hash<shape> {
  size_t operator()(const shape &k) const { return k.i; }
};
template <> struct hash<const shape> {
  size_t operator()(const shape &k) const { return k.i; }
};
} // namespace std

struct Shape {
  // {'tok': 'tok'}
  const tok tok;

  Shape() : tok(tok()) {}

  Shape(const tok &tok) : tok(tok) {}

  static shape create(const tok &tok);

  static shape get_or_create(const token &x);
  shape save() const { return get_or_create(tok); }
};

struct ShapeIndex {

  // {'getter': 'tok', 'unique': True, 'type': 'token'}
  static std::unordered_map<token, shape> lookup_by_tok_index;
  static shape lookup_by_tok(const token &x);

  // {'getter': 'type', 'type': 'token'}
  using lookup_by_type_index_type = std::unordered_multimap<token, shape>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;
  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const token &x);

  static shape index(shape);
  static std::vector<Shape> all_Shapes;
};

std::ostream &operator<<(std::ostream &os, const Shape &);
std::ostream &operator<<(std::ostream &os, const shape &);
