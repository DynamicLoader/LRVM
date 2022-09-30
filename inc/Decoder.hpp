#ifndef __LRVM_DECODER_HPP__
#define __LRVM_DECODER_HPP__

#include "../lrvm.h"

namespace LRVM {

namespace Internal {

    typedef enum {
        Invaild_Operation,
        // RV32I Base Integer Instructions
        ADD,
        SUB,
        XOR,
        OR,
        AND,
        SLL,
        SRL,
        SRA,
        SLT,
        SLTU,
        ADDI,
        XORI,
        ORI,
        ANDI,
        SLLI,
        SRLI,
        SRAI,
        SLTI,
        SLTIU,
        LB,
        LH,
        LW,
        LBU,
        LHU,
        SB,
        SH,
        SW,
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,
        JAL,
        JALR,
        LUI,
        AUIPC,
        FENCE, // To be implement later...
        ECALL,
        EBREAK,
    } _operation_t;

    typedef enum {
        R,
        I,
        S,
        B,
        U,
        J
    } _cmd_t;

    typedef struct {
        _operation_t cmd;
        uint8_t rd;
        uint8_t rs1, rs2;
        int32_t imm;
    } Op_t;

    namespace Decoder {
        // funct3 table
        _operation_t __operation_irr_t[8] = { ADD, SLL, SLT, SLTU, XOR, SRL, OR, AND };
        _operation_t __operation_iir_t[8] = { ADDI, SLLI, SLTI, SLTIU, XORI, SRLI, ORI, ANDI };
        _operation_t __operation_il_t[8] = { LB, LH, LW, Invaild_Operation, LBU, LHU, Invaild_Operation, Invaild_Operation };
        _operation_t __operation_is_t[8] = { SB, SH, SW, Invaild_Operation, Invaild_Operation, Invaild_Operation, Invaild_Operation, Invaild_Operation };
        _operation_t __operation_ib_t[8] = { BEQ, BNE, Invaild_Operation, Invaild_Operation, BLT, BGE, BLTU, BGEU };

        void _getParam(_cmd_t type, Op_t& op, int32_t cmd)
        {
            int8_t _rd = (cmd & 0xF80) >> 7;
            int8_t _rs1 = (cmd & 0xF8000) >> 15;
            int8_t _rs2 = (cmd & 0x1F00000) >> 20;
            switch (type) {
            case R:
                op.rd = _rd;
                op.rs1 = _rs1;
                op.rs2 = _rs2;
                break;
            case I:
                op.rd = _rd;
                op.rs1 = _rs1;
                op.imm = (cmd & 0x80000000 ? 0xFFFFF800 : 0) | (cmd & 0xFFF00000) >> 20;
                break;
            case S:
                op.rs1 = _rs1;
                op.rs2 = _rs2;
                op.imm = (cmd & 0x80000000 ? 0xFFFFF800 : 0) | ((cmd & 0xFE000000) >> 20) | _rd;
                break;
            case B:
                op.rs1 = _rs1;
                op.rs2 = _rs2;
                {
                    int8_t bit7 = (cmd >> 7) & 0b1;
                    op.imm = bit7 | (cmd & 0x80000000 ? 0xFFFFF000 : 0) | ((cmd & 0xFE000000) >> 20) | _rd;
                }
                break;
            case U:
                op.rd = _rd;
                op.imm = cmd & 0xFFFFF000;
                break;
            case J:
                op.rd = _rd;
                {
                    int32_t imm19_12 = cmd & 0xFF000;
                    int16_t imm11 = (cmd & 0x100000) >> 9;
                    int16_t imm10_1 = (cmd & 0x7FE00000) >> 20;
                    int32_t imm20 = (cmd & 0x80000000) >> 11;
                    op.imm = imm11 | imm20 | imm19_12 | imm10_1 | (cmd & 0x80000000 ? 0xFFF00000 : 0);
                }
                break;
            default:
                break;
            }
        }

        Op_t getOp(int32_t cmd)
        {
            Op_t ret;
            uint8_t tmp = (cmd & 0b01111111);
            uint8_t funct3 = (cmd >> 12) & 0b0111;
            switch (tmp) {
            case 0b0110011: // Basic R-R operation (R-type)
            {
                uint8_t funct7 = (cmd >> 25) & 0b1111111;
                if (funct7 == 0x20)
                    ret.cmd = (funct3 == 0x0 ? SUB : SRA);
                if (funct7 != 0x00) {
                    ret.cmd = Invaild_Operation;
                    return ret;
                }
            }
                ret.cmd = __operation_irr_t[funct3];
                _getParam(R, ret, cmd);
                break;
            case 0b0010011: // Basic R-I operation (I-type)
            {
                uint8_t funct7 = (cmd >> 25) & 0b1111111;
                if ((funct7 == 0x20) && (funct3 == 0x5)) {
                    ret.cmd = SRAI;
                    return ret;
                }
                ret.cmd = __operation_iir_t[funct3];
                if (((ret.cmd == SLLI) || (ret.cmd == SRLI)) && (funct7 != 0x00))
                    ret.cmd = Invaild_Operation;
            }
                _getParam(I, ret, cmd);
                break;
            case 0b0000011: // Load (I-type)
                ret.cmd = __operation_il_t[funct3];
                _getParam(I, ret, cmd);
                break;
            case 0b0100011: // Store (S-type)
                ret.cmd = __operation_is_t[funct3];
                _getParam(S, ret, cmd);
                break;
            case 0b1100011: // Branch (B-type)
                ret.cmd = __operation_ib_t[funct3];
                _getParam(B, ret, cmd);
                break;
            case 0b1101111: // JAL (J-type)
                ret.cmd = JAL;
                _getParam(J, ret, cmd);
                break;
            case 0b1100111: // JALR (I-type)
                ret.cmd = (funct3 ? Invaild_Operation : JALR);
                _getParam(I, ret, cmd);
                break;
            case 0b110111: // LUI (U-type)
                ret.cmd = LUI;
                _getParam(U, ret, cmd);
                break;
            case 0b0010111: // AUIPC (U-type)
                ret.cmd = AUIPC;
                _getParam(U, ret, cmd);
                break;
            case 0b0001111: // FENCE (like I-type)
                ret.cmd = (funct3 == 0x0 ? FENCE : Invaild_Operation);
                _getParam(I, ret, cmd);
                break;
            case 0b1110011: // Ecall and Ebreak (I-type)
            {
                uint16_t imm = (cmd >> 20);
                if (imm == 0x0) {
                    ret.cmd = ECALL;
                } else if (imm == 0x1) {
                    ret.cmd = EBREAK;
                } else {
                    ret.cmd = Invaild_Operation;
                    return ret;
                }
            }
                _getParam(I, ret, cmd);
                break;
            default:
                ret.cmd = Invaild_Operation;
                return ret;
            }
            return ret;
        }

    } // namespace Decoder

} // namespace Internal

}

#endif