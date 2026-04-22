#include <iostream>
#include <string>
#include <vector>
#include "../include/Scanner.hpp"
#include "../include/Parser.hpp"
#include "../include/AstPrinter.hpp"
#include "../include/Token.hpp"



using namespace std;
int main() {
    std::cout << "--- Program Started ---" << std::endl;

    string source = "-123 * (45.67)";
    
    Scanner scanner(source);
    vector<Token> tokens = scanner.scanTokens();
    std::cout << "Tokens found: " << tokens.size() << std::endl;

    Parser parser(tokens);
    auto expression = parser.parse();

    if (expression != nullptr) {
        AstPrinter printer;
        std::cout << "Result: " << printer.print(expression) << std::endl;
    } else {
        std::cout << "Parser returned null!" << std::endl;
    }

    std::cout << "--- Program Finished ---" << std::endl;
    return 0;
}