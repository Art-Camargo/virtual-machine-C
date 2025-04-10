#include <iostream>
#include <unistd.h>
#include <string>
#include "loader.cpp"

using namespace std;

void alu(int opcode, int reg_dest, int reg_op1, int reg_op2, int registers[4]) {
    switch (opcode) {
        case 0: registers[reg_dest] = registers[reg_op1] + registers[reg_op2]; break; // add
        case 1: registers[reg_dest] = registers[reg_op1] - registers[reg_op2]; break; // sub
        case 2: registers[reg_dest] = registers[reg_op1] * registers[reg_op2]; break; // mul
        case 3: registers[reg_dest] = (registers[reg_op2] != 0) ? registers[reg_op1] / registers[reg_op2] : 0; break; // div
    }
}

void exec(instruction mem[], int data_memory[]) {
  int pc = 0, stopped = 0, opcode;
  int registers[4] = {0};

  while ((pc < INSTRUCTION_MEMORY_SIZE) && !stopped) {
      opcode = mem[pc].opcode;

      if (opcode == -1) {
          pc++;
          continue;
      }

      int reg_dest = mem[pc].operands[0];
      int reg_op1 = mem[pc].operands[1];
      int reg_op2 = mem[pc].operands[2];

      switch (opcode) {
          case 0:
          case 1:
          case 2:
          case 3: alu(opcode, reg_dest, reg_op1, reg_op2, registers); break; // add, sub, mul, div
          case 4: registers[reg_dest] = data_memory[reg_op1]; break;             // mv (load)
          case 5: data_memory[registers[reg_dest]] = registers[reg_op1]; break;            // st (store)
          case 6: pc = reg_dest - 1; break;                                // jmp
          case 7: if (registers[reg_dest] == registers[reg_op1]) { pc = reg_op2; continue; } break; // jeq
          case 8: if (registers[reg_dest] >  registers[reg_op1]) { pc = reg_op2; continue; } break; // jgt
          case 9: if (registers[reg_dest] <  registers[reg_op1]) { pc = reg_op2; continue; } break; // jlt
          case 10: cout << "Output: " << registers[reg_dest] << endl; break; // w
          case 11: cout << "Input: "; cin >> registers[reg_dest]; break;    // r
          case 12: stopped = 1; break;                               // stp
      }
      pc++;
      printf("Executing instruction %d: ", pc);
      printf("Opcode: %d, Operands: ", opcode);
      printf("Registers [%d, %d, %d], ", registers[0], registers[1], registers[2]);
      printf("\n\n\n");
      sleep(1);
  }
}

int main() {
  instruction instruction_memory[INSTRUCTION_MEMORY_SIZE];
  int data_memory[DATA_MEMORY_SIZE];
  reset_data_memory(data_memory);
  reset_instruction_memory(instruction_memory);
  load_program("program.txt", instruction_memory, data_memory);
  exec(instruction_memory, data_memory);
  return 0;
}
