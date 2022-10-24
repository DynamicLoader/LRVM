#ifndef __LRVM_ALU_HPP__
#define __LRVM_ALU_HPP__

#include "../lrvm.h"

namespace LRVM {
namespace Internal {

namespace ALU
{
    
    void Add(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] + vm._x[op.rs2];
        vm._pc++;
    }
    void Sub(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] - vm._x[op.rs2];
        vm._pc++;
    }
    void Xor(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] ^ vm._x[op.rs2];
        vm._pc++;
    }
    void Or(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] | vm._x[op.rs2];
        vm._pc++;
    }
    void And(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] & vm._x[op.rs2];
        vm._pc++;
    }
    void Sll(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (uint32_t)vm._x[op.rs1] << (uint32_t)vm._x[op.rs2];
        vm._pc++;
    }
    void Srl(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (uint32_t)vm._x[op.rs1] >> (uint32_t)vm._x[op.rs2];
        vm._pc++;
    }
    void Sra(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] >> vm._x[op.rs2];
        vm._pc++;
    }
    void Slt(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (vm._x[op.rs1] < vm._x[op.rs2] ? 1 : 0);
        vm._pc++;
    }
    void Sltu(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = ((uint32_t)vm._x[op.rs1] < (uint32_t)vm._x[op.rs2] ? 1 : 0);
        vm._pc++;
    }

    void Addi(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] + op.imm;
        vm._pc++;
    }
    void Xori(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] ^ op.imm;
        vm._pc++;
    }
    void Ori(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] | op.imm;
        vm._pc++;
    }
    void Andi(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] & op.imm;
        vm._pc++;
    }
    void Slli(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (uint32_t)vm._x[op.rs1] << (uint32_t)(op.imm & 0b11111);
        vm._pc++;
    }
    void Srli(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (uint32_t)vm._x[op.rs1] >> (uint32_t)(op.imm & 0b11111);
        vm._pc++;
    }
    void Srai(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = vm._x[op.rs1] >> (op.imm & 0b11111);
        vm._pc++;
    }
    void Slti(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (vm._x[op.rs1] < op.imm ? 1 : 0);
        vm._pc++;
    }
    void Sltiu(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = ((uint32_t)vm._x[op.rs1] < (uint32_t)op.imm ? 1 : 0);
        vm._pc++;
    }

    void Lui(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = op.imm;
        vm._pc++;
    }

    void Auipc(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (vm._pc << 2) + (op.imm);
        vm._pc++;
    }

    void Beq(LRVM_ExecutorArgsDef) { vm._pc += (vm._x[op.rs1] == vm._x[op.rs2] ? op.imm : 1); }
    void Bne(LRVM_ExecutorArgsDef) { vm._pc += (vm._x[op.rs1] != vm._x[op.rs2] ? op.imm : 1); }
    void Blt(LRVM_ExecutorArgsDef) { vm._pc += (vm._x[op.rs1] < vm._x[op.rs2] ? op.imm : 1); }
    void Bge(LRVM_ExecutorArgsDef) { vm._pc += (vm._x[op.rs1] >= vm._x[op.rs2] ? op.imm : 1); }
    void Bltu(LRVM_ExecutorArgsDef) { vm._pc += ((uint32_t)vm._x[op.rs1] < (uint32_t)vm._x[op.rs2] ? op.imm : 1); }
    void Bgeu(LRVM_ExecutorArgsDef) { vm._pc += ((uint32_t)vm._x[op.rs1] >= (uint32_t)vm._x[op.rs2] ? op.imm : 1); }

    void Jal(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (vm._pc + 1) << 2; // Code stream is defined as an uint32_t*;we prepare the rd as normal offset of bytes
        if (op.imm & 0b11) {
           vm._errorHandler(vm, op);
        }
        vm._pc += op.imm >> 2;
    }

    void Jalr(LRVM_ExecutorArgsDef)
    {
        vm._x[op.rd] = (vm._pc + 1) << 2;
        uint32_t tmp = vm._x[op.rs1] + op.imm;
        if (tmp & 0b11) {
            vm._errorHandler(vm, op);
        }
        vm._pc = tmp >> 2;
    }
    
} // namespace ALU

} // namespace Internal

}

#endif