#pragma once

#include <bitset>
#include "vm.h"

class Assembler {
public:
    enum opcodes {
        // MOV D    -> 0b1 010 0 00100
        MOV,
        // INC D x  -> 0b1 110 x 00000 <dest> <jmp>
        INC,
        DEC,
        // NEG D    -> 0b1 001 0 00111 <dest> <jmp>
        // NEG A    -> 0b1 001 0 11001 <dest> <jmp>
        // NEG M    -> 0b1 001 1 11001 <dest> <jmp>
        NEG,
        // ADD D x  -> 0b1 001 x 00000 <dest> <jmp>
        ADD,
        // SUB D x  -> 0b1 001 x 01001 <dest> <jmp>
        // SUB x D  -> 0b1 001 x 00011 <dest> <jmp>
        SUB,
        // MUL D x  -> 0b1 110 x 00000 <dest> <jmp>
        MUL,
        // DIV D x  -> 0b1 111 x 00000 <dest> <jmp>
        DIV,
        JMP,
        NOT,
        OR,
        AND,
        XOR,
        ROL,
        ROR,
        RAL,
        RAR,
    };

    enum Register {
        D, A, M
    };

    struct instruction {
        Register input;
        std::bitset<3> dest;
        std::bitset<3> jmp;
    };
};