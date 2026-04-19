#include <iostream>
#include "Expr.hpp"
#include "AstPrinter.hpp"
#include "TokenType.hpp"




int main() {
    // We are manually building the AST for: -123 * (45.67)

    // 1. Build the left side: -123
    // Using your real Token constructor: TokenType, lexeme, literal value (empty for operators), line number
    auto left = std::make_unique<Unary>(
        Token(TokenType::MINUS, "-", std::any(), 1), 
        std::make_unique<Literal>(123.0)
    );

    // 2. Build the right side: (45.67)
    auto right = std::make_unique<Grouping>(
        std::make_unique<Literal>(45.67)
    );

    // 3. Combine them with a Binary node: *
    std::unique_ptr<Expr> expression = std::make_unique<Binary>(
        std::move(left),
        Token(TokenType::STAR, "*", std::any(), 1),
        std::move(right)
    );

    // 4. Create the tool and start the engine
    AstPrinter printer;
    
    // Pass the unique_ptr directly, because your AstPrinter::print expects unique_ptr<Expr>&
    std::cout << "Final Output: " << printer.print(expression) << std::endl;

    return 0;
}