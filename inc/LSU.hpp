#ifndef __LRVM_LSU_HPP__
#define __LRVM_LSU_HPP__

#include "../lrvm.h"

namespace LRVM {
namespace Internal {

// We got RISC-V as a little-endian machine,so there will be some translation during load and store...
#ifndef __BYTE_ORDER__
#warning Your compiler has not defined macro __BYTE_ORDER__ ! The VM will probably not working properly!
#warning Treating as little-endian.
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Not need to convert;define empty functions.
    constexpr uint32_t swap32(uint32_t x) { return x; }
    constexpr uint16_t swap16(uint16_t x) { return x; }
#else
#define swap32(x) __builtin_bswap32(x);
#define swap16(x) __buillin_bswap16(x);
#endif

    namespace LSU {

        void LB(LRVM_ExecutorArgsDef)
        {
            int32_t addr = (vm._x[op.rs1] + op.imm);
            vm._x[op.rd] = (int8_t)vm._memory[addr];
            vm._pc++;
        }
        void LH(LRVM_ExecutorArgsDef)
        {
            int32_t addr = (vm._x[op.rs1] + op.imm);
            if (addr & 0b1) {
                vm._errorHandler(vm, op);
                return;
            }
            uint16_t* tmp = (uint16_t*)(vm._memory + addr);
            vm._x[op.rd] = (int16_t)swap16(*tmp);
            vm._pc++;
        }
        void LW(LRVM_ExecutorArgsDef)
        {
            int32_t addr = (vm._x[op.rs1] + op.imm);
            if (addr & 0b11) {
                vm._errorHandler(vm, op);
                return;
            }
            uint32_t* tmp = (uint32_t*)(vm._memory + addr);
            vm._x[op.rd] = (int32_t)swap32(*tmp);
            vm._pc++;
        }
        void LBU(LRVM_ExecutorArgsDef)
        {
            int32_t addr = (vm._x[op.rs1] + op.imm);
            vm._x[op.rd] = vm._memory[addr];
            vm._pc++;
        }
        void LHU(LRVM_ExecutorArgsDef)
        {
            int32_t addr = (vm._x[op.rs1] + op.imm);
            if (addr & 0b1) {
                vm._errorHandler(vm, op);
                return;
            }
            uint16_t* tmp = (uint16_t*)(vm._memory + addr);
            vm._x[op.rd] = swap16(*tmp);
            vm._pc++;
        }

        void SB(LRVM_ExecutorArgsDef)
        {
            int32_t addr = vm._x[op.rs1] + op.imm;
            vm._memory[addr] = vm._x[op.rs2] & 0xFF;
            vm._pc++;
        }
        void SH(LRVM_ExecutorArgsDef)
        {
            int32_t addr = vm._x[op.rs1] + op.imm;
            if (addr & 0b1) {
                vm._errorHandler(vm, op);
                return;
            }
            uint16_t* rd = (uint16_t*)(vm._memory + addr);
            *rd = swap16(vm._x[op.rs2] & 0xFFFF);
            vm._pc++;
        }
        void SW(LRVM_ExecutorArgsDef)
        {
            int32_t addr = vm._x[op.rs1] + op.imm;
            if (addr & 0b1) {
                vm._errorHandler(vm, op);
                return;
            }
            uint32_t* rd = (uint32_t*)(vm._memory + addr);
            *rd = swap32(vm._x[op.rs2]);
            vm._pc++;
        }

    } // namespace LSU

}

}

#endif