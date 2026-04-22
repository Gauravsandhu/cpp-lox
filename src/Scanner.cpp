#include <string>
#include <vector>
#include <iostream>
#include <any>
#include <unordered_map>
#include "../include/Token.hpp"
#include "../include/TokenType.hpp"
#include "../include/Scanner.hpp" // <-- Added this so it reads its own menu!

using namespace std;

// Notice every function now has Scanner:: attached to it!

Scanner::Scanner(string source){
    this->source = source;
}

bool Scanner::isAtEnd(){
    if (current == source.length()){
        return true;
    }
    return false;
}

vector<Token> Scanner::scanTokens(){
    while(!isAtEnd()){
        start = current;
        scanToken();
    }
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", "", line));
    return tokens;
}

void Scanner::scanToken(){
    char c = advance();
    switch(c){
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '/': 
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(TokenType::SLASH); break;
            }; break;
        case '"': str(); break;
        
        default:
            if(isDigit(c)){
                number();
            } 
            else if(isAlpha(c)){
                identifier();
            }else{
                error(line, "Unexpected character.");
            }; break;
    }
}

char Scanner::advance(){
    return(source[current++]);
}

void Scanner::addToken(TokenType type){
    addToken(type, any());
}

void Scanner::addToken(TokenType type, any literal){
    int length = current - start;
    string text = source.substr(start, length);
    tokens.push_back(Token(type,text,literal,line));
}

bool Scanner::match(char expected){
    if(!isAtEnd()){
         if(source[current] == expected){
            current++;
            return true;
        }
    }
    return false;
}

char Scanner::peek(){
    if(isAtEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext(){
    if(current + 1 >= source.length()) return '\0';
    return source[current+1];
}

void Scanner::str(){
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n')line++;
        advance();
    }

    if(isAtEnd()){
        cout << "Error on line: " << line;
        return;
    }
    
    advance();
    string value = source.substr(start+1, current - start - 2);
    addToken(TokenType::STRING, value);
}

bool Scanner::isDigit(char c){
    return c >= '0' && c <= '9';
}

void Scanner::number(){
    while(isDigit(peek())) advance();
    if(peek() == '.' && isDigit(peekNext())){
        advance();
    }
    while(isDigit(peek())) advance();
    string text = source.substr(start, current - start);
    addToken(TokenType::NUMBER, stod(text));
}

bool Scanner::isAlpha(char c){
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c);
}

void Scanner::identifier(){
    while(isAlphaNumeric(peek())){
        advance();
    }
    string text = source.substr(start, current - start);

    TokenType type;
    if (keywords.find(text) == keywords.end()) {
        type = TokenType::IDENTIFIER;
    } else {
        type = keywords.at(text);
    }
    addToken(type);
}

void Scanner::error(int line, string s){
    cout << "Error at line: " << line;
}