#include <array>
#include <vector>

typedef unsigned short word;
const size_t word_size = sizeof(word) * 8;

#define INSTRUCTION_TYPE_ALU_BIT 0x8000u

// These bits show the type of ALU instruction
#define ALU_OP_BITS              0x7800u
// All ALU instructions will be constant time complexity with respect to the values (but not word length)
#define ALU_A_BIT                0x4000u // addition
#define ALU_S_BIT                0x2000u // shift left / right (left default)
#define ALU_M_BIT                0x1000u // multiply / divide (multiply default)
#define ALU_R_BIT                0x0800u // choose inverse operation (no effect on addition)
// These bits specify certain ways to transform the inputs and output to allow more operations
#define ALU_Z1_BIT               0x0400u // whether to zero 1st input (before negation)
#define ALU_N1_BIT               0x0200u // whether to negate all of 1st input
#define ALU_Z2_BIT               0x0100u // whether to zero 2nd input (before negation)
#define ALU_N2_BIT               0x0080u // whether to negate all of 2nd input
#define ALU_NO_BIT               0x0040u // whether to negate output
// Last 6 bits not used in ALU instructions

// or  :  To do i1 | i2, don't set any bits!        (bitwise or)
// and :  To do i1 & i2, set N1, N2, NO bits        (bitwise and)
// not :  To do ~i1, set N1, Z2 bits                (bitwise not)

// ls  :  To do i1 << i2, set S bit                 (left shift)
// rs  :  To do i1 >> i2, set S, R bits             (right shift)
// l1  :  To do i1 << 1, set Z2, N2, S, R bits      (fast multiply by 2)
// r1  :  To do i1 >> 1, set Z2, N2, S bits         (fast divide by 2)

// add :  To do i1 + i2, set A bit                  (addition)
// sub :  To do i1 - i2, set N1, NO, A bits         (subtraction)
// inc :  To do i1 + 1, set N1, Z2, N2, NO, A bits  (fast increment)
// dec :  To do i1 - 1, set Z2, N2, A bits          (fast decrement)

// mul :  To do i1 * i2, set M bit                  (multiplication)
// div :  To do i1 / i2, set M, R bits              (division)

class RAM {
    std::vector<word> memory;
    word& operator[](int i){
        if(i >= memory.size()) memory.resize(i);
        return memory[i];
    }
};

class ALU {

    word execute_arithmetic(word instruction, word i1, word i2){
        // transform inputs
        if(instruction & ALU_Z1_BIT) i1 = 0;
        if(instruction & ALU_Z2_BIT) i2 = 0;
        if(instruction & ALU_N1_BIT) i1 = ~i1;
        if(instruction & ALU_N2_BIT) i2 = ~i2;

        // execute instruction
        word output;
        switch(instruction & ALU_OP_BITS){
            case 0: // No bits, do bitwise OR
                output = i1 | i2;
                break;
            case ALU_A_BIT:
                output = i1 + i2;
                break;
            case ALU_S_BIT:
                output = i2 > 0 ? i1 << i2 : i1 >> -i2;
                break;
            case ALU_S_BIT | ALU_R_BIT:
                output = i2 > 0 ? i1 >> i2 : i1 << -i2;
                break;
            case ALU_M_BIT:
                output = i1 * i2;
                break;
            case ALU_M_BIT | ALU_R_BIT:
                output = i1 / i2;
                break;
            default:
                output = 0;
        }

        // transform output
        if(instruction | ALU_NO_BIT) output = ~output;

        return output;
    }
};

class CPU {
    // special registers
    word register_M;
    word register_D;
    word register_A;

    std::array<word, 8> registers;

    word stack_ptr;
    word instruction_ptr;

    // Components
    RAM ram;
    ALU alu;

    void execute (word instruction) {
        // first bit indicates whether is alu command or mem command
        if(instruction & INSTRUCTION_TYPE_ALU_BIT) {
            // This is an ALU command

        }
        // otherwise
        else {
            // do stuff
        }
    }
};

class Program {

};