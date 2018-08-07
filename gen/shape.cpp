

#include <shape.h>

std::vector<Shape> ShapeIndex::all_Shapes{{}};
const Shape *shape::operator->() const {
  return ShapeIndex::all_Shapes.data() + i;
}
const Shape &shape::operator*() const { return ShapeIndex::all_Shapes[i]; }

ShapeIndex::lookup_by_type_index_type ShapeIndex::lookup_by_type_index;

std::unordered_map<std::vector<token>, shape> ShapeIndex::lookup_by_cs_index;

ShapeIndex::lookup_by_token_index_type ShapeIndex::lookup_by_token_index;

// {'body': '{\n  if(cs.size() == 0) return type();\n  assert(cs[0]->mv);\n
// return cs[0]->mv->typ;\n}', 'type': 'type'}
type Shape::typ() const {
  {
    if (cs.size() == 0)
      return type();
    assert(cs[0]->mv);
    return cs[0]->mv->typ;
  }
}

shape Shape::create(const std::vector<token> &cs) {
  ShapeIndex::all_Shapes.push_back({cs});
  shape s{(int)ShapeIndex::all_Shapes.size() - 1};

  return ShapeIndex::index(s);
}

shape Shape::get_or_create(const std::vector<token> &x) {
  auto s = ShapeIndex::lookup_by_cs(x);
  if (!s)
    return create(x);
  return s;
}

shape Shape::get_if_exists(const std::vector<token> &x) {
  auto s = ShapeIndex::lookup_by_cs(x);
  if (!s)
    return shape{0};
  return s;
}

// {'getter': 'typ()', 'type': 'type'}
std::pair<ShapeIndex::lookup_by_type_index_iterator,
          ShapeIndex::lookup_by_type_index_iterator>
ShapeIndex::lookup_by_type(const type &x) {
  return lookup_by_type_index.equal_range(x);
}

// {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
shape ShapeIndex::lookup_by_cs(const std::vector<token> &x) {
  auto it = lookup_by_cs_index.find(x);
  if (it == lookup_by_cs_index.end())
    return shape();
  return it->second;
}

// {'getter': 'cs', 'type': 'token', 'iterable': True}
std::pair<ShapeIndex::lookup_by_token_index_iterator,
          ShapeIndex::lookup_by_token_index_iterator>
ShapeIndex::lookup_by_token(const token &x) {
  return lookup_by_token_index.equal_range(x);
}

shape ShapeIndex::index(const shape s) {
  const auto &obj = *s;
  const auto &obj_typ = obj.typ();
  lookup_by_type_index.emplace(obj_typ, s);
  const auto &obj_cs = obj.cs;
  lookup_by_cs_index[obj_cs] = s;
  for (const auto &obj_cs : obj.cs)
    lookup_by_token_index.emplace(obj_cs, s);
  return s;
}

/*
std::ostream& operator<<(std::ostream& os, const Shape& s) {
  os << "Shape{";
  os <<  s.cs;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const shape& s) {
  return os << s.i;
}
*/
