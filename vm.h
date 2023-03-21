#include <array>
#include <vector>

// A 'byte' on the virtual computer is 16 bits
typedef unsigned short word;
const size_t word_size = sizeof(word) * 8;

// If this bit is set, do a C-instruction
#define OP_BIT          0x8000u

// These 3 bits specifies what operation to do
#define ALU_OP_BITS     0x7000u
// 3 bits = 8 opcodes
#define ALU_OP_ADD      0x1000u     // addition
#define ALU_OP_OR       0x2000u     // binary or
#define ALU_OP_XOR      0x3000u     // binary xor
#define ALU_OP_SL       0x4000u     // left shift
#define ALU_OP_SR       0x5000u     // right shift
#define ALU_OP_MUL      0x6000u     // multiply
#define ALU_OP_DIV      0x7000u     // divide

// This bit specifies whether to use A or M as the second argument (D is the first argument to operations)
#define ALU_INPUT_BIT       0x0800u     // if this bit is set, we use M instead as A for the second input

// These 5 bits specify certain ways to transform the inputs and output to allow more operations
#define ALU_ZX_BIT      0x0400u     // zero 1st input (before negation)
#define ALU_NX_BIT      0x0200u     // negate all of 1st input
#define ALU_ZY_BIT      0x0100u     // zero 2nd input (before negation)
#define ALU_NY_BIT      0x0080u     // negate all of 2nd input
#define ALU_NO_BIT      0x0040u     // negate output

// These 3 bits specify the destination (output registers) of the computation
#define ALU_DEST_BITS   0x0038u
#define ALU_DEST_D_BIT  0x0020u     // output to D register
#define ALU_DEST_A_BIT  0x0010u     // output to A register
#define ALU_DEST_M_BIT  0x0008u     // output to M register

// These 3 bits specify the jump behavior for the result of the computation
#define ALU_JMP_N      0x0004u     // jump if less than 0
#define ALU_JMP_Z      0x0002u     // jump if equal to 0
#define ALU_JMP_P      0x0001u     // jump if greater than 0

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



class ALU {
public:
    /// \param D: A reference to the D register
    /// \param A: A reference to the A register
    /// \param M: A reference to the M memory location
    /// \return Whether to jump
    bool execute_arithmetic(word instruction, word& D, word& A, word& M){ // NOLINT(readability-convert-member-functions-to-static)
        // Read inputs
        word  i1 = D
            , i2 = instruction & ALU_INPUT_BIT ? M : A;

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
            case ALU_OP_SL:
                output = i2 >= 0 ? i1 << i2 : i1 >> -i2;
                break;
            case ALU_OP_SR:
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
    // registers
    word register_D;
    word register_A;

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