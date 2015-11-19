#include "mips.h"
#include <iostream>
#include "headers/mips_cpu_instruction.h"

//const ADD_RTYPE_CODE 0x20;


mips_error mips_advance_pc(mips_cpu_h state, int32_t p) {
	state->pc = state->pcNext;
    state->pcNext = state->pcNext + p;

    return mips_Success;
}

/*! \param pData Array of 4 bytes
    \retval The big-endian equivlent
*/
uint32_t to_big(const uint8_t *pData) 
{
    return
        (((uint32_t)pData[0])<<24)
        |
        (((uint32_t)pData[1])<<16)
        |
        (((uint32_t)pData[2])<<8)
        |
        (((uint32_t)pData[3])<<0);
}

mips_error mips_execute(mips_cpu_h state, Instruction inst) {
	
	mips_error e;

	//Call R-type instructions
	if (inst.rtype.opcode == 0x00) {
		RType r = inst.rtype;

		if (r.function == MIPS_ADD_FNCODE) {
			if (r.shift == 0x00) {
				e = rtype_add(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}
		} else if (r.function == MIPS_ADDU_FNCODE) {
			if (r.shift == 0x00) {
				e = rtype_addu(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_AND_FNCODE) {
			if (r.shift == 0x00) {
				e = rtype_and(state, r.source1, r.source2, r.dest);
			} else{
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_DIV_FNCODE) {
			if (r.dest == 0x00 && r.shift == 0x00) {
				e = rtype_div(state, r.source1, r.source2);
			} else{
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_DIVU_FNCODE) {
			if (r.dest == 0x00 && r.shift == 0x00) {
				e = rtype_divu(state, r.source1, r.source2);	
			}else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_JALR_FNCODE) {
			if (r.source2 == 0x00 && r.shift == 0x00) {
				e = rtype_jalr(state, r.source1, r.dest);	
			}else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_JR_FNCODE) {
			if (r.source2 == 0x00 && r.dest == 0x00 && r.shift == 0x00) {
				e = rtype_jr(state, r.source1);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_MFHI_FNCODE) {
			if (r.source1 == 0x00 && r.source2 == 0x00 && r.shift == 0x00) {
				e = rtype_mfhi(state, r.dest);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_MFLO_FNCODE) {
			if (r.source1 == 0x00 && r.source2 == 0x00 && r.shift == 0x00) {
				e = rtype_mflo(state, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_MTHI_FNCODE) {
			if (r.dest == 0x00 && r.source2 == 0x00 && r.shift == 0x00) {
				e = rtype_mthi(state, r.source1);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_MTLO_FNCODE) {
			if (r.dest == 0x00 && r.source2 == 0x00 && r.shift == 0x00) {
				e = rtype_mtlo(state, r.source1);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_MULT_FNCODE) {
			if (r.dest == 0x00 && r.shift == 0x00) {
				e = rtype_mult(state, r.source1, r.source2);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_MULTU_FNCODE) {
			if (r.dest == 0x00 && r.shift == 0x00) {
				e = rtype_multu(state, r.source1, r.source2);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_OR_FNCODE) {
			if (r.shift == 0x00)
			{
				e = rtype_or(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SLL_FNCODE) {
			if (r.source1 == 0x00){
				e = rtype_sll(state, r.source2, r.dest, r.shift);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SLLV_FNCODE) {
			if (r.shift == 0x00)
			{
				e = rtype_sllv(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}
		} else if (r.function == MIPS_SLT_FNCODE) {
			if (r.shift == 0x00)
			{
				e = rtype_slt(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SLTU_FNCODE) {
			if (r.shift == 0x00)
			{
				e = rtype_sltu(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SRA_FNCODE) { 
			if (r.source1 == 0x00)
			{
				e = rtype_sra(state, r.source2, r.dest, r.shift);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}
		} else if (r.function == MIPS_SRAV_FNCODE) {
			if (r.shift == 0x00)
			{
				e = rtype_srav(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SRL_FNCODE) {
			if(r.source1 == 0x00) {
				e = rtype_srl(state, r.source2, r.dest, r.shift);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SRLV_FNCODE) {
			if (r.shift == 0x00) {
				e = rtype_srlv(state, r.source1, r.source2, r.dest);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SUB_FNCODE) {
			if (r.shift == 0x00) {
				e = rtype_sub(state, r.source1, r.source2, r.dest);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_SUBU_FNCODE) {
			if (r.shift == 0x00) {
				e = rtype_subu(state, r.source1, r.source2, r.dest);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (r.function == MIPS_XOR_FNCODE) {
			if (r.shift == 0x00)
			{
				e = rtype_xor(state, r.source1, r.source2, r.dest);
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else {
			e = mips_ErrorNotImplemented; 
		}

	//Call I-type instructions
	} else if (inst.rtype.opcode >= 0x04 || inst.rtype.opcode == 0x01) {
		IType i = inst.itype;

		if (i.opcode == MIPS_ADDI_OPCODE) {
			e = itype_addi(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_ADDIU_OPCODE) {
			e = itype_addiu(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_ANDI_OPCODE) {
			e = itype_andi(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_BEQ_OPCODE) {
			e = itype_beq(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_BGEZ_OPCODE && i.dest == MIPS_BGEZ_FNCODE) {
			e = itype_bgez(state, i.source, i.imm);
		} else if (i.opcode == MIPS_BGEZAL_OPCODE && i.dest == MIPS_BGEZAL_FNCODE) {
			e = itype_bgezal(state, i.source, i.imm);
		} else if (i.opcode == MIPS_BGTZ_OPCODE) {
			if (i.dest == 0x00) {
				e = itype_bgtz(state, i.source, i.imm);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (i.opcode == MIPS_BLEZ_OPCODE) {
			if (i.dest == 0x00) {
				e = itype_blez(state, i.source, i.imm);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (i.opcode == MIPS_BLTZ_OPCODE && i.dest == MIPS_BLTZ_FNCODE) {
			e = itype_bltz(state, i.source, i.imm);
		} else if (i.opcode == MIPS_BLTZAL_OPCODE && i.dest == MIPS_BLTZAL_FNCODE) {
			e = itype_bltzal(state, i.source, i.imm);
		} else if (i.opcode == MIPS_BNE_OPCODE) {
			e = itype_bne(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LB_OPCODE) {
			e = itype_lb(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LBU_OPCODE) {
			e = itype_lbu(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LH_OPCODE) {
			e = itype_lh(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LHU_OPCODE) {
			e = itype_lhu(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LW_OPCODE) {
			e = itype_lw(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LWL_OPCODE) {
			e = itype_lwl(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LWR_OPCODE) {
			e = itype_lwr(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_LUI_OPCODE) {
			if (i.source == 0x00) {
				e = itype_lui(state, i.dest, i.imm);	
			} else {
				e = mips_ExceptionInvalidInstruction;
			}			
		} else if (i.opcode == MIPS_SB_OPCODE) {
			e = itype_sb(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_SH_OPCODE) {
			e = itype_sh(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_ORI_OPCODE) {
			e = itype_ori(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_SLTI_OPCODE) {
			e = itype_slti(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_SLTIU_OPCODE) {
			e = itype_sltiu(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_SW_OPCODE) {
			e = itype_sw(state, i.source, i.dest, i.imm);
		} else if (i.opcode == MIPS_XORI_OPCODE) {
			e = itype_xori(state, i.source, i.dest, i.imm);
		} else {
			e = mips_ErrorNotImplemented;	
		}
		
	//Call J-type instructions
	} else {
		JType j = inst.jtype;
		if (j.opcode == MIPS_J_OPCODE) {
			e = jtype_jump(state, j.memory);
		} else {
			e = jtype_jal(state, j.memory);
		}

		// mips_error jtype_jump(mips_cpu_h state, uint32_t target);
		// mips_error jtype_jal(mips_cpu_h state, uint32_t target);
	}

	return e;
}


/////////////////////// RTYPE ///////////////////////

mips_error rtype_xor(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd)
{
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	mips_cpu_set_register(state, rd, va ^ vb);

	return mips_advance_pc(state, 4);
}


mips_error rtype_and(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd)
{
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	mips_cpu_set_register(state, rd, va & vb);

    return mips_advance_pc(state, 4);
}

mips_error rtype_or(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd)
{
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	mips_cpu_set_register(state, rd, va | vb);

    return mips_advance_pc(state, 4);
}

mips_error rtype_sltu(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd)
{
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

    if (va < vb) {
		mips_cpu_set_register(state, rd, 1);
	} else {
		mips_cpu_set_register(state, rd, 0);
	}
	
    return mips_advance_pc(state, 4);
}

mips_error rtype_addu(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd)
{
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

    mips_cpu_set_register(state, rd, va + vb);

	return mips_advance_pc(state, 4);
}

mips_error rtype_add(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd)
{
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	uint32_t tmp = va + vb;
    
    uint32_t a = (va >> 31) & 1;
    uint32_t b = (vb >> 31) & 1;
    uint32_t d = (tmp >> 31) & 1;

    if (a == b && a != d) {
    	return mips_ExceptionArithmeticOverflow;
    } else {
    	mips_cpu_set_register(state, rd, tmp);
    	
    	return mips_advance_pc(state, 4);;	
    }    
}

mips_error rtype_div(mips_cpu_h state, 
					uint32_t ra, uint32_t rb) {

	int32_t va = state->regs[ra];
	int32_t vb = state->regs[rb];

	if (vb == 0) {
		
		//return mips_ExceptionArithmeticOverflow;
		vb = 1;
	}

	state->LO = va / vb;
	state->HI = va % vb;
	
	return mips_advance_pc(state, 4);
}
//
mips_error rtype_divu(mips_cpu_h state, 
					uint32_t ra, uint32_t rb) {

	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	if (vb == 0) {
		vb = 1;
		//return mips_ExceptionArithmeticOverflow;
	}

	state->LO = va / vb;
	state->HI = va % vb;

	return mips_advance_pc(state, 4);
}

mips_error rtype_jalr(mips_cpu_h state, uint32_t ra, uint32_t rd) {

	if (ra == rd) {
		return mips_ErrorInvalidArgument;
	}

	mips_cpu_set_register(state, rd, state->pc + 8);

	state->pc = state->pcNext;
	state->pcNext = state->regs[ra];
	
	return mips_Success;
}

mips_error rtype_jr(mips_cpu_h state, uint32_t ra) {


	state->pc = state->pcNext;
	state->pcNext = state->regs[ra];

	return mips_Success;
}

mips_error rtype_mfhi(mips_cpu_h state, uint32_t dest) {
	mips_error e = mips_cpu_set_register(state, dest, state->HI);

	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error rtype_mflo(mips_cpu_h state, uint32_t dest) {
	mips_error e = mips_cpu_set_register(state, dest, state->LO);

	if (e != mips_Success) return e;
	
	return mips_advance_pc(state, 4);
}

mips_error rtype_mthi(mips_cpu_h state, uint32_t source) {
	mips_error e = mips_cpu_get_register(state, source, &state->HI);
	if (e != mips_Success) return e;
	
	return mips_advance_pc(state, 4);
}

mips_error rtype_mtlo(mips_cpu_h state, uint32_t source) {
	mips_error e = mips_cpu_get_register(state, source, &state->LO);
	if (e != mips_Success) return e;
	
	return mips_advance_pc(state, 4);
}

mips_error rtype_mult(mips_cpu_h state, uint32_t ra, uint32_t rb) {
	int32_t va = state->regs[ra];
	int32_t vb = state->regs[rb];
	int64_t r = int64_t(va) * int64_t(vb);

	state->HI = (r >> 32);
	state->LO = r & 0xFFFFFFFF;

	return mips_advance_pc(state, 4);
}

mips_error rtype_multu(mips_cpu_h state, uint32_t ra, uint32_t rb) {
	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];
	uint64_t r = uint64_t(va) * uint64_t(vb);

	state->HI = (r >> 32) & 0xFFFFFFFF;
	state->LO = r & 0xFFFFFFFF;

	return mips_advance_pc(state, 4);
}

mips_error rtype_sll(mips_cpu_h state, uint32_t rt, uint32_t rd,
						uint32_t sa) {

	uint32_t vt = state->regs[rt];
	mips_error e = mips_cpu_set_register(state, rd, (vt << sa));
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error rtype_sllv(mips_cpu_h state, uint32_t rs, uint32_t rt,
						uint32_t rd) {

	uint32_t vt = state->regs[rt];
	uint32_t sa = state->regs[rs];
	mips_error e;

	if (sa < 32) {
		e = mips_cpu_set_register(state, rd, (vt << sa));	
	} else {
		e = mips_cpu_set_register(state, rd, 0x00);
	}
	
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}


mips_error rtype_slt(mips_cpu_h state, uint32_t rs,
						uint32_t rt, uint32_t rd) {
	int32_t vs = state->regs[rs];
	int32_t vt = state->regs[rt];

	if (vs < vt) {
		mips_cpu_set_register(state, rd, 1);
	} else {
		mips_cpu_set_register(state, rd, 0);
	}

	return mips_advance_pc(state, 4);
}


mips_error rtype_sra(mips_cpu_h state, uint32_t rt, uint32_t rd,
						uint32_t sa) {

	uint32_t vt = state->regs[rt];

	uint32_t shifted = vt >> sa;

	if (((vt >> 31) & 0x01) == 1 && sa > 0) { //making sure we don't run into undefined behaviour
		uint32_t mask = 0xFFFFFFFF;
		mask = mask << (32 - sa);
		shifted = shifted | mask;
	}

	mips_error e = mips_cpu_set_register(state, rd, shifted);
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error rtype_srav(mips_cpu_h state, uint32_t rs, 
							uint32_t rt, uint32_t rd) {

	uint32_t vt = state->regs[rt];
	uint32_t vs = state->regs[rs];

	uint32_t shifted = vt >> vs;

	if (((vt >> 31) & 0x01) == 1 && vs > 0) { //making sure we don't run into undefined behaviour
		uint32_t mask = 0xFFFFFFFF;
		mask = mask << (32 - vs);
		shifted = shifted | mask;
	}

	mips_error e = mips_cpu_set_register(state, rd, shifted);
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error rtype_srl(mips_cpu_h state,
							uint32_t rt, uint32_t rd, uint32_t sa) {

	uint32_t vt = state->regs[rt];
	uint32_t shifted = vt >> sa;

	mips_error e = mips_cpu_set_register(state, rd, shifted);
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error rtype_srlv(mips_cpu_h state, uint32_t rs,
							uint32_t rt, uint32_t rd) {

	uint32_t vt = state->regs[rt];
	uint32_t vs = state->regs[rs];
	uint32_t shifted = vt >> vs;

	if (vs > 31) {
		shifted = 0;
	}

	mips_error e = mips_cpu_set_register(state, rd, shifted);
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error rtype_sub(mips_cpu_h state, uint32_t rs,
							uint32_t rt, uint32_t rd) {


	int32_t vs = state->regs[rs];
	int32_t vt = state->regs[rt];

	int32_t sub = vs - vt;


    if ((sub < vs) != (vt > 0)) {
    	return mips_ExceptionArithmeticOverflow;
    } else {
    	mips_cpu_set_register(state, rd, sub);
    	return mips_advance_pc(state, 4);
    }
}

mips_error rtype_subu(mips_cpu_h state, uint32_t rs,
							uint32_t rt, uint32_t rd) {

	uint32_t vt = state->regs[rt];
	uint32_t vs = state->regs[rs];

	uint32_t sub = vs - vt;

	mips_cpu_set_register(state, rd, sub);
	
	return mips_advance_pc(state, 4);
}


/////////////////////// ITYPE ///////////////////////

//throloughly test, especially how sign extension works
//step CPU
mips_error itype_addi(mips_cpu_h state,
			uint32_t source, uint32_t dest, int16_t immediate) {

	uint32_t va = state->regs[source];
	uint32_t res = va + immediate;

	uint32_t a = (va >> 31) && 1;
	uint32_t b = (uint32_t(immediate) >> 31) & 1;
    uint32_t d = (res >> 31) & 1;

    if (a == b && a != d) {
    	return mips_ExceptionArithmeticOverflow;
    } else {
    	mips_cpu_set_register(state, dest, res);
		return mips_advance_pc(state, 4);
    }    
}


//step CPU
mips_error itype_addiu(mips_cpu_h state,
			uint32_t source, uint32_t dest, int16_t immediate) {

	uint32_t va = state->regs[source];
	uint32_t res = va + immediate;

	mips_cpu_set_register(state, dest, res);
	return mips_advance_pc(state, 4);
}

mips_error itype_andi(mips_cpu_h state,
			uint32_t source, uint32_t dest, int16_t immediate) {

	uint32_t va = state->regs[source];
	uint32_t imm = immediate & 0x0000FFFF;
	uint32_t res = va & imm;

	mips_cpu_set_register(state, dest, res);
	return mips_advance_pc(state, 4);
}



mips_error itype_beq(mips_cpu_h state,
			uint32_t ra, uint32_t rb, int16_t immediate) {

	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	if (va == vb) {
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;   
}



mips_error itype_bgez(mips_cpu_h state,
			uint32_t rs, int16_t immediate) {

	int32_t vs = state->regs[rs];

	if (vs >= 0) {
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;
}

mips_error itype_bgezal(mips_cpu_h state,
			uint32_t rs, uint32_t immediate) {

	if (rs == 31) {
		return mips_ExceptionInvalidInstruction;
	}
	int32_t vs = state->regs[rs];

	
	state->regs[31] = state->pc + 8;

	if (vs >= 0) {
		mips_advance_pc(state, (immediate << 2));

	
	} else {
	
		mips_advance_pc(state, 4);
	}

	return mips_Success;
}

mips_error itype_bgtz(mips_cpu_h state,
			uint32_t rs, int16_t immediate) {

	int32_t vs = state->regs[rs];

	if (vs > 0) {
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;   
}

mips_error itype_blez(mips_cpu_h state,
			uint32_t rs, int16_t immediate) {

	int32_t vs = state->regs[rs];

	if (vs <= 0) {
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;   
}

mips_error itype_bltz(mips_cpu_h state,
			uint32_t rs, int16_t immediate) {

	int32_t vs = state->regs[rs];

	if (vs < 0) {
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;   
}

mips_error itype_bltzal(mips_cpu_h state,
			uint32_t rs, int16_t immediate) {

	int32_t vs = state->regs[rs];
	state->regs[31] = state->pc + 8;

	if (vs < 0) {	
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;   
}

mips_error itype_bne(mips_cpu_h state,
			uint32_t ra, uint32_t rb, int16_t immediate) {

	uint32_t va = state->regs[ra];
	uint32_t vb = state->regs[rb];

	if (va != vb) {
		mips_advance_pc(state, (immediate << 2));
	} else {
		mips_advance_pc(state, 4);
	}

	return mips_Success;   
}

mips_error itype_lb(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	uint32_t roundedAddress = address - (address % 4);
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, roundedAddress, 4, word);

	if (e != mips_Success) {
		return e;
	}

	int32_t loadedByte = word[address % 4];

	if ((loadedByte >> 7) & 0x01) {
		loadedByte = loadedByte | 0xFFFFFF00;
	}

	mips_cpu_set_register(state, dest, loadedByte);
	
	return mips_advance_pc(state, 4);
}

//THROUGH CHECKS NEEDED
mips_error itype_lbu(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;

	uint32_t roundedAddress = address - (address % 4);
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, roundedAddress, 4, word);

	if (e != mips_Success) {
		return e;
	}

	uint32_t loadedByte = word[address % 4];
	mips_cpu_set_register(state, dest, loadedByte);
	
	return mips_advance_pc(state, 4);
}


mips_error itype_lh(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	if (address & 0x01) return mips_ExceptionInvalidAddress;

	uint32_t roundedAddress = address - (address % 4);
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, roundedAddress, 4, word);

	if (e != mips_Success) {
		return e;
	}

	int16_t loadedByte =  (((uint32_t)word[address % 4])<<8)
				| (((uint32_t)word[(address % 4) + 1])<<0);

	mips_cpu_set_register(state, dest, loadedByte);
	
	return mips_advance_pc(state, 4);
}


mips_error itype_lhu(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	uint32_t roundedAddress = address - (address % 4);
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, roundedAddress, 4, word);

	if (e != mips_Success) {
		return e;
	}

	uint32_t loadedByte =  (((uint32_t)word[address % 4])<<8)
				| (((uint32_t)word[(address % 4) + 1])<<0);

	mips_cpu_set_register(state, dest, loadedByte);
	
	return mips_advance_pc(state, 4);
}


mips_error itype_lw(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	if ((address & 0x03) != 0x00) return mips_ExceptionInvalidAddress;

	uint8_t word[4];
	mips_error e = mips_mem_read(state->mem, address, 4, word);
	if (e != mips_Success) return e;

	uint32_t loadedByte = to_big(word);

	//int16_t loadedByte =  (((uint32_t)word[address % 4])<<8)
	//			| (((uint32_t)word[(address % 4) + 1])<<0);

	mips_cpu_set_register(state, dest, loadedByte);
	
	return mips_advance_pc(state, 4);
}

mips_error itype_lwl(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	uint32_t offset = (address % 4);
	uint32_t alignedAddress = address - offset;

	uint8_t word[4];
	mips_error e = mips_mem_read(state->mem, alignedAddress, 4, word);
	if (e != mips_Success) return e;

	uint32_t joinedWord = to_big(word);
	joinedWord = joinedWord << (offset * 0x08);

	uint32_t prevCont = 0x00;

	if (offset > 0) {
		e = mips_cpu_get_register(state, dest, &prevCont);
		if (e != mips_Success) return e;

		prevCont = prevCont & (0xFFFFFFFF >> ((4 - offset) * 0x08)); 
	}

	uint32_t finalWord = joinedWord | prevCont;
	
	e = mips_cpu_set_register(state, dest, finalWord);
	if (e != mips_Success) return e;
	
	return mips_advance_pc(state, 4);
}

mips_error itype_lwr(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	uint32_t offset = (address % 4);
	uint32_t alignedAddress = address - offset;
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, alignedAddress, 4, word);
	if (e != mips_Success) return e;

	uint32_t joinedWord = to_big(word);
	uint32_t prevCont = 0x00;

	joinedWord = joinedWord >> ((3 - offset) * 0x08);

	if (offset < 3) {		
	
		e = mips_cpu_get_register(state, dest, &prevCont);
		if (e != mips_Success) return e;

		uint32_t mask = 0xFFFFFFFF << ((offset + 1) * 0x08);
		prevCont = prevCont & mask;

		//prevCont = prevCont & (0xFFFFFFFF << ((3 - offset) * 0x08)); 
	}

	uint32_t finalWord = joinedWord | prevCont;
	
	e = mips_cpu_set_register(state, dest, finalWord);
	if (e != mips_Success) return e;
		
	
	return mips_advance_pc(state, 4);	
}


mips_error itype_lui(mips_cpu_h state, uint32_t rt, uint32_t immediate) {

	uint32_t ext = (immediate << 16) & 0xFFFF0000;
	mips_cpu_set_register(state, rt, ext);

	return mips_advance_pc(state, 4);
}


mips_error itype_ori(mips_cpu_h state, uint32_t src,
					uint32_t dest, uint32_t immediate) {

	uint32_t va;
	mips_error e = mips_cpu_get_register(state, src, &va);
	if (e != mips_Success) return e;

	immediate = immediate & 0xFFFF;

	uint32_t result = immediate | va;
	mips_cpu_set_register(state, dest, result);

	return mips_advance_pc(state, 4);
}

mips_error itype_sb(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	uint32_t roundedAddress = address - (address % 4);
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, roundedAddress, 4, word);
	if (e != mips_Success) return e;

	word[address % 4] = state->regs[dest]; // >> 0x18;

	e = mips_mem_write(state->mem, roundedAddress, 4, word);
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}

mips_error itype_sh(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	if (address & 0x01) return mips_ExceptionInvalidAddress;

	uint32_t roundedAddress = address - (address % 4);
	uint8_t word[4];

	mips_error e = mips_mem_read(state->mem, roundedAddress, 4, word);
	if (e != mips_Success)return e;

	word[address % 4] = state->regs[dest] >> 0x08;
	word[(address % 4) + 1] = (state->regs[dest]);

	e = mips_mem_write(state->mem, roundedAddress, 4, word);
	if (e != mips_Success) return e;

	return mips_advance_pc(state, 4);
}



mips_error itype_slti(mips_cpu_h state, uint32_t rs,
						uint32_t rd, int32_t immediate) {
	int32_t vs = state->regs[rs];

	if (vs < immediate) {
		mips_cpu_set_register(state, rd, 1);
	} else {
		mips_cpu_set_register(state, rd, 0);
	}

	
	return mips_advance_pc(state, 4);

}

mips_error itype_sltiu(mips_cpu_h state, uint32_t rs,
						uint32_t rd, uint32_t immediate) {
	uint32_t vs = state->regs[rs];
	
	if ((immediate >> 15) & 0x01) {
		immediate = immediate | 0xFFFF0000;
	}


	if (vs < immediate) {
		mips_cpu_set_register(state, rd, 1);
	} else {
		mips_cpu_set_register(state, rd, 0);
	}

	return mips_advance_pc(state, 4);

}

mips_error itype_sw(mips_cpu_h state, uint32_t src,
						uint32_t dest, int16_t immediate) {

	uint32_t address = state->regs[src] + immediate;
	if ((address & 0x03) != 0x00) return mips_ExceptionInvalidAddress;

	uint32_t w= state->regs[dest];
	uint8_t word[4];
	word[0] = (w >> 24) & 0xff;
	word[1] = (w >> 16) & 0xff;
	word[2] = (w >> 8) & 0xff;
	word[3] = (w >> 0) & 0xff;

	mips_error e = mips_mem_write(state->mem, address, 4, word);
	if (e != mips_Success) return e;
	
	return mips_advance_pc(state, 4);
}


mips_error itype_xori(mips_cpu_h state, uint32_t rs,
						uint32_t rt, uint32_t immediate) {
	uint32_t vs = state->regs[rs];
	uint32_t res = vs ^ immediate;

	mips_cpu_set_register(state, rt, res);
	
    return mips_advance_pc(state, 4);
}

/////////////////////// JTYPE ///////////////////////
mips_error jtype_jump(mips_cpu_h state, uint32_t target) {
	state->pc = state->pcNext;
	state->pcNext = (state->pc & 0xf0000000) | (target << 2);
	return mips_Success;
}

mips_error jtype_jal(mips_cpu_h state, uint32_t target) {
	state->regs[31] = state->pc + 8;
	state->pc = state->pcNext;
	state->pcNext = (state->pc & 0xf0000000) | (target << 2);

	return mips_Success;
}

