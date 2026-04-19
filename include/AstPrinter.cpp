#include "AstPrinter.hpp"

// 1. Literal Base Case
std::any AstPrinter::visit(Literal& expr) {
    if (!expr.value.has_value()) return std::string("nil");

    if (expr.value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(expr.value);
    }
    if (expr.value.type() == typeid(double)) {
        // Simple conversion for demonstration
        return std::to_string(std::any_cast<double>(expr.value)); 
    }
    return std::string("unknown");
}

// 2. Unary Recursion
std::any AstPrinter::visit(Unary& expr) {
    std::string rightStr = std::any_cast<std::string>(expr.right->accept(*this));
    return "(" + expr.op.lexeme + " " + rightStr + ")";
}

// 3. Binary Recursion
std::any AstPrinter::visit(Binary& expr) {
    std::string leftStr = std::any_cast<std::string>(expr.left->accept(*this));
    std::string rightStr = std::any_cast<std::string>(expr.right->accept(*this));
    return "(" + expr.op.lexeme + " " + leftStr + " " + rightStr + ")";
}

// 4. Grouping Recursion
std::any AstPrinter::visit(Grouping& expr) {
    std::string innerStr = std::any_cast<std::string>(expr.expression->accept(*this));
    return "(group " + innerStr + ")";
}