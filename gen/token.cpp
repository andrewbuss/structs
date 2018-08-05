

#include <token.h>

std::vector<Token> all_tokens{{}};
const Token *token::operator->() const { return all_tokens.data() + i; }
const Token &token::operator*() const { return all_tokens[i]; }

std::unordered_map<std::string, token> TokenIndex::lookup_by_s_index;

token Token::create(const std::string &s) {
  all_tokens.push_back({s});
  token t = {(int)all_tokens.size() - 1};
  return TokenIndex::index(t);
}

token Token::get_or_create(const std::string &x) {
  auto t = TokenIndex::lookup_by_s(x);
  if (!t)
    return create(x);
  return t;
}

// {'getter': 's', 'unique': True, 'type': 'std::string'}
token TokenIndex::lookup_by_s(const std::string &x) {
  auto it = lookup_by_s_index.find(x);
  if (it == lookup_by_s_index.end())
    return token();
  return it->second;
}

token TokenIndex::index(const token t) {
  const auto &obj = *t;
  const auto &obj_s = obj.s;
  lookup_by_s_index[obj_s] = t;
  return t;
}
/*
std::ostream& operator<<(std::ostream& os, const Token& t) {
  os << "Token{";
  os <<  t.s;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const token& t) {
  return os << t.i;
}
*/
