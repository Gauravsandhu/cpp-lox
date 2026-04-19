#pragma once
#include <memory>
#include <any>
#include "Token.hpp"
using namespace std;

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor{
public:
    virtual ~Visitor() = default;
    virtual any visit(Binary& expr) = 0;
    virtual any visit(Literal& expr) = 0;
    virtual any visit(Grouping& expr) = 0;
    virtual any visit(Unary& expr) = 0;
};


class Expr{
public:
    virtual ~Expr() = default;

    virtual any accept(Visitor& visitor) = 0;
};

class Binary : public Expr{
public:

    unique_ptr<Expr> left;
    Token op;
    unique_ptr<Expr> right;

    // Constructor
   Binary(unique_ptr<Expr> left, Token op, unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Unary : public Expr{
public:
    Token op;

    unique_ptr<Expr> right;
    Unary( Token op, unique_ptr<Expr> right)
        :  op(op), right(std::move(right)) {}

    any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

};
class Literal : public Expr{
public:
    const any value;

    Literal(any value) : value(std::move(value)){}

    any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }
};

class Grouping : public Expr{
public:
    unique_ptr<Expr> expression;
    Grouping(unique_ptr<Expr> expression) : expression(std::move(expression)){}

    any accept(Visitor& visitor) override{
        return visitor.visit(*this);
    }
};


