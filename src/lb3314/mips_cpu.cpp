#include <iostream>

#include "mips.h"
#include "headers/mips_cpu_instruction.h"


mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
) 
{
    if (index >= 32) {
        return mips_ExceptionInvalidAddress;
    }

    *value = state->regs[index];
    return mips_Success;
}

/*! Modifies one of the 32 general purpose MIPS registers. */
mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
)
{
    if (index >= 32) {
        return mips_ExceptionInvalidAddress;
    }

    if (index != 0) {
        state->regs[index] = value;    
    }
    
    return mips_Success;
}

mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest)
{
    state->logLevel = level;
    state->logDst = dest;
    return mips_Success;
}

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
    mips_cpu_impl *cpu = new mips_cpu_impl;
    
    cpu->pc = 0;
    cpu->pcNext = 4;
    for(int i = 0;i < 32;i++){
        cpu->regs[i] = 0;
    }
    cpu->mem = mem;
    
    return cpu;
}

mips_error mips_cpu_set_pc(
    mips_cpu_h state,   //!< Valid (non-empty) handle to a CPU
    uint32_t pc         //!< Address of the next instruction to exectute.
) {
    state->pc = pc;
    state->pcNext = pc + 4;

    return mips_Success;
}

mips_error mips_cpu_get_pc(
    mips_cpu_h state,   //!< Valid (non-empty) handle to a CPU
    uint32_t *pc        //!< Where to write the byte address too
) {

    *pc = state->pc;

    return mips_Success;
}


mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
)
{
    uint8_t buffer[4];
    
    mips_error err=mips_mem_read(
        state->mem,		//!< Handle to target memory
        state->pc,	//!< Byte address to start transaction at
        4,	//!< Number of bytes to transfer
        buffer	//!< Receives the target bytes
    );
    
    if(err != 0) return err;
    
    Instruction inst;
    inst.unparsed = to_big(buffer);

    err = mips_execute(state, inst);

    //print_state(state);

    return err;

}

mips_error mips_cpu_reset(mips_cpu_h state){
    //cpu->registers = {0};
    for(int i = 1; i < 32; i++) {
        state->regs[i] = 0;
    }

    state->pc = 0;
    state->pcNext = 4;
    
    return mips_Success;
}

void mips_cpu_free(mips_cpu_h state) {
    state->pc += 0;
}
