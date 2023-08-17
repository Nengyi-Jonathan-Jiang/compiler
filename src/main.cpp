#include <iostream>

#include "frontend/lexer/lexer.h"
#include "frontend/parser/grammar/GrammarBuilder.h"
#include "frontend/parser/grammar/Grammar.h"

void test_parser() {
    using lexer::lexRule, lexer::lexer;

    const symbol s_lbrace = std::make_shared<symbol_>("{", 0);
    const symbol s_rbrace = std::make_shared<symbol_>("}", 1);
    const symbol s_class = std::make_shared<symbol_>("class", 2);
    const symbol s_const = std::make_shared<symbol_>("const", 3);
    const symbol s_identifier = std::make_shared<symbol_>("identifier", 4);
    const symbol s_int = std::make_shared<symbol_>("int", 5);
    const symbol s_float = std::make_shared<symbol_>("float", 6);

    const lexRule r_class {s_class, "1"};
    const lexRule r_const {s_const, "2"};
    const lexRule r_identifier {s_identifier, R"([\l_]\w*)"};
    const lexRule r_int {s_int, R"(0|-?[123456789]\d*)"};
    const lexRule r_float {s_float, R"((0|-?[123456789]\d*)\.\d*|\.\d\d*)"};

    lexer lexer = lexer::lexer::compile({r_class, r_const, r_identifier, r_int, r_float});

    const std::string text = "class onion { axe 6 f 34 f152 -23 }";

    const std::vector<token> tokens = lexer.lex_all(text);
    for(const token& i : tokens){
        std::cout << i.to_string() << "\n";
    }

    std::cout << "Successful lex.\n";

    const symbol s_var_decl = std::make_shared<symbol_>("var", 7);
    const symbol s_class_decl = std::make_shared<symbol_>("class", 7);
    const symbol s_var_decls = std::make_shared<symbol_>("vars", 7);

    const parseRule r_var_decl1 = std::make_shared<parseRule_>(s_var_decl, symbolString{s_identifier, s_int});
    const parseRule r_var_decl2 = std::make_shared<parseRule_>(s_var_decl, symbolString{s_const, s_identifier, s_int});
    const parseRule r_var_decls1 = std::make_shared<parseRule_>(s_var_decls, symbolString{});
    const parseRule r_var_decls2 = std::make_shared<parseRule_>(s_var_decls, symbolString{s_var_decl, s_var_decls});
    const parseRule r_class_decl = std::make_shared<parseRule_>(s_class_decl, symbolString{s_class, s_identifier, s_lbrace, s_var_decls, s_rbrace});

    std::cout << "Compiling grammar.\n";

    const Grammar grammar = GrammarBuilder::compile({
        r_var_decl1,
        r_var_decl2,
        r_var_decls1,
        r_var_decls2,
        r_class_decl
    }, s_class_decl);

    grammar.print_info();
}

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
    test_parser();
    return 0;
}