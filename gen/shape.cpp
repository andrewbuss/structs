

#include <shape.h>

std::vector<Shape> ShapeIndex::all_Shapes{{}};
const Shape *shape::operator->() const {
  return ShapeIndex::all_Shapes.data() + i;
}
const Shape &shape::operator*() const { return ShapeIndex::all_Shapes[i]; }

std::unordered_map<token, shape> ShapeIndex::lookup_by_tok_index;

ShapeIndex::lookup_by_type_index_type ShapeIndex::lookup_by_type_index;

shape Shape::create(const tok &tok) {
  ShapeIndex::all_Shapes.push_back({tok});
  shape s = {(int)ShapeIndex::all_Shapes.size() - 1};
  return ShapeIndex::index(s);
}

shape Shape::get_or_create(const token &x) {
  auto s = ShapeIndex::lookup_by_tok(x);
  if (!s)
    return create(x);
  return s;
}

// {'getter': 'tok', 'unique': True, 'type': 'token'}
shape ShapeIndex::lookup_by_tok(const token &x) {
  auto it = lookup_by_tok_index.find(x);
  if (it == lookup_by_tok_index.end())
    return shape();
  return it->second;
}

// {'getter': 'type', 'type': 'token'}
std::pair<ShapeIndex::lookup_by_type_index_iterator,
          ShapeIndex::lookup_by_type_index_iterator>
ShapeIndex::lookup_by_type(const token &x) {
  return lookup_by_type_index.equal_range(x);
}

shape ShapeIndex::index(const shape s) {
  const auto &obj = *s;
  const auto &obj_tok = obj.tok;
  lookup_by_tok_index[obj_tok] = s;
  const auto &obj_type = obj.type;
  lookup_by_type_index.emplace(obj_type, s);
  return s;
}

/*
std::ostream& operator<<(std::ostream& os, const Shape& s) {
  os << "Shape{";
  os <<  s.tok;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const shape& s) {
  return os << s.i;
}
*/
