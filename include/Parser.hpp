#pragma once

#include <vector>
#include <initializer_list>
#include <memory>
#include "Token.hpp"
#include "Expr.hpp"

using namespace std;

class Parser{
private:
    vector<Token> tokens;
    int current = 0;

    unique_ptr<Expr> expression(){
       return equality();
    }

    std::unique_ptr<Expr> equality(){
        auto expr = comparison(); 

        while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
            Token op = previous();                
            auto right = comparison();          
       
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }
        return expr;
    }
    std::unique_ptr<Expr> comparison() {
    auto expr = term(); // 1. Grab the left side (calling the next rule down)

    // 2. Loop as long as we see >, >=, <, or <=
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();                 // Grab the operator
        auto right = term();                   // 3. Grab the right side
        
        // 4. Bundle them into a Binary AST node
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}
    std::unique_ptr<Expr> term(){
        auto expr = factor();
        while(match({TokenType::PLUS, TokenType::MINUS})){
            Token op = previous();
            auto right = factor();

            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }
        return expr;
    }


    std::unique_ptr<Expr> factor(){
        auto expr = unary(); // Grab the left side

    // Loop as long as we see / or *
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();                 // Grab the operator
        auto right = unary();                  // Grab the right side
        
        // Bundle them into a Binary AST node
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
    }

    // Grammar Rule: unary -> ( "!" | "-" ) unary | primary
std::unique_ptr<Expr> unary() {
    // 1. If the current token is ! or -
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();                 // Grab the operator
        auto right = unary();                  // Grab the right side (recursive)
        
        // Bundle them into a Unary AST node
        return std::make_unique<Unary>(op, std::move(right));
    }

    // 2. Otherwise, drop down to the final level
    return primary();
}



std::unique_ptr<Expr> primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_unique<Literal>(true);
    if (match({TokenType::NIL})) return std::make_unique<Literal>(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(previous().literal);
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression(); 
        
        // The strict bouncer: MUST be a ')', or else throw the error
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        
        return std::make_unique<Grouping>(std::move(expr));
    }

    // If we reach this point, the token doesn't match any valid expression starter
    throw std::runtime_error("Expect expression.");
}

         //HELPER FUNCTIONS//
        //////////////////////
    Token consume(TokenType type, std::string message) {
    if (check(type)) return advance();
    
    // If the token doesn't match, we halt and throw the error message
    throw std::runtime_error(message);
}

    bool match(initializer_list<TokenType> types){
        for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
    };

    bool check(TokenType type){
        if(isAtEnd()) return false;
        return peek().type == type;
    };

    Token advance(){
        if(isAtEnd()){
            return peek();
        }
        return tokens[current++];
    };

    // checks if we reach the end
    bool isAtEnd(){
       
       return peek().type == TokenType::EOF_TOKEN;
    };

    // returns current Token
    Token peek(){
        return tokens[current];
    };

    Token previous() {
    return tokens[current - 1];
    }

public:
    Parser(vector<Token> tokens) : tokens(std::move(tokens)){}

    // the main function that kicks off the parsing process

    unique_ptr<Expr> parse(){
       
        try {
            return expression();
        } catch (const std::runtime_error& error) {
        
            return nullptr;
        }
    };

};