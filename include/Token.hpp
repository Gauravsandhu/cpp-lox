#ifndef CPPLOX_TOKEN_HPP
#define CPPLOX_TOKEN_HPP

#include <any>
#include <string>
#include "TokenType.hpp"


struct Token {
  TokenType type;
  std::string lexeme;
  std::any literal;
  int line;

  Token(TokenType type, std::string lexeme, std::any literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}
};

#endif