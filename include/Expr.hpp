#pragma once
#include <memory>
#include <any>
#include "Token.hpp"

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor{
public:
    virtual ~Visitor() = default;
    virtual std::any visit(Binary& expr) = 0;
    virtual std::any visit(Literal& expr) = 0;
    virtual std::any visit(Grouping& expr) = 0;
    virtual std::any visit(Unary& expr) = 0;
};


class Expr{
public:
    virtual ~Expr() = default;

    virtual std::any accept(Visitor& visitor) = 0;
};

class Binary : public Expr{
public:

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    // Constructor
   Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Unary : public Expr{
public:
    Token op;

    std::unique_ptr<Expr> right;
    Unary( Token op, std::unique_ptr<Expr> right)
        :  op(op), right(std::move(right)) {}

    std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

};
class Literal : public Expr{
public:
    const std::any value;

    Literal(std::any value) : value(std::move(value)){}

    std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Grouping : public Expr{
public:
    std::unique_ptr<Expr> expression;
    Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression)){}

    std::any accept(Visitor& visitor) override{
        return visitor.visit(*this);
    }
};


