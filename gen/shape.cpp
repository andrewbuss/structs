

#include <shape.h>

std::vector<Shape> ShapeIndex::all_Shapes{{}};
const Shape *shape::operator->() const {
  return ShapeIndex::all_Shapes.data() + i;
}
const Shape &shape::operator*() const { return ShapeIndex::all_Shapes[i]; }
shape shape::null = 0;

ShapeIndex::lookup_by_tok_index_type ShapeIndex::lookup_by_tok_index;

ShapeIndex::lookup_by_typ_index_type ShapeIndex::lookup_by_typ_index;

std::unordered_map<std::vector<token>, shape> ShapeIndex::lookup_by_shp_index;

// {'body': '{ return TypeIndex::lookup_by_tok(shp[0]); }', 'type': 'type'}
type Shape::typ() const {
  { return TypeIndex::lookup_by_tok(shp[0]); }
}

shape Shape::create(const std::vector<token> &shp) {
  ShapeIndex::all_Shapes.push_back({shp});
  shape s{(int)ShapeIndex::all_Shapes.size() - 1};

  return ShapeIndex::index(s);
}

shape Shape::get_or_create(const std::vector<token> &x) {
  auto s = ShapeIndex::lookup_by_shp_index.find(x);
  if (s == ShapeIndex::lookup_by_shp_index.end())
    return create(x);
  return s->second;
}

shape Shape::get_if_exists(const std::vector<token> &x) {
  auto s = ShapeIndex::lookup_by_shp(x);
  if (!s)
    return shape::null;
  return s;
}

// {'getter': 'shp', 'type': 'token', 'iterable': True}
std::pair<ShapeIndex::lookup_by_tok_index_iterator,
          ShapeIndex::lookup_by_tok_index_iterator>
ShapeIndex::lookup_by_tok(const token &x) {
  return lookup_by_tok_index.equal_range(x);
}

// {'getter': 'typ()', 'type': 'type'}
std::pair<ShapeIndex::lookup_by_typ_index_iterator,
          ShapeIndex::lookup_by_typ_index_iterator>
ShapeIndex::lookup_by_typ(const type &x) {
  return lookup_by_typ_index.equal_range(x);
}

// {'getter': 'shp', 'unique': True, 'type': 'std::vector<token>'}
shape ShapeIndex::lookup_by_shp(const std::vector<token> &x) {
  auto it = lookup_by_shp_index.find(x);
  if (it == lookup_by_shp_index.end())
    return shape();
  return it->second;
}

shape ShapeIndex::index(const shape s) {
  const auto &obj = *s;
  for (const auto &obj_shp : obj.shp)
    lookup_by_tok_index.emplace(obj_shp, s);
  const auto &obj_typ = obj.typ();
  lookup_by_typ_index.emplace(obj_typ, s);
  const auto &obj_shp = obj.shp;
  lookup_by_shp_index[obj_shp] = s;
  return s;
}

std::ostream &operator<<(std::ostream &os, const Shape &s) {
  os << "Shape{";
  os << s.shp;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const shape &s) {
  return os << *(s) << "@" << s.i;
}
