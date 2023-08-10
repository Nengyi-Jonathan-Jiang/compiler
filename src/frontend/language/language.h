#pragma once

#include <string>
#include <map>
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../common/token.h"

class language {
    std::map<id_t, std::string> symbolNames;
    lexer::lexer lexer;
    parser parser;

    static language import_from_file(std::string file_name);
    void export_to_file(std::string file_name);
};