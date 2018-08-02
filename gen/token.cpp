#include <token.h>

std::vector<Token> Token::all_tokens{{}};

std::unordered_map<std::string, token> Token::lookup_by_s_index;

std::ostream &operator<<(std::ostream &os, const Token &t) {
  os << "Token{";
  os << t.s;
  return os << "}";
}
