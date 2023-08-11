#include <iostream>

#include "src/frontend/lexer/lexer.h"

void test_lexer() {
    using lexer::lexRule, lexer::lexer;

    const symbol s_class = std::make_shared<symbol_>("class", 0);
    const symbol s_const = std::make_shared<symbol_>("const", 1);
    const symbol s_identifier = std::make_shared<symbol_>("identifier", 2);
    const symbol s_int = std::make_shared<symbol_>("int", 3);
    const symbol s_float = std::make_shared<symbol_>("float", 4);

    const lexRule r_class {s_class, "class"};
    const lexRule r_const {s_const, "const"};
    const lexRule r_identifier {s_identifier, R"([\l_]\w*)"};
    const lexRule r_int {s_int, R"(0|-?[123456789]\d*)"};
    const lexRule r_float {s_float, R"((0|-?[123456789]\d*)\.\d*|\.\d\d*)"};

    lexer lexer = lexer::lexer::compile({r_class, r_const, r_identifier, r_int, r_float});

    const std::string text = "class const 264 05 const 0.142857 3.14159 0.1 .1 meditative_onion_points_2";

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