#include <iostream>
#include <queue>

#include "src/frontend/lexer/lexer.h"

void test_lexer() {
    const symbol s_class = std::make_shared<symbol_>("class", 0);
    const symbol s_const = std::make_shared<symbol_>("const", 1);
    const symbol s_identifier = std::make_shared<symbol_>("identifier", 2);
    const symbol s_int = std::make_shared<symbol_>("int", 3);
    const symbol s_float = std::make_shared<symbol_>("float", 4);

    const lexer::lexRule r_class {s_class, "class"};
    const lexer::lexRule r_const {s_const, "const"};
    const lexer::lexRule r_identifier {s_identifier, R"([aclonst_][aclonst_01]*)"};
    const lexer::lexRule r_int {s_int, R"(0|-?1[01]*)"};
    const lexer::lexRule r_float {s_float, R"((0|-?1[01]*)\.[01]*|\.[01][01]*)"};

    lexer::lexer lexer = lexer::lexer::compile({r_class, r_const, r_identifier, r_int, r_float});

    const std::string text = "class const 11010 010 const 0.11 1.0 0.1 .1 a_ost_c";

    const std::vector<token> tokens = lexer.lex_all(text);
    for(const token& i : tokens){
        std::cout << i.to_string() << "\n";
    }

    std::cout << "Success.\n";
}

int main() {
    test_lexer();
    return 0;
}