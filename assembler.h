#pragma once

#include <bitset>
#include "vm.h"

class Assembler {
public:
    enum opcodes {
        // MOV D    -> 0b1 010 0 00100
        // MOV A    -> 0b1 010 0 00100
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
        // NOT D    -> 0b1 010 0 01100 <dest> <jmp>
        // NOT A    -> 0b1 010 0 10010 <dest> <jmp>
        // NOT M    -> 0b1 010 1 10010 <dest> <jmp>
        NOT,
        // OR D x  -> 0b1 010 x 00000 <dest> <jmp>
        OR,
        // AND D x  -> 0b1 010 x 00000 <dest> <jmp>
        AND,
        XOR,
        ROL,
        ROR,
        RAL,
        RAR,
        // LOAD <ptr> -> 0b0 <ptr>
        LOAD,
        // STO <ptr>   -> 0b0 <ptr>
        STO,
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