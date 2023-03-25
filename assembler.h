#pragma once

#include <bitset>
#include <string>
#include <sstream>
#include <map>
#include "vm.h"

class Assembler {
public:
    ROM assemble(const std::string& text) {
        ROM res;
        std::istringstream in(text);

        std::map<std::string, word> labels;

        int instruction = 0;
        while(!in.eof()){
            std::string command; in >> command;

            if(command == "LABEL") {
                std::string s; in >> s;
                labels[s] = instruction;
            }
            if(command == "GOTO") {

            }
            else if(command == "CONST") {
                word value; in >> value;
                int reg; in >> reg;
                if(in.peek() == ','){

                }
            }
            else if(command == "MOV") {

            }
            else if(command == "ADD") {

            }
            else if(command == "NUL") {

            }
        }

        return res;
    }
};