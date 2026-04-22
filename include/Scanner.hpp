#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include "Token.hpp"
#include "TokenType.hpp"

class Scanner {
    // 🔒 Private data members (The Scanner's internal state)
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    std::unordered_map<std::string, TokenType> keywords;

    // 🛠️ Private helper functions (Internal logic)
    bool isAtEnd();
    void scanToken();
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();
    void str();
    bool isDigit(char c);
    void number();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void identifier();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    void error(int line, std::string message);

public:
    // 🔓 Public API (What other files can call)
    Scanner(std::string source);
    std::vector<Token> scanTokens();
};

#endif