
#include <string>
#include <Token.hpp>
#include <vector>

using namespace std;

class Scanner{

    string source;
    vector<Token> tokens;
    int start = 0, current = 0;
    int line = 1;

public:

    Scanner(string source);

};