#pragma once

#include <array>
#include <vector>

// A 'byte' on the virtual computer is 32 bits
typedef unsigned short word;
const size_t word_size = sizeof(word) * 8;  // Words
const size_t instruction_size = 2;  // Instructions are 2 words in size

// First bit is not used

// These 3 bits specifies what operation to do
#define ALU_OP_BITS     0x7000u
// 3 bits = 8 opcodes
#define OP_C_STORE      0x0000u     // special - we store the next word as a constant
#define ALU_OP_ADD      0x1000u     // addition
#define ALU_OP_OR       0x2000u     // binary or
#define ALU_OP_XOR      0x3000u     // binary or
#define ALU_OP_SHL      0x4000u     // left shift
#define ALU_OP_SHR      0x5000u     // right shift
#define ALU_OP_MUL      0x6000u     // multiply
#define ALU_OP_DIV      0x7000u     // divide

// These four bits specify the destination of the operation
#define ALU_D_MEM_BIT   0x0800u     // This bit specifies whether to treat the dest register as a memory location
#define ALU_D_BITS      0x0700u     // These 3 bits specify the destination (output register) of the computation

// These 5 bits specify certain ways to transform the inputs and output to allow more operations.
// By convention, the store instruction should not set any of them
#define ALU_ZX_BIT      0x0080u     // zero 1st input (before negation)
#define ALU_NX_BIT      0x0040u     // negate all of 1st input
#define ALU_ZY_BIT      0x0020u     // zero 2nd input (before negation)
#define ALU_NY_BIT      0x0010u     // negate all of 2nd input
#define ALU_NO_BIT      0x0008u     // negate output

// These 3 bits specify the jump behavior for the result of the computation
// By convention, if store instruction wish to jump, all bits should be set
#define ALU_JMP_N      0x0004u     // jump if less than 0
#define ALU_JMP_Z      0x0002u     // jump if equal to 0
#define ALU_JMP_P      0x0001u     // jump if greater than 0

// These
// Note: only one input can be treated as a memory location at a time because RAM can only be accessed once per cycle
#define ALU_X_MEM_BIT       0x8000u     // This bit specifies whether to treat 1st input as a memory location
#define ALU_X_BITS          0x7000u     // These 3 bits specify 1st input register of the computation
#define ALU_Y_MEM_BIT       0x0800u     // This bit specifies whether to treat 2nd input as a memory location
#define ALU_Y_BITS          0x0700u     // These 3 bits specify 1nd input register of the computation

// Sample operations

// Using OR op

// or  :  To do i1 | i2, no extra bits to set       (bitwise or)
// and :  To do i1 & i2, set N1, N2, NO bits        (bitwise and)
// not :  To do ~i1, set N1, Z2 bits                (bitwise not)

// Using

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

struct RAM {
    word& operator[](int i){
        if(i >= memory.size()) memory.resize(i);
        return memory[i];
    }

    private: std::vector<word> memory;
};

struct ROM {
    static void write(word instruction, ROM& rom) {
        rom.memory.push_back(instruction);
    }

    static void clear_memory(ROM& rom) {
        rom.memory.clear();
    }

    word& operator[](int i){
        if(i >= memory.size()) return garbage;
        return memory[i];
    }

    bool isGarbage(word& ptr){
        return &ptr == &garbage;
    }

private:
    std::vector<word> memory;
    static word garbage;
};



/*
 * HDL Specs:
 *
 */
class ALU {
public:
    /// \param D: A reference to the D register
    /// \param A: A reference to the A register
    /// \param M: A reference to the M memory location
    /// \return Whether to jump
    bool execute_arithmetic(word instruction, word& D, word& A, word& M){ // NOLINT(readability-convert-member-functions-to-static)
        // Read inputs
        word  i1 = D
            , i2 = instruction & ALU_INPUT_BITS ? M : A;

        // transform inputs
        if(instruction & ALU_ZX_BIT) i1 = 0;
        if(instruction & ALU_ZY_BIT) i2 = 0;
        if(instruction & ALU_NX_BIT) i1 = ~i1;
        if(instruction & ALU_NY_BIT) i2 = ~i2;

        // execute instruction
        word output;
        switch(instruction & ALU_OP_BITS){
            case ALU_OP_ADD:
                output = i1 + i2;
                break;
            case ALU_OP_OR:
                output = i1 | i2;
                break;
            case ALU_OP_XOR:
                output = i1 ^ i2;
                break;
            case ALU_OP_SHL:
                output = i2 >= 0 ? i1 << i2 : i1 >> -i2;
                break;
            case ALU_OP_SHR:
                output = i2 >= 0 ? i1 >> i2 : i1 << -i2;
                break;
            case ALU_OP_MUL:
                output = i1 * i2;
                break;
            case ALU_OP_DIV:
                output = i1 / i2;
                break;
            default:
                output = 0;
        }

        // transform output
        if(instruction & ALU_NO_BIT) output = ~output;

        // write to destination
        if(instruction & ALU_DEST_D_BIT) D = output;
        if(instruction & ALU_DEST_A_BIT) A = output;
        if(instruction & ALU_DEST_M_BIT) M = output;

        // Jump
        return output > 0 && (instruction & ALU_JMP_P)
            || output < 0 && (instruction & ALU_JMP_N)
            || output == 0 && (instruction & ALU_JMP_Z);
    }
};

class CPU {
    word registers[8];

    // special registers
    word instruction_ptr;

    // Components
    RAM ram;
    ALU alu;

    ROM program_memory;

    void execute () {
        // fetch the current instruction from program memory
        word instruction = program_memory[instruction_ptr];

        // next instruction pointer. If no jump, this is just the current instruction pointer + 1
        word next_instruction_ptr = instruction_ptr + 1;

        // first bit of instruction indicates whether is alu command or mem command
        if(instruction & OP_BIT) {
            // This is an ALU command

            // M is at RAM[A]
            word& memory_M = ram[register_A];

            // execute the instruction
            bool do_jump = alu.execute_arithmetic(instruction, register_D, register_A, memory_M);

            // jump if needed
            if(do_jump) next_instruction_ptr = register_A;
        }
        // otherwise
        else {
            // put the remaining bits in the A register
            // don't need to bit mask because first bit is zero anyways
            register_A = instruction;
        }

        // update instruction pointer
        instruction_ptr = next_instruction_ptr;
    }
};