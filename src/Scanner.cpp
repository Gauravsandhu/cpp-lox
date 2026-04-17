
#include <string>
#include "Token.hpp"
#include "TokenType.hpp"
#include <vector>
#include <iostream>
#include <any>
#include <unordered_map>


using namespace std;

class Scanner{

    string source;
    vector<Token> tokens;
    int start = 0, current = 0;
    int line = 1;

    std::unordered_map<std::string, TokenType> keywords = {
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

public:

    Scanner(string source){
        this-> source = source;
    };

    bool isAtEnd(){
        if (current == source.length()){
            return true;
        };
        return false;
    }


    vector<Token> scanTokens(){
        
        while(!isAtEnd()){
            start = current;
            scanToken();
        }
        tokens.push_back(Token(TokenType::EOF_TOKEN, "", "", line));
        return tokens;
    }

    // This is the real token scanner thing which has the conditions to add token into our Vector
    void scanToken(){
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
            case '!': addToken(match('=')?TokenType::BANG_EQUAL : TokenType::BANG); break;
            case '=': addToken(match('=')?TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
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

    
    // just gives character at curr and moves curr forward
    char advance(){
        return(source[current++]);
    }

    // 
    void addToken(TokenType type){
        addToken(type, any());
    }

    void addToken(TokenType type, any literal){
        int length = current - start;
        string text = source.substr(start, length);
        tokens.push_back(Token(type,text,literal,line));
    }
    
    //this checks if next character is what we expected or not (for '!=' and '<=' type of tokens)
    bool match(char expected){
        if(!isAtEnd()){
             if(source[current] == expected){
                current++;
                return true;
            }
        }
       
        return false;
    }

    char peek(){
        if(isAtEnd()) return '\0';
        return source[current];
    }

    char peekNext(){
        if(current + 1 >= source.length()) return '\0';
        return source[current+1];
    }


    void str(){
        while(peek() != '"' && !isAtEnd()){
            if(peek() == '\n')line++;
            advance();
        }

        if(isAtEnd()){
            cout << "Error on line: " << line;
            return;
        }
        
        advance();
        string value = source.substr(start+1,current - start -2);
        addToken(TokenType::STRING, value);
    }
    

    bool isDigit(char c){
        return c >= '0' && c<= '9';
    }

    void number(){
        while(isDigit(peek()))  advance();
            if(peek() == '.' && isDigit(peekNext())){
                advance();
            }
        while(isDigit(peek())) advance();
        string text = source.substr(start, current - start);
        addToken(TokenType::NUMBER, stod(text));
    }


    bool isAlpha(char c){
        return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
    }

    bool isAlphaNumeric(char c){
        return isAlpha(c) || isDigit(c);
    }

    void identifier(){
        while(isAlphaNumeric(peek())){
            advance();
        }
        string text = source.substr(start, current - start);

        TokenType type;
    if (keywords.find(text) == keywords.end()) {
        // Not found in map? It's a name the user made (Identifier)
        type = TokenType::IDENTIFIER;
    } else {
        // Found in map? Use the specific keyword type (like IF or VAR)
        type = keywords.at(text);
    }
    addToken(type);
    }

    void error(int line, string s){
        cout << "Error at line: " << line;
    }

};