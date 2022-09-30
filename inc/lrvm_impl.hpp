#ifndef __LRVM_IMPL_HPP__
#define __LRVM_IMPL_HPP__
// Standard Library Headers
#include <stdint.h>

#include "Decoder.hpp"
#include "Utils.hpp"

namespace LRVM {
namespace Internal {
    // To be accessed from functions; not set to "private"
    class VMCore {
    public:
        VMCore(VMCore& vm)
            : _pc(vm._pc)
            , _codeStream(vm._codeStream)
            , _memory(vm._memory)
            , _errcode(vm._errcode)
            , _errorHandler(vm._errorHandler)
        {
            for (uint8_t i = 0; i < 32; ++i)
                this->_x[i] = vm._x[i];
        }

        int32_t _x[32];
        uint32_t _pc = 0;
        uint32_t* _codeStream = nullptr;
        uint8_t* _memory = nullptr;
        uint32_t _errcode = 0;
        void (*_errorHandler)(VMCore& vm, Op_t op) = nullptr;
    };

}
} // namespace LRVM

#define LRVM_ExecutorArgsDef LRVM::Internal::VMCore &vm, LRVM::Internal::Op_t op
#include "ALU.hpp"
#include "LSU.hpp"

namespace LRVM {
namespace Internal {

    // Something like a scheduler
    class VMCPU {
    private:
        // Type definations
        using _lrvm_icb_t = void (*)(LRVM_ExecutorArgsDef);
        using _lrvm_ucb_t = void (*)(LRVM_ExecutorArgsDef);
        // Invalid instruction : 1
        // RV32I : 40
        FixedArray<_lrvm_icb_t, 41> _cb_table;
        FixedArray<VMCore*, LRVM_VMCPU_MAX_CORE_COUNT> _core;

        bool _excuteInst(VMCore& vm)
        {
            int32_t inst = vm._codeStream[vm._pc];
            Op_t op = Decoder::getOp(inst);
            // printf("Got inst at %d = %x ... OP=%i,rs1=%i,rs2=%i,rd=%i,imm=%d", _pc << 2, inst, op.cmd, op.rs1, op.rs2, op.rd, op.imm);
            //  Here we execute the instruciton!
            this->_cb_table[op.cmd](vm, op);
            return (vm._errcode != 0);
        }

    public:
        VMCPU()
        {
            _cb_table.at(Invaild_Operation) = [](LRVM_ExecutorArgsDef) { vm._errorHandler(vm, op); };
            // ALU
            _cb_table.at(ADD) = ALU::Add;
            _cb_table.at(SUB) = ALU::Sub;
            _cb_table.at(XOR) = ALU::Xor;
            _cb_table.at(OR) = ALU::Or;
            _cb_table.at(AND) = ALU::And;
            _cb_table.at(SLL) = ALU::Sll;
            _cb_table.at(SRL) = ALU::Srl;
            _cb_table.at(SRA) = ALU::Sra;
            _cb_table.at(SLT) = ALU::Slt;
            _cb_table.at(SLTU) = ALU::Sltu;
            _cb_table.at(ADDI) = ALU::Addi;
            _cb_table.at(XORI) = ALU::Xori;
            _cb_table.at(ORI) = ALU::Ori;
            _cb_table.at(ANDI) = ALU::Andi;
            _cb_table.at(SLLI) = ALU::Slli;
            _cb_table.at(SRLI) = ALU::Srli;
            _cb_table.at(SRAI) = ALU::Srai;
            _cb_table.at(SLTI) = ALU::Slti;
            _cb_table.at(SLTIU) = ALU::Sltiu;
            _cb_table.at(BEQ) = ALU::Beq;
            _cb_table.at(BNE) = ALU::Bne;
            _cb_table.at(BLT) = ALU::Blt;
            _cb_table.at(BGE) = ALU::Bge;
            _cb_table.at(BLTU) = ALU::Bgeu;
            _cb_table.at(JAL) = ALU::Jal;
            _cb_table.at(JALR) = ALU::Jalr;

            // LSU
            _cb_table.at(LB) = LSU::LB;
            _cb_table.at(LH) = LSU::LH;
            _cb_table.at(LW) = LSU::LW;
            _cb_table.at(LBU) = LSU::LBU;
            _cb_table.at(LHU) = LSU::LHU;
            _cb_table.at(SB) = LSU::SB;
            _cb_table.at(SH) = LSU::SH;
            _cb_table.at(SW) = LSU::SW;
        }
        ~VMCPU() { }

        // functions to manage VMCore
        /*
        bool addCore(VMCore* core)
        {
            if (core == nullptr)
                return false;
            this->_core.push_back(core);
            return true;
        }
        bool removeCore(VMCore* core)
        {
            if (core == nullptr)
                return false;
            for (uint8_t i = 0; i < LRVM_VMCPU_MAX_CORE_COUNT; ++i)
                if (this->_core[i] == core) {
                    this->_core.clear(i); // ToDo
                    return true;
                }
            return false;
        }
        */

        bool run()
        {
            for (auto core : _core) {
                this->_excuteInst(*core);
            }
            return true;
        }
    };

} // namespace Internal
} // namespace LRVM

#endif