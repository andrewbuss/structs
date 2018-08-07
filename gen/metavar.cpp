

#include <metavar.h>
#include <token.h>

std::vector<Metavar> MetavarIndex::all_Metavars{{}};
const Metavar *metavar::operator->() const {
  return MetavarIndex::all_Metavars.data() + i;
}
const Metavar &metavar::operator*() const {
  return MetavarIndex::all_Metavars[i];
}

std::unordered_map<token, metavar> MetavarIndex::lookup_by_tok_index;

MetavarIndex::lookup_by_type_index_type MetavarIndex::lookup_by_type_index;

metavar Metavar::create(const token &tok, const token &type) {
  MetavarIndex::all_Metavars.push_back({tok, type});
  metavar m = {(int)MetavarIndex::all_Metavars.size() - 1};
  return MetavarIndex::index(m);
}

metavar Metavar::get_or_create(const token &x) {
  auto m = MetavarIndex::lookup_by_tok(x);
  if (!m)
    return create(x);
  return m;
}

// {'getter': 'tok', 'unique': True, 'type': 'token'}
metavar MetavarIndex::lookup_by_tok(const token &x) {
  auto it = lookup_by_tok_index.find(x);
  if (it == lookup_by_tok_index.end())
    return metavar();
  return it->second;
}

// {'getter': 'type', 'type': 'token'}
std::pair<MetavarIndex::lookup_by_type_index_iterator,
          MetavarIndex::lookup_by_type_index_iterator>
MetavarIndex::lookup_by_type(const token &x) {
  return lookup_by_type_index.equal_range(x);
}

metavar MetavarIndex::index(const metavar m) {
  const auto &obj = *m;
  const auto &obj_tok = obj.tok;
  lookup_by_tok_index[obj_tok] = m;
  const auto &obj_type = obj.type;
  lookup_by_type_index.emplace(obj_type, m);
  return m;
}

/*
std::ostream& operator<<(std::ostream& os, const Metavar& m) {
  os << "Metavar{";
  os <<  m.tok;os <<  ", " <<  m.type;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const metavar& m) {
  return os << m.i;
}
*/
