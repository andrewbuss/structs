

#include <token.h>

std::vector<Token> TokenIndex::all_Tokens{{}};
const Token *token::operator->() const {
  return TokenIndex::all_Tokens.data() + i;
}
const Token &token::operator*() const { return TokenIndex::all_Tokens[i]; }

std::unordered_map<std::string, token> TokenIndex::lookup_by_s_index;

// {'body': 'if(!typ)\n  typ = Type::get_or_create(this -
// TokenIndex::all_Tokens.data());\nreturn typ;', 'type': 'type'}
type Token::as_type() const {
  if (!typ)
    typ = Type::get_or_create(this - TokenIndex::all_Tokens.data());
  return typ;
}

token Token::create(const std::string &s) {
  TokenIndex::all_Tokens.push_back({s});
  token t{(int)TokenIndex::all_Tokens.size() - 1};
  auto pos = t->s.find('$');
  if (pos != std::string::npos) {
    token typ_token = TokenIndex::lookup_by_s(t->s.substr(0, pos));
    type typ = Type::get_or_create(typ_token);
    for (int i = 0; i < 16; i++) {
      metavar mv = Metavar::create(typ, i);
      typ->mvs.push_back(mv);
      mv->tok = t;
    }
    uint8_t i = std::stoi(t->s.substr(0, t->s.size() - pos - 1));
    t->mv = MetavarIndex::lookup_by_typ_i(std::make_pair(typ, i));
  }
  return TokenIndex::index(t);
}

token Token::get_or_create(const std::string &x) {
  auto t = TokenIndex::lookup_by_s(x);
  if (!t)
    return create(x);
  return t;
}

token Token::get_if_exists(const std::string &x) {
  auto t = TokenIndex::lookup_by_s(x);
  if (!t)
    return token{0};
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
