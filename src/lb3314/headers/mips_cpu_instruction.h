#ifndef mips_cpu_instruction
#define mips_cpu_instruction

struct mips_cpu_impl
{
    uint32_t pc;
    uint32_t pcNext;
    
    uint32_t regs[32];

    uint32_t LO, HI;
    
    mips_mem_h mem;
    
    unsigned logLevel;
    FILE *logDst;
};

struct RType
{
	uint32_t function	:	6;
	uint32_t shift		:	5;
	uint32_t dest		:	5;
	uint32_t source2	:	5;
	uint32_t source1	:	5;
	uint32_t opcode		:	6;
};	//= 32 bits

struct IType
{
	uint32_t imm		:	16;
	uint32_t dest		:	5;
	uint32_t source		:	5;
	uint32_t opcode		:	6;
};	//= 32 bits

struct JType
{
	uint32_t memory		:	26;
	uint32_t opcode		:	6;
};	//= 32 bits

union Instruction
{
	RType rtype;
	IType itype;
	JType jtype;
	uint32_t unparsed;
};


typedef enum _mips_instruction_codes {
	MIPS_ADD_FNCODE = 		 0x20,
	MIPS_ADDI_OPCODE = 	0x08,
	MIPS_ADDIU_OPCODE = 0x09,
	MIPS_ADDU_FNCODE = 		 0x21,
	MIPS_AND_FNCODE = 		 0x24,
	MIPS_ANDI_OPCODE =  0x0C,
	MIPS_BEQ_OPCODE =   0x04,
	MIPS_BGEZ_OPCODE =  0x01,
	MIPS_BGEZ_FNCODE = 		 0x01,
	MIPS_BGEZAL_OPCODE= 0x01,
	MIPS_BGEZAL_FNCODE = 	 0x11,
	MIPS_BGTZ_OPCODE =  0x07,
	MIPS_BLEZ_OPCODE = 	0x06,
	MIPS_BLTZ_OPCODE =  0x01,
	MIPS_BLTZ_FNCODE =  	 0x00,
	MIPS_BLTZAL_OPCODE= 0x01,
	MIPS_BLTZAL_FNCODE = 	 0x10,
	MIPS_BNE_OPCODE =	0x05,
	MIPS_DIV_FNCODE =		 0x1A,
	MIPS_DIVU_FNCODE =		 0x1B,
	MIPS_J_OPCODE = 	0x02,
	MIPS_JAL_OPCODE = 	0x03,
	MIPS_JALR_FNCODE =		 0x09,
	MIPS_JR_FNCODE =		 0x08,
	MIPS_LB_OPCODE =	0x20,
	MIPS_LBU_OPCODE =	0x24,
	MIPS_LH_OPCODE =	0x21,
	MIPS_LHU_OPCODE =	0x25,
	MIPS_LUI_OPCODE =	0x0F,
	MIPS_LW_OPCODE =	0x23,
	MIPS_LWL_OPCODE =	0x22,
	MIPS_LWR_OPCODE =	0x26,
	MIPS_MFHI_FNCODE =		 0x10, //same as add?
	MIPS_MFLO_FNCODE =		 0x12,
	MIPS_MTHI_FNCODE = 		 0x11,
	MIPS_MTLO_FNCODE =		 0x13,
	MIPS_MULT_FNCODE = 		 0x18,
	MIPS_MULTU_FNCODE = 	 0x19,
	MIPS_OR_FNCODE = 		 0x25,
	MIPS_ORI_OPCODE = 	0x0D,
	MIPS_SB_OPCODE = 	0x28,
	MIPS_SH_OPCODE = 	0x29,
	MIPS_SLL_FNCODE = 		 0x00,
	MIPS_SLLV_FNCODE = 		 0x04,
	MIPS_SLT_FNCODE = 		 0x2A,
	MIPS_SLTI_OPCODE = 	0x0A,
	MIPS_SLTIU_OPCODE = 0x0B,
	MIPS_SLTU_FNCODE = 		 0x2B,
	MIPS_SRA_FNCODE = 		 0x03,
	MIPS_SRAV_FNCODE = 		 0x07,
	MIPS_SRL_FNCODE = 		 0x02,
	MIPS_SRLV_FNCODE = 		 0x06,
	MIPS_SUB_FNCODE = 		 0x22,
	MIPS_SUBU_FNCODE = 		 0x23,
	MIPS_SW_OPCODE = 	0x2B,
	MIPS_XOR_FNCODE = 		 0x26,
	MIPS_XORI_OPCODE = 	0x0E


} MIPS_INSTRUCTION_CODES;

// mips_error mips_execute(mips_cpu_h state, Instruction inst);
// mips_error rtype_xor(mips_cpu_h state, Instruction inst);

uint32_t to_big(const uint8_t *pData);
mips_error mips_execute(mips_cpu_h state, Instruction inst);

// mips_error rtype_xor(uint32_t va, uint32_t vb, uint32_t & vd);
// mips_error rtype_and(uint32_t va, uint32_t vb, uint32_t & vd);
// mips_error rtype_or(uint32_t va, uint32_t vb, uint32_t & vd);
// mips_error rtype_sltu(uint32_t va, uint32_t vb, uint32_t & vd);

// mips_error rtype_addu(uint32_t va, uint32_t vb, uint32_t & vd);
// mips_error rtype_add(uint32_t va, uint32_t vb, uint32_t & vd);


///ITYPE
//mips_error itype_addi(uint32_t * va, uint32_t & vd, uint32_t vi);
// struct Unparsed
// {
// 	uint32_t un
// };


//JTYPE
// mips_error jtype_jump(mips_cpu_h state, uint32_t target);
// mips_error jtype_jal(mips_cpu_h state, uint32_t target);
/////////////////////////////////
//////////////////////////////////
/////////////////////////////


mips_error rtype_add(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd);
mips_error rtype_addu(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd);
mips_error rtype_and(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd);
mips_error rtype_div(mips_cpu_h state, uint32_t ra, uint32_t rb);
mips_error rtype_divu(mips_cpu_h state, uint32_t ra, uint32_t rb);
mips_error rtype_jalr(mips_cpu_h state, uint32_t ra, uint32_t rd);
mips_error rtype_jr(mips_cpu_h state, uint32_t ra);
mips_error rtype_mfhi(mips_cpu_h state, uint32_t dest);
mips_error rtype_mflo(mips_cpu_h state, uint32_t dest);
mips_error rtype_mthi(mips_cpu_h state, uint32_t source);
mips_error rtype_mtlo(mips_cpu_h state, uint32_t source);
mips_error rtype_mult(mips_cpu_h state, uint32_t ra, uint32_t rb);
mips_error rtype_multu(mips_cpu_h state, uint32_t ra, uint32_t rb);
mips_error rtype_or(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd);
mips_error rtype_sll(mips_cpu_h state, uint32_t rt, uint32_t rd, uint32_t sa);
mips_error rtype_sllv(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t rd);
mips_error rtype_slt(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t rd);
mips_error rtype_sltu(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd);
mips_error rtype_sra(mips_cpu_h state, uint32_t rt, uint32_t rd, uint32_t sa);
mips_error rtype_srav(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t rd);
mips_error rtype_srl(mips_cpu_h state, uint32_t rt, uint32_t rd, uint32_t sa);
mips_error rtype_srlv(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t rd);
mips_error rtype_sub(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t rd);
mips_error rtype_subu(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t rd);
mips_error rtype_xor(mips_cpu_h state, uint32_t ra, uint32_t rb, uint32_t rd);


mips_error itype_addi(mips_cpu_h state, uint32_t source, uint32_t dest, int16_t immediate);
mips_error itype_addiu(mips_cpu_h state, uint32_t source, uint32_t dest, int16_t immediate);
mips_error itype_andi(mips_cpu_h state, uint32_t source, uint32_t dest, int16_t immediate);
mips_error itype_beq(mips_cpu_h state, uint32_t ra, uint32_t rb, int16_t immediate);
mips_error itype_bgez(mips_cpu_h state, uint32_t rs, int16_t immediate);
mips_error itype_bgezal(mips_cpu_h state, uint32_t rs, uint32_t immediate);
mips_error itype_bgtz(mips_cpu_h state,	uint32_t rs, int16_t immediate);
mips_error itype_blez(mips_cpu_h state,	uint32_t rs, int16_t immediate);
mips_error itype_bltz(mips_cpu_h state,	uint32_t rs, int16_t immediate);
mips_error itype_bltzal(mips_cpu_h state, uint32_t rs, int16_t immediate);
mips_error itype_bne(mips_cpu_h state, uint32_t ra, uint32_t rb, int16_t immediate);
mips_error itype_lb(mips_cpu_h state, uint32_t src,	uint32_t dest, int16_t immediate);
mips_error itype_lbu(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_lh(mips_cpu_h state, uint32_t src,	uint32_t dest, int16_t immediate);
mips_error itype_lhu(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_lw(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_lwl(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_lwr(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_lui(mips_cpu_h state, uint32_t rt, uint32_t immediate);
mips_error itype_ori(mips_cpu_h state, uint32_t src, uint32_t dest, uint32_t immediate);
mips_error itype_sb(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_sh(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_slti(mips_cpu_h state, uint32_t rs, uint32_t rd, int32_t immediate);
mips_error itype_sltiu(mips_cpu_h state, uint32_t rs, uint32_t rd, uint32_t immediate);
mips_error itype_sw(mips_cpu_h state, uint32_t src, uint32_t dest, int16_t immediate);
mips_error itype_xori(mips_cpu_h state, uint32_t rs, uint32_t rt, uint32_t immediate);


mips_error jtype_jump(mips_cpu_h state, uint32_t target);
mips_error jtype_jal(mips_cpu_h state, uint32_t target);

#endif