
#pragma once
#include <any>
#include "Expr.hpp"

using namespace std;

// AST PRINTER

class AstPrinter : public Visitor{
public:
    string print(unique_ptr<Expr>& expr){
        return any_cast<string>(expr->accept(*this));
    }


    // Our promises to the Visitor blueprint
    std::any visit(Binary& expr) override;
    std::any visit(Grouping& expr) override;
    std::any visit(Literal& expr) override;
    std::any visit(Unary& expr) override;
};

   