#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mips.h"
#define MIPS_TEST_LOG_ENABLED

using namespace std;
mips_error write_rtype(mips_mem_h mem, uint32_t source1, uint32_t source2,
                        uint32_t dest, uint32_t shift, uint32_t function, uint32_t start)
{
    
    //opcode = opcode & 0x3F;
    source1 = source1 & 0x1F;
    source2 = source2 & 0x1F;
    dest = dest & 0x1F;
    shift = shift & 0x1F;
    function = function & 0x3F;

    uint32_t instr = (source1 << 21) | (source2 << 16) |
                            (dest << 11) | (shift << 6) | function;

    uint8_t buffer[4];
    buffer[0] = (instr >> 24) & 0xFF;
    buffer[1] = (instr >> 16) & 0xFF;
    buffer[2] = (instr >> 8) & 0xFF;
    buffer[3] = (instr >> 0) & 0xFF;

    return mips_mem_write(mem, start, 4, buffer);
}

mips_error write_itype(mips_mem_h mem, uint32_t opcode, uint32_t source, uint32_t dest,
                        uint32_t imm, uint32_t start)
{
    
    opcode = opcode & 0x3F;
    source = source & 0x1F;
    dest = dest & 0x1F;
    imm = imm & 0xFFFF;

    uint32_t instr = (opcode << 26) | (source << 21) | (dest << 16) | imm;

    uint8_t buffer[4];
    buffer[0] = (instr >> 24) & 0xFF;
    buffer[1] = (instr >> 16) & 0xFF;
    buffer[2] = (instr >> 8) & 0xFF;
    buffer[3] = (instr >> 0) & 0xFF;

    return mips_mem_write(mem, start, 4, buffer);
}

mips_error write_jtype(mips_mem_h mem, uint32_t opcode, uint32_t memory, uint32_t start) {
    
    opcode = opcode & 0x3F;
    memory = memory & 0x3FFFFFF;

    uint32_t instr = (opcode << 26) | memory;

    uint8_t buffer[4];
    buffer[0] = (instr >> 24) & 0xFF;
    buffer[1] = (instr >> 16) & 0xFF;
    buffer[2] = (instr >> 8) & 0xFF;
    buffer[3] = (instr >> 0) & 0xFF;

    return mips_mem_write(mem, start, 4, buffer);
}

mips_error write_word(mips_mem_h mem, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
                        uint32_t start) {
    uint8_t b[4];
    b[0] = b0;
    b[1] = b1;
    b[2] = b2;
    b[3] = b3;

    return mips_mem_write(mem, start, 4, b);
}

void write_log_fail(ofstream &file, mips_error e, string test, string msg)
{
    #ifdef MIPS_TEST_LOG_ENABLED
        file << "FAILED TEST: " << test << ". " << msg << endl;
        file << "Error code: " << hex << e << endl;
        file << endl;
    #endif
}

void rtype_add_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("add");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x20, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADD #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == 12 + 23) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADD #1", "Wrong result CPU");
        //write_log_fail(log, e, "ADD #1",  "Basic addition FAIL.");
    }
}

void rtype_add_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("add");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x20, 4);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADD #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 0, &result);
    
    if (result == 0) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADD #2", "R0 Overwritten");
        
        
    }
}

void rtype_add_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("add");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x20, 8);
    e = mips_cpu_set_register(state, 1, -15);
    e = mips_cpu_set_register(state, 2, 20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADD #3", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 1, &result);
    
    if (result == 5) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADD #3", "Adding mixed sign numbers.");
        
        //write_log_fail(log, e, "ADD #3",  "Mixed sign addition FAIL.");
    }
}

void rtype_add_4(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("add");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x20, 12);
    e = mips_cpu_set_register(state, 1, -15);
    e = mips_cpu_set_register(state, 2, -20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "ADD #4", "Failed to step CPU");
        return;
    }

    e = mips_cpu_get_register(state, 1, &result);
    
    if (result == uint32_t(-35)) {
        mips_test_end_test(testId, true, "Negative addition success");
    } else {
        mips_test_end_test(testId, false, "Adding negative numbers");
        write_log_fail(logfile, e, "ADD #4", "Adding negative numbers");
        
        //write_log_fail(log, e, "ADD #4",  "Negative sign addition FAIL.");
    }
}

void rtype_add_5(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("add");
    uint32_t result, pc;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x20, 16);
    e = mips_cpu_set_register(state, 1, 0x7FFFFFFA);
    e = mips_cpu_set_register(state, 2, 0x06);

    e = mips_cpu_step(state);
    mips_cpu_get_register(state, 1, &result);
    mips_cpu_get_pc(state, &pc);

    if (e != mips_ExceptionArithmeticOverflow ||
        pc != 16 || result != 0x7FFFFFFA) {
        
        mips_test_end_test(testId, false, "Modified regs upon error. Code: " + e);
        write_log_fail(logfile, e, "ADD #5", "Sign overflow");

    } else {
        mips_test_end_test(testId, true, "Overflow success");
    }
}

void rtype_add_6(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("add");
    

    mips_error e = write_rtype(mem, 1, 2, 1, 0xFFFF, 0x20, 16);
    e = mips_cpu_set_register(state, 1, 0x7A);
    e = mips_cpu_set_register(state, 2, 0x05);

    e = mips_cpu_step(state);
       

    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Bad instruction fail. Code: " + e);
         write_log_fail(logfile, e, "ADD #6", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Bad instruction success");
    }
}

void rtype_addu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addu");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x21, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADDU #1", "Cpu step FAILED");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == 12 + 23) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADDU #1", "Addition failed");
        
        //write_log_fail(log, e, "ADD #1",  "Basic addition FAIL.");
    }
}

void rtype_addu_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addu");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x21, 4);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADDU #2", "Cpu step FAILED");
         return;
    }

    e = mips_cpu_get_register(state, 0, &result);
    
    if (result == 0) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADDU #2", "Ovewriting R0");
        
    }
}

void rtype_addu_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addu");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x21, 8);
    e = mips_cpu_set_register(state, 1, -15);
    e = mips_cpu_set_register(state, 2, 20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADDU #3", "Cpu step FAILED");
         return;
    }

    e = mips_cpu_get_register(state, 1, &result);
    
    if (result == 5) {
        mips_test_end_test(testId, true, "Mixed sign addition");
    } else {
        mips_test_end_test(testId, false, "Mixed sign addition");
        write_log_fail(logfile, e, "ADDU #3", "Mixed sign addition");
        
        //write_log_fail(log, e, "ADD #3",  "Mixed sign addition FAIL.");
    }
}

void rtype_addu_4(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addu");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x21, 12);
    e = mips_cpu_set_register(state, 1, -15);
    e = mips_cpu_set_register(state, 2, -20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADDU #4", "Cpu step FAILED");
         //write_log_fail(log, e, "ADD #4",  "Negative sign addition FAIL.");
         return;
    }

    e = mips_cpu_get_register(state, 1, &result);
    
    if (result == uint32_t(-35)) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADDU #4", "Negative addition");
        //write_log_fail(log, e, "ADD #4",  "Negative sign addition FAIL.");
    }
}

void rtype_addu_5(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addu");
    uint32_t result, pc;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x21, 16);
    e = mips_cpu_set_register(state, 1, 0x7FFFFFFA);
    e = mips_cpu_set_register(state, 2, 0x06);

    e = mips_cpu_step(state);
    mips_cpu_get_register(state, 1, &result);
    mips_cpu_get_pc(state, &pc);

    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Modified regs upon error. Code: " + e);
        write_log_fail(logfile, e, "ADDU #5", "Cpu step FAILED");
    } else {
        mips_test_end_test(testId, true, "Overflow success");
    }
}

void rtype_addu_6(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addu");

    mips_error e = write_rtype(mem, 1, 2, 1, 1, 0x21, 20);
    e = mips_cpu_set_register(state, 1, 0x7A);
    e = mips_cpu_set_register(state, 2, 0x05);

    e = mips_cpu_step(state);

    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Bad instruction fail. Code: " + e);
        write_log_fail(logfile, e, "ADDU #6", "Invalid instruction");
    } else {
        
        mips_test_end_test(testId, true, "Bad instruction success");
    }
}

void rtype_and_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("and");
    
    mips_error e = write_rtype(mem, 1, 2, 3, 1, 0x24, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "AND #1", "Badly formatted instruction");
         return;
    }
    
    mips_test_end_test(testId, true, "");    
}

void rtype_and_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("and");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x24, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "AND #2", "CPU step failed");
         return;
    }

    e = mips_cpu_get_register(state, 0, &result);
    
    if (result == 0) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "AND #2", "Writing to R0");
    }
}

void rtype_and_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("and");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x24, 4);
    e = mips_cpu_set_register(state, 1, 0xABCDEFAB);
    e = mips_cpu_set_register(state, 2, 0xBAFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "AND #3", "CPU step failed");
         return;
    }

    e = mips_cpu_get_register(state, 1, &result);
    
    if (result == (0xABCDEFAB & 0xBAFEDCBA)) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "AND #3", "Basic operation");
    }
}

void rtype_div_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("div");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1A, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x85);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #1", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == 4 && res_hi == 5) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "DIV #1", "Wrong results in HI & LO");
    }
}

void rtype_div_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("div");

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x1A, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x84);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #2", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Basic operation success");
    }
}

void rtype_div_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("div");

    mips_error e = write_rtype(mem, 1, 2, 0, 1, 0x1A, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x84);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #3", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Basic operation success");
    }
}

void rtype_div_4(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("div");

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1A, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x84);
    e = mips_cpu_set_register(state, 2, 0x0);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #4", "Division by zero");
    } else {
        mips_test_end_test(testId, true, "Division by zero");
    }
}

void rtype_div_5(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("div");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1A, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, -21);
    e = mips_cpu_set_register(state, 2, 12);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #5", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #5", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #5", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == uint32_t(-1) && res_hi == uint32_t(-9)) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");

        write_log_fail(logfile, e, "DIV #5", "Wrong results in HI & LO ");
    }
}

void rtype_div_6(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("div");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1A, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 2352362);
    e = mips_cpu_set_register(state, 2, -29);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #6", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #6", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIV #6", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == uint32_t(-81115) && res_hi == uint32_t(27)) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "DIV #6", "Wrong results in HI & LO");
    }
}

void rtype_divu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("divu");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1B, 0);   //DIVU
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x85);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #1", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == 4 && res_hi == 5) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "DIVU #1", "Wrong results in HI & LO");
    }
}

void rtype_divu_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("divu");

    mips_error e = write_rtype(mem, 1, 2, 1, 0, 0x1B, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x84);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #2", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Basic operation success");
    }
}

void rtype_divu_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("divu");

    mips_error e = write_rtype(mem, 1, 2, 0, 1, 0x1B, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x84);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #3", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Basic operation success");
    }
}

void rtype_divu_4(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("divu");

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1B, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x84);
    e = mips_cpu_set_register(state, 2, 0x0);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #4", "Division by zero");
    } else {
        mips_test_end_test(testId, true, "Division by zero");
    }
}

void rtype_divu_5(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("divu");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x1B, 0);   //DIV
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x8FFFFFFF);
    e = mips_cpu_set_register(state, 2, 0xFFFFFFFF);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #5", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #5", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "DIVU #5", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == 0 && res_hi == 0x8FFFFFFF) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");

        write_log_fail(logfile, e, "DIVU #5", "Wrong results in HI & LO ");
    }
}

void rtype_jalr_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jalr");
    uint32_t res;

    mips_error e = write_rtype(mem, 1, 0, 2, 0, 0x09, 0);   //JALR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JALR #1", "CPU step failed");
         return;
    }
    e = mips_cpu_get_register(state, 2, &res);
    if (res != 8) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "JALR #1", "INVALID branch return addr");
        return;
    }

    e = mips_cpu_get_pc(state, &res);
    if (res != 4) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "JALR #1", "INVALID branch delay slot addr");
        return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JALR #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_pc(state, &res);
    if (res != 0x84) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "JALR #1", "Wrong branch address");
    } else {
        mips_test_end_test(testId, true, "Branch success");
    }

}

void rtype_jalr_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jalr");
    uint32_t res;

    mips_error e = write_rtype(mem, 1, 0, 0, 0, 0x09, 0);   //JALR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JALR #2", "CPU step failed");
         return;
    }
    e = mips_cpu_get_register(state, 0, &res);
    if (res != 0) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "JALR #2", "R0 Overwritten");
    } else {
        mips_test_end_test(testId, true, "Jalr success");
    }
}

void rtype_jalr_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jalr");

    mips_error e = write_rtype(mem, 1, 1, 2, 0, 0x09, 0);   //JALR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JALR #3", "Invalid instruction");
         
    } else {
        mips_test_end_test(testId, true, "Jalr success");
    }
}

void rtype_jalr_4(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jalr");

    mips_error e = write_rtype(mem, 1, 0, 2, 4, 0x09, 0);   //JALR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JALR #4", "Invalid instruction");
         
    } else {
        mips_test_end_test(testId, true, "Jalr success");
    }
}


void rtype_jr_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jr");
    uint32_t res;

    mips_error e = write_rtype(mem, 1, 0, 0, 0, 0x08, 0);   //JR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JR #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_pc(state, &res);
    if (res != 4) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "JR #1", "INVALID branch delay slot addr");
        return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "JR #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_pc(state, &res);
    if (res != 0x84) {
        mips_test_end_test(testId, false, "Failed to step. Code: " + e);
        write_log_fail(logfile, e, "JR #1", "Wrong branch address");
    } else {
        mips_test_end_test(testId, true, "Branch success");
    }
}


void rtype_jr_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jr");
 //   uint32_t res;

    mips_error e = write_rtype(mem, 1, 1, 0, 0, 0x08, 0);   //JR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Invalid instruction step. Code: " + e);
         write_log_fail(logfile, e, "JR #2", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Invalid instruction success");
    }
}


void rtype_jr_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jr");
//    uint32_t res;

    mips_error e = write_rtype(mem, 1, 0, 1, 0, 0x08, 0);   //JR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Invalid instruction step. Code: " + e);
         write_log_fail(logfile, e, "JR #3", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Invalid instruction success");
    }
}


void rtype_jr_4(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jr");
//    uint32_t res;

    mips_error e = write_rtype(mem, 1, 0, 0, 1, 0x08, 0);   //JR
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x84);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Invalid instruction step. Code: " + e);
         write_log_fail(logfile, e, "JR #4", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Invalid instruction success");
    }
}

void rtype_mthi_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("mthi");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 0, 0, 0, 0x11, 0); // mthi
    e = mips_cpu_set_register(state, 1, 0x9A8B7C6D);    
    e = write_rtype(mem, 0, 0, 2, 0, 0x10, 4);            // mfhi
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MTHI #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MTHI #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result != 0x9A8B7C6D) {
         mips_test_end_test(testId, false, "Invalid result. Code: " + e);
         write_log_fail(logfile, e, "MTHI #1", "WRONG RESULT");
    } else {
        mips_test_end_test(testId, true, "Passed");
    }

    
}

void rtype_mfhi_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("mfhi");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 0, 0, 0, 0x11, 0); // mthi
    e = mips_cpu_set_register(state, 1, 0x9A8B7C6D);
    e = write_rtype(mem, 0, 0, 2, 0, 0x10, 4);            // mfhi
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MFHI #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MFHI #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result != 0x9A8B7C6D) {
         mips_test_end_test(testId, false, "Invalid result. Code: " + e);
         write_log_fail(logfile, e, "MFHI #1", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Invalid instruction success");
    }

    
}


void rtype_mtlo_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("mtlo");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 0, 0, 0, 0x13, 0); // mtlo
    e = mips_cpu_set_register(state, 1, 0x9A8B7C6D);    
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 4);            // mflo
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MTLO #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MTLO #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result != 0x9A8B7C6D) {
         mips_test_end_test(testId, false, "Invalid result. Code: " + e);
         write_log_fail(logfile, e, "MTLO #1", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Invalid instruction success");
    }

    
}

void rtype_mflo_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("mflo");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 0, 0, 0, 0x13, 0); // mtlo
    e = mips_cpu_set_register(state, 1, 0x9A8B7C6D);    
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 4);            // mflo
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MFLO #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MFLO #1", "CPU step failed");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result != 0x9A8B7C6D) {
         mips_test_end_test(testId, false, "Invalid result. Code: " + e);
         write_log_fail(logfile, e, "MFLO #1", "Invalid instruction");
    } else {
        mips_test_end_test(testId, true, "Invalid instruction success");
    }


}

// void rtype_mfhi_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
//     int testId = mips_test_begin_test("jr");
// //    uint32_t res;

//     mips_error e = write_rtype(mem, 1, 0, 0, 1, 0x08, 0);   //JR
//     e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
//     e = write_rtype(mem, 0, 0, 0, 0, 0x00, 4);
//     e = mips_cpu_set_register(state, 1, 0x84);

//     e = mips_cpu_step(state);
//     if (e == mips_Success) {
//          mips_test_end_test(testId, false, "Invalid instruction step. Code: " + e);
//          write_log_fail(logfile, e, "JR #4", "Invalid instruction");
//     } else {
//         mips_test_end_test(testId, true, "Invalid instruction success");
//     }
// }


void rtype_mult_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("mult");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x18, 0);   //MULT
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x85);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULT #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULT #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULT #1", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == 0x10A0 && res_hi == 0) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "MULT #1", "Wrong results in HI & LO");
    }
}

void rtype_mult_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("mult");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x18, 0);   //MULT
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0xFFFFFFFF);
    e = mips_cpu_set_register(state, 2, 0x2);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULT #2", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULT #2", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULT #2", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == uint32_t(-2) && res_hi == uint32_t(-1)) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "MULT #2", "Wrong results in HI & LO");
    }
}


void rtype_multu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("multu");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x19, 0);   //MULT
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0x85);
    e = mips_cpu_set_register(state, 2, 0x20);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULTU #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULTU #1", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULTU #1", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == 0x10A0 && res_hi == 0) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "MULTU #1", "Wrong results in HI & LO");
    }
}

void rtype_multu_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("multu");
    uint32_t res_hi, res_lo;

    mips_error e = write_rtype(mem, 1, 2, 0, 0, 0x19, 0);   //MULT
    e = write_rtype(mem, 0, 0, 1, 0, 0x10, 4);   //MFHI > 1
    e = write_rtype(mem, 0, 0, 2, 0, 0x12, 8);   //MFLO > 2
    e = mips_cpu_set_register(state, 1, 0xFFFFFFFF);
    e = mips_cpu_set_register(state, 2, 0x2);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULTU #2", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULTU #2", "CPU step failed");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "MULTU #2", "CPU step failed");
         return;
    }


    e = mips_cpu_get_register(state, 1, &res_hi);
    e = mips_cpu_get_register(state, 2, &res_lo);

    if (res_lo == 0xFFFFFFFE && res_hi == 0x01) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        

        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "MULTU #2", "Wrong results in HI & LO");
    }
}



void rtype_or_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("or");
    
    mips_error e = write_rtype(mem, 1, 2, 3, 1, 0x25, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 23);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "OR #1", "Badly formatted instruction");
         return;
    }
    
    mips_test_end_test(testId, true, "");    
}

void rtype_or_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("or");
    uint32_t res;
    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x25, 0);
    e = mips_cpu_set_register(state, 1, 0xABCDEFAB);
    e = mips_cpu_set_register(state, 2, 0xBAFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "OR #2", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == (0xABCDEFAB | 0xBAFEDCBA)) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "OR #2", "Wrong result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_sll_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sll");
    uint32_t res;
    mips_error e = write_rtype(mem, 0, 2, 3, 0x12, 0x00, 0);
    e = mips_cpu_set_register(state, 2, 0xBAFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SLL #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == ((0xBAFEDCBA << 0x12) & 0xFFFFF000)) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SLL #1", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_sllv_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sllv");
    uint32_t res;
    mips_error e = write_rtype(mem, 1, 2, 3, 0x00, 0x04, 0);
    e = mips_cpu_set_register(state, 1, 0x12);
    e = mips_cpu_set_register(state, 2, 0xBAFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SLLV #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == ((0xBAFEDCBA << 0x12) & 0xFFFFF000)) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SLLV #1", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_slt_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("slt");
    uint32_t res;
    mips_error e = write_rtype(mem, 1, 2, 3, 0x00, 0x2A, 0);
    e = mips_cpu_set_register(state, 1, 0x7FFFFFFF);
    e = mips_cpu_set_register(state, 2, 0xFFFFFFFF);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SLT #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SLT #1", "Wrong result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_sltu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sltu");
    uint32_t res;
    mips_error e = write_rtype(mem, 1, 2, 3, 0x00, 0x2B, 0);
    e = mips_cpu_set_register(state, 1, 0x7FFFFFFF);
    e = mips_cpu_set_register(state, 2, 0xFFFFFFFF);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SLTU #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 1) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SLTU #1", "Wrong result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}


void rtype_sra_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sra");
    uint32_t res;
    mips_error e = write_rtype(mem, 0, 2, 3, 0x0C, 0x03, 0);
    e = mips_cpu_set_register(state, 2, 0x7AFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SRA #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0x7AFED) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SRA #1", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_sra_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sra");
    uint32_t res;
    mips_error e = write_rtype(mem, 0, 2, 3, 0x0C, 0x03, 0);
    e = mips_cpu_set_register(state, 2, 0xFAFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SRA #2", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0xFFFFAFED) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SRA #2", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_srav_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("srav");
    uint32_t res;
    mips_error e = write_rtype(mem, 5, 2, 3, 0x00, 0x07, 0);
    e = mips_cpu_set_register(state, 5, 0x08);
    e = mips_cpu_set_register(state, 2, 0x7AFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SRAV #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0x7AFEDC) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SRAV #1", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_srav_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("srav");
    uint32_t res;
    mips_error e = write_rtype(mem, 5, 2, 3, 0x00, 0x07, 0);
    e = mips_cpu_set_register(state, 5, 0x00000008);
    e = mips_cpu_set_register(state, 2, 0x8AFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SRAV #2", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0xFF8AFEDC) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SRAV #2", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_srl_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("srl");
    uint32_t res;
    mips_error e = write_rtype(mem, 0, 2, 3, 0x08, 0x02, 0);
    e = mips_cpu_set_register(state, 2, 0x8AFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SRL #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0x008AFEDC) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SRL #1", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_srlv_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("srlv");
    uint32_t res;
    mips_error e = write_rtype(mem, 1, 2, 3, 0x00, 0x06, 0);
    e = mips_cpu_set_register(state, 1, 0x0C);
    e = mips_cpu_set_register(state, 2, 0x8AFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SRLV #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == 0x0008AFED) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "SRLV #1", "Wrong shift result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_sub_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sub");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x22, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 5);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SUB #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == uint32_t(12 - 5)) {
        mips_test_end_test(testId, true, "Basic subtraction success");
    } else {
        mips_test_end_test(testId, false, "Basic subtraction FAILED");
        write_log_fail(logfile, e, "SUB #1", "Wrong result CPU");
    }
}

void rtype_sub_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sub");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x22, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 13);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SUB #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == uint32_t(- 1)) {
        mips_test_end_test(testId, true, "Basic subtraction success");
    } else {
        mips_test_end_test(testId, false, "Basic subtraction FAILED");
        write_log_fail(logfile, e, "SUB #2", "Wrong result CPU");
    }
}

void rtype_sub_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sub");
    //uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x22, 0);
    e = mips_cpu_set_register(state, 1, 0x80000000);
    e = mips_cpu_set_register(state, 2, 1);

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SUB #3", "No exception returned on overflow");
    } else {
        mips_test_end_test(testId, true, "Basic subtraction success");
    }
}

void rtype_subu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("subu");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x23, 0);
    e = mips_cpu_set_register(state, 1, 12);
    e = mips_cpu_set_register(state, 2, 5);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SUBU #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == uint32_t(12 - 5)) {
        mips_test_end_test(testId, true, "Basic subtraction success");
    } else {
        mips_test_end_test(testId, false, "Basic subtraction FAILED");
        write_log_fail(logfile, e, "SUBU #1", "Wrong result CPU");
    }
}

void rtype_subu_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("subu");
    uint32_t result;

    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x23, 0);
    e = mips_cpu_set_register(state, 1, 0x80000000);
    e = mips_cpu_set_register(state, 2, 1);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "SUBU #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == 0x7FFFFFFF) {
        mips_test_end_test(testId, true, "Basic subtraction success");
    } else {
        mips_test_end_test(testId, false, "Basic subtraction FAILED");
        write_log_fail(logfile, e, "SUBU #2", "Wrong result CPU");
    }
}



void rtype_xor_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("xor");
    uint32_t res;
    mips_error e = write_rtype(mem, 1, 2, 3, 0, 0x26, 0);
    e = mips_cpu_set_register(state, 1, 0xABCDEFAB);
    e = mips_cpu_set_register(state, 2, 0xBAFEDCBA);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "XOR #1", "Cpu step failed");
         return;
    }
    
    e = mips_cpu_get_register(state, 3, &res);

    if (res == (0xABCDEFAB ^ 0xBAFEDCBA)) {
        mips_test_end_test(testId, true, "");        
    } else {
        mips_test_end_test(testId, false, "Wrong result. Code: " + e);
         write_log_fail(logfile, e, "XOR #1", "Wrong result in R3");
    }

    //mips_test_end_test(testId, true, "");    
}

void rtype_add(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);

    rtype_add_1(state, mem, logfile);
    rtype_add_2(state, mem, logfile);
    rtype_add_3(state, mem, logfile);
    rtype_add_4(state, mem, logfile);
    rtype_add_5(state, mem, logfile);
    rtype_add_6(state, mem, logfile);

}

void rtype_addu(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);

    rtype_addu_1(state, mem, logfile);
    rtype_addu_2(state, mem, logfile);
    rtype_addu_3(state, mem, logfile);
    rtype_addu_4(state, mem, logfile);
    rtype_addu_5(state, mem, logfile);
    rtype_addu_6(state, mem, logfile);

}

void rtype_and(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_and_1(state, mem, logfile);
    rtype_and_2(state, mem, logfile);
    rtype_and_3(state, mem, logfile);
}

void rtype_div(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_div_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_div_2(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_div_3(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_div_4(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_div_5(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_div_6(state, mem, logfile);
}

void rtype_divu(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_divu_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_divu_2(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_divu_3(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_divu_4(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_divu_5(state, mem, logfile);
}

void rtype_jalr(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_jalr_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_jalr_2(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_jalr_3(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_jalr_4(state, mem, logfile);
}

void rtype_jr(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_jr_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_jr_2(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_jr_3(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_jr_4(state, mem, logfile);
}

//NO EXTRA
void rtype_mult(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_mult_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_mult_2(state, mem, logfile);
    // mips_cpu_reset(state);
    // rtype_jr_3(state, mem, logfile);
    // mips_cpu_reset(state);
    // rtype_jr_4(state, mem, logfile);
}

//NO EXTRA
void rtype_multu(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_multu_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_multu_2(state, mem, logfile);
    // mips_cpu_reset(state);
    // rtype_jr_3(state, mem, logfile);
    // mips_cpu_reset(state);
    // rtype_jr_4(state, mem, logfile);
}

void rtype_or(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_or_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_or_2(state, mem, logfile);
    // mips_cpu_reset(state);
    // rtype_jr_3(state, mem, logfile);
    // mips_cpu_reset(state);
    // rtype_jr_4(state, mem, logfile);
}

void rtype_sll(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_sll_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_sllv_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_slt_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_sltu_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_sra_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_sra_2(state, mem, logfile);

    mips_cpu_reset(state);
    rtype_srav_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_srav_2(state, mem, logfile);

    mips_cpu_reset(state);
    rtype_srl_1(state, mem, logfile);

    mips_cpu_reset(state);
    rtype_srlv_1(state, mem, logfile);
}

void rtype_sub(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_sub_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_sub_2(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_sub_3(state, mem, logfile);
    mips_cpu_reset(state);

    mips_cpu_reset(state);
    rtype_subu_1(state, mem, logfile);
    mips_cpu_reset(state);
    rtype_subu_2(state, mem, logfile);
    mips_cpu_reset(state);
    //rtype_sub_2(state, mem, logfile);
}

void rtype_xor(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_xor_1(state, mem, logfile);
}

void rtype_hilo(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    rtype_mthi_1(state, mem, logfile);

    mips_cpu_reset(state);
    rtype_mtlo_1(state, mem, logfile);

    mips_cpu_reset(state);
    rtype_mfhi_1(state, mem, logfile);

    mips_cpu_reset(state);
    rtype_mflo_1(state, mem, logfile);


}

void itype_addi_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addi");
    uint32_t result;

    mips_error e = write_itype(mem, 0x08, 2, 3, 0x1234, 0);
    e = mips_cpu_set_register(state, 2, 0x1);
    

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step. Code: " + e);
         write_log_fail(logfile, e, "ADDI #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    
    if (result == 0x1235) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADDI #1", "Wrong result CPU");
    }
}

void itype_addi_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addi");

    mips_error e = write_itype(mem, 0x08, 2, 3, 0x01, 0);
    e = mips_cpu_set_register(state, 2, 0x7FFFFFFF);
    

    e = mips_cpu_step(state);
    if (e == mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "ADDI #2", "Failed to step CPU");

    } else {
        mips_test_end_test(testId, true, "Basic addition success");
    }
}

void itype_addi_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addi");
    uint32_t result;

    mips_error e = write_itype(mem, 0x08, 2, 3, 0x01, 0);
    e = mips_cpu_set_register(state, 2, 0x8FFFFFFE);
    

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "ADDI #3", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    if (result == 0x8FFFFFFF) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADDI #3", "Wrong result CPU");
    }
}

void itype_addiu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("addiu");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x09, 2, 3, 0x01, 0);
    e = mips_cpu_set_register(state, 2, 0x7FFFFFFF);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "ADDIU #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    if (result == 0x80000000) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ADDIU #1", "Wrong result CPU");
    }
}

void itype_andi_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("andi");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x0C, 2, 3, 0xABCD, 0);
    e = mips_cpu_set_register(state, 2, 0xABCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "ANDI #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 3, &result);
    if (result == (0xABCDEFAB & 0x0000ABCD)) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "ANDI #1", "Wrong result CPU");
    }
}

void itype_beq_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("beq");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x04, 2, 3, 0xABCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0xABCDEFAB);
    e = mips_cpu_set_register(state, 3, 0xABCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BEQ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BEQ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0xfffeaf2c) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BEQ #1", "Wrong result CPU");
    }
}

void itype_beq_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("beq");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x04, 2, 3, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0xABCDEFAB);
    e = mips_cpu_set_register(state, 3, 0xABCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BEQ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BEQ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BEQ #2", "Wrong result CPU");
    }
}

void itype_bgez_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bgez");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x01, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x0BCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BGEZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BGEZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BGEZ #1", "Wrong result CPU");
    }
}

void itype_bgezal_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bgezal");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x11, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x0BCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BGEZAL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 31, &result);
    if (result != 0x08) {
        mips_test_end_test(testId, false, "Wrong link address. Code: " + e);
        write_log_fail(logfile, e, "BGEZAL #1", "Wrong link address");
        return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BGEZAL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BGEZAL #1", "Wrong result CPU");
    }
}

void itype_bgezal_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bgezal");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x11, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0xABCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BGEZAL #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 31, &result);
    if (result != 0x08) {
        mips_test_end_test(testId, false, "Wrong link address. Code: " + e);
        write_log_fail(logfile, e, "BGEZAL #2", "Wrong link address");
        return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BGEZAL #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x08) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "BGEZAL #2", "Wrong result CPU");
    }
}

void itype_bgtz_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bgtz");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x07, 2, 0x00, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x0BCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BGTZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BGTZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BGTZ #1", "Wrong result CPU");
    }
}

void itype_bgtz_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bgtz");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x07, 2, 0x00, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0xABCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BGTZ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BGTZ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x08) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BGTZ #2", "Wrong result CPU");
    }
}


void itype_blez_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("blez");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x06, 2, 0x00, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x00);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLEZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLEZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BLEZ #1", "Wrong result CPU");
    }
}

void itype_blez_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("blez");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x06, 2, 0x00, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x0BCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLEZ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLEZ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x08) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BLEZ #2", "Wrong result CPU");
    }
}

void itype_bltz_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bltz");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x00, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x80000000);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLTZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLTZ #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BLTZ #1", "Wrong result CPU");
    }
}

void itype_bltz_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bltz");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x00, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x00000000);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLTZ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BLTZ #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x08) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BLTZ #2", "Wrong result CPU");
    }
}

void itype_bltzal_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bltzal");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x10, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0x0BCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BLTZAL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 31, &result);
    if (result != 0x08) {

        
        mips_test_end_test(testId, false, "Wrong link address. Code: " + e);
        write_log_fail(logfile, e, "BLTZAL #1", "Wrong link address");
        return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BLTZAL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x08) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BLTZAL #1", "Wrong result CPU");
    }
}

void itype_bltzal_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bltzal");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x01, 2, 0x10, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 2, 0xABCDEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BLTZAL #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 31, &result);
    if (result != 0x08) {
        mips_test_end_test(testId, false, "Wrong link address. Code: " + e);
        write_log_fail(logfile, e, "BLTZAL #2", "Wrong link address");
        return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Failed to step CPU. Code: " + e);
         write_log_fail(logfile, e, "BLTZAL #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BLTZAL #2", "Wrong result CPU");
    }
}

void itype_bne_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bne");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x05,1, 2, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x0BCDEFAB);
    e = mips_cpu_set_register(state, 2, 0x0BCEEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BNE #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BNE #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x2F2C) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BNE #1", "Wrong result CPU");
    }
}

void itype_bne_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("bne");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x05,1, 2, 0x0BCA, 0);
    e = write_itype(mem, 0x00, 0, 0, 0x00, 4);
    e = mips_cpu_set_register(state, 1, 0x0BCAEFAB);
    e = mips_cpu_set_register(state, 2, 0x0BCAEFAB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BNE #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "BNE #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);

    if (result == 0x08) {
        mips_test_end_test(testId, true, "Basic addition success");
    } else {
        mips_test_end_test(testId, false, "Basic addition FAILED");
        write_log_fail(logfile, e, "BNE #2", "Wrong result CPU");
    }
}

void itype_lb_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lb");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x20, 1, 2, 0x10, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x02); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LB #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x3C) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LB #1", "Wrong result CPU");
    }
}

void itype_lb_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lb");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x20,1, 2, 0xFFFF, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x12); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LB #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x2B) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LB #2", "Wrong result CPU");
    }
}

void itype_lbu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lbu");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x24, 1, 2, 0x10, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x02); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LBU #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x3C) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LBU #1", "Wrong result CPU");
    }
}

void itype_lh_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lh");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x21, 1, 2, 0x10, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x02); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LH #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x3C4D) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LH #1", "Wrong result CPU");
    }
}


void itype_lhu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lhu");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x25, 1, 2, 0x10, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x02); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LHU #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x3C4D) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LHU #1", "Wrong result CPU");
    }
}

void itype_lw_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lw");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x23, 1, 2, 0x0E, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x02); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LW #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x1A2B3C4D) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LW #1", "Wrong result CPU");
    }
}

void itype_lwl_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lwl");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x22, 1, 2, 0x0E, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    mips_cpu_set_register(state, 2, 0x23); //0x10
    
    e = mips_cpu_set_register(state, 1, 0x03); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LWL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x2B3C4D23) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LWL #1", "Wrong result CPU");
    }
}

void itype_lwr_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lwr");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x26, 1, 2, 0x0E, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    mips_cpu_set_register(state, 2, 0x23456789); //0x10
    
    e = mips_cpu_set_register(state, 1, 0x03); //0x10
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LWR #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0x23451A2B) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LWR #1", "Wrong result CPU");
    }
}

void itype_lui_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("lui");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x0F, 0, 2, 0xABCD, 0);

    e = mips_cpu_set_register(state, 2, 0x23456789); //0x10
    
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "LUI #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == 0xABCD0000) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "LUI #1", "Wrong result CPU");
    }
}


void itype_ori_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("ori");
    uint32_t result;
    
    mips_error e = write_itype(mem, 0x0D, 1, 2, 0xABCD, 0);

    e = mips_cpu_set_register(state, 1, 0x23456789); //0x10
    
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "ORI #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &result);

    if (result == (0xABCD | 0x23456789)) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "ORI #1", "Wrong result CPU");
    }
}


void itype_sb_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sb");
    
    
    mips_error e = write_itype(mem, 0x28, 1, 2, 0x0F, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x03);
    e = mips_cpu_set_register(state, 2, 0x123456AA);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SB #1", "Failed to step CPU");
         return;
    }

    uint8_t data[4];
    e = mips_mem_read(mem, 0x10, 4, data);

    if (data[2] == 0xAA) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SB #1", "Wrong result CPU");
    }
}


void itype_sh_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sh");
    
    
    mips_error e = write_itype(mem, 0x29, 1, 2, 0x0F, 0);
    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x03);
    e = mips_cpu_set_register(state, 2, 0x1234AABB);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SH #1", "Failed to step CPU");
         return;
    }

    uint8_t data[4];
    e = mips_mem_read(mem, 0x10, 4, data);

    if (data[2] == 0xAA && data[3] == 0xBB) {

        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SH #1", "Wrong result CPU");
    }
}

void itype_slti_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("slti");
    uint32_t res;
    
    mips_error e = write_itype(mem, 0x0A, 1, 2, 0x0F, 0);
//    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x8FFFFFFF);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SLTI #1", "Failed to step CPU");
         return;
    }


    e = mips_cpu_get_register(state, 2, &res);

    if (res == 1) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SLTI #1", "Wrong result CPU");
    }
}

void itype_slti_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("slti");
    uint32_t res;
    
    mips_error e = write_itype(mem, 0x0A, 1, 2, 0x0F, 0);
//    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x7FFFFFFF);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SLTI #2", "Failed to step CPU");
         return;
    }


    e = mips_cpu_get_register(state, 2, &res);

    if (res == 0) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SLTI #2", "Wrong result CPU");
    }
}


void itype_sltiu_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sltiu");
    uint32_t res;
    
    mips_error e = write_itype(mem, 0x0B, 1, 2, 0x0F, 0);
//    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x8FFFFFFF);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SLTIU #1", "Failed to step CPU");
         return;
    }


    e = mips_cpu_get_register(state, 2, &res);

    if (res == 0) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SLTIU #1", "Wrong result CPU");
    }
}

void itype_sltiu_2(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sltiu");
    uint32_t res;
    
    mips_error e = write_itype(mem, 0x0B, 1, 2, 0x0F, 0);
//    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x0F);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SLTIU #2", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 2, &res);

    if (res == 0) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SLTIU #2", "Wrong result CPU");
    }
}

void itype_sltiu_3(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sltiu");
    uint32_t res;
    
    mips_error e = write_itype(mem, 0x0B, 1, 2, 0xFF, 0);
//    e = write_word(mem, 0x1A, 0x2B, 0x3C, 0x4D, 0x10);

    e = mips_cpu_set_register(state, 1, 0x0F);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SLTIU #3", "Failed to step CPU");
         return;
    }


    e = mips_cpu_get_register(state, 2, &res);

    if (res == 1) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SLTIU #3", "Wrong result CPU");
    }
}


void itype_sw_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("sw");
    
    mips_error e = write_itype(mem, 0x2B, 1, 2, 0x02, 0);
    e = mips_cpu_set_register(state, 1, 0x0E);
    e = mips_cpu_set_register(state, 2, 0xABCDEF0F);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "SW #1", "Failed to step CPU");
         return;
    }

    uint8_t d[4];
    e = mips_mem_read(mem, 0x10, 4, d);

    if (d[0] == 0xAB && d[1] == 0xCD && d[2] == 0xEF && d[3] == 0x0F) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "SW #1", "Wrong result CPU");
    }
}

void itype_xori_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("xori");
    uint32_t res;

    mips_error e = write_itype(mem, 0x0E, 2, 1, 0xF0BC, 0);
    e = mips_cpu_set_register(state, 1, 0x0E);
    e = mips_cpu_set_register(state, 2, 0xABCDEF0F);

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "XORI #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_register(state, 1, &res);

    if (res == 0xABCD1FB3) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "XORI #1", "Wrong result CPU");
    }
}


void itype_arithm(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    itype_addi_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_addi_2(state, mem, logfile);
    mips_cpu_reset(state);
    itype_addi_3(state, mem, logfile);


    mips_cpu_reset(state);
    itype_addiu_1(state, mem, logfile);
}

void itype_logic(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    itype_andi_1(state, mem, logfile);
}

void itype_branch(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    itype_beq_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_beq_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_bgez_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_bgezal_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_bgezal_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_bgtz_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_bgtz_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_blez_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_blez_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_bltz_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_bltz_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_bltzal_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_bltzal_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_bne_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_bne_2(state, mem, logfile);
}

void itype_mem(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    itype_lb_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lb_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lbu_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lh_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lhu_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lw_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lwl_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lwr_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_lui_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_ori_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_sb_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_sh_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_slti_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_slti_2(state, mem, logfile);

    mips_cpu_reset(state);
    itype_sltiu_1(state, mem, logfile);
    mips_cpu_reset(state);
    itype_sltiu_2(state, mem, logfile);
    mips_cpu_reset(state);
    itype_sltiu_3(state, mem, logfile);

    mips_cpu_reset(state);
    itype_sw_1(state, mem, logfile);

    mips_cpu_reset(state);
    itype_xori_1(state, mem, logfile);

}


void jtype_jump_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("j");
    uint32_t result;
    
    mips_error e = write_jtype(mem, 0x02, 0xFF, 0);
    write_jtype(mem, 0x00, 0x00, 4);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "J #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "J #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);


    if (result == 0x03FC) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "J #1", "Wrong JUMP address CPU");
    }
}

void jtype_jal_1(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    int testId = mips_test_begin_test("jal");
    uint32_t result, link;
    
    mips_error e = write_jtype(mem, 0x03, 0xFF, 0);
    write_jtype(mem, 0x00, 0x00, 4);
    
    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "JAL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_step(state);
    if (e != mips_Success) {
         mips_test_end_test(testId, false, "Step success on overflow. Code: " + e);
         write_log_fail(logfile, e, "JAL #1", "Failed to step CPU");
         return;
    }

    e = mips_cpu_get_pc(state, &result);
    e = mips_cpu_get_register(state, 31, &link);

    if (result == 0x03FC && link == 0x08) {
        mips_test_end_test(testId, true, "Basic operation success");
    } else {
        mips_test_end_test(testId, false, "Basic operation FAILED");
        write_log_fail(logfile, e, "JAL #1", "Wrong JUMP address CPU");
    }
}

void jtypes(mips_cpu_h state, mips_mem_h mem, ofstream &logfile) {
    mips_cpu_reset(state);
    jtype_jump_1(state, mem, logfile);

    mips_cpu_reset(state);
    jtype_jal_1(state, mem, logfile);
}



int main()
{
    mips_mem_h mem = mips_mem_create_ram(4096, 4);
    mips_cpu_h cpu = mips_cpu_create(mem);
    ofstream logfile;
    logfile.open("mips_error_log.txt");
    
    
    mips_error e = mips_cpu_set_debug_level(cpu, 4, stderr);
    if(e != mips_Success){
        fprintf(stderr, "mips_cpu_set_debug_level : failed.\n");
        exit(1);
    }
    
    mips_test_begin_suite();

    rtype_add(cpu, mem, logfile);
    rtype_addu(cpu, mem, logfile);
    rtype_and(cpu, mem, logfile);
    rtype_div(cpu, mem, logfile);
    rtype_divu(cpu, mem, logfile);
    rtype_jalr(cpu, mem, logfile);
    rtype_jr(cpu, mem, logfile);
    rtype_hilo(cpu, mem, logfile);
    rtype_mult(cpu, mem, logfile);
    rtype_multu(cpu, mem, logfile);
    rtype_or(cpu, mem, logfile);
    rtype_sll(cpu, mem, logfile);
    rtype_sub(cpu, mem, logfile);
    rtype_xor(cpu, mem, logfile);


    itype_arithm(cpu, mem, logfile);
    itype_logic(cpu, mem, logfile);
    itype_branch(cpu, mem, logfile);
    itype_mem(cpu, mem, logfile);

    jtypes(cpu, mem, logfile);

    logfile.close();
    
    mips_test_end_suite();
    
    return 0;
}
