#ifndef CPPLOX_TOKEN_HPP
#define CPPLOX_TOKEN_HPP

#include <any>
#include <string>
#include "TokenType.hpp"


struct Token {
  const TokenType type;
  const std::string lexeme;
  const any literal;
  const int line;

  Token(TokenType type, std::string lexeme, any literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}
};

#endif