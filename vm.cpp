#include <iostream>
#include <unistd.h>

using namespace std;

#define INSTRUCTION_MEMORY_SIZE 320
#define DATA_MEMORY_SIZE 256

typedef struct {
    int opcode;
    int operands[3];
    int operand_count;
} instruction;

void reset_instruction_memory(instruction memory[]) {
    for (int i = 0; i < INSTRUCTION_MEMORY_SIZE; i++) {
        memory[i].opcode = -1;
        memory[i].operand_count = 0;
        for (int j = 0; j < 3; j++) {
            memory[i].operands[j] = 0;
        }
    }
}

void put(instruction memory[], int position, int opcode, int operand_count, int operands[]) {
    memory[position].opcode = opcode;
    memory[position].operand_count = operand_count;
    for (int i = 0; i < operand_count; i++) {
        memory[position].operands[i] = operands[i];
    }
}

int get_opcode(instruction memory[], int pc) {
    return memory[pc].opcode;
}

void exec(instruction mem[], int data_memory[]) {
  int pc = 0, stopped = 0, opcode;
  int registers[4] = {0};

  while (pc < INSTRUCTION_MEMORY_SIZE && !stopped) {
      opcode = mem[pc].opcode;

      if (opcode == -1) {
          pc++;
          continue;
      }

      int reg_dest = mem[pc].operands[0];
      int reg_op1 = mem[pc].operands[1];
      int a2 = mem[pc].operands[2];

      switch (opcode) {
          case 0: registers[reg_dest] = registers[reg_op1] + registers[reg_op2]; break; // add
          case 1: registers[reg_dest] = registers[reg_op1] - registers[reg_op2]; break; // sub
          case 2: registers[reg_dest] = registers[reg_op1] * registers[reg_op2]; break; // mul
          case 3: registers[reg_dest] = (registers[reg_op2] != 0) ? registers[reg_op1] / registers[reg_op2] : 0; break; // div
          case 4: registers[reg_dest] = data_memory[reg_op1]; break;             // mv (load)
          case 5: data_memory[reg_dest] = registers[reg_op1]; break;            // st (store)
          case 6: pc = reg_dest; continue;                                // jmp
          case 7: if (registers[reg_dest] == registers[reg_op1]) { pc = reg_op2; continue; } break; // jeq
          case 8: if (registers[reg_dest] >  registers[reg_op1]) { pc = reg_op2; continue; } break; // jgt
          case 9: if (registers[reg_dest] <  registers[reg_op1]) { pc = reg_op2; continue; } break; // jlt
          case 10: cout << "Output: " << registers[reg_dest] << endl; break; // w
          case 11: cout << "Input: "; cin >> registers[reg_dest]; break;    // r
          case 12: stopped = 1; break;                               // stp
      }

      printf("registers: %d %d %d %d\n", registers[0], registers[1], registers[2], registers[3]);
      pc++;
      sleep(1);
  }
}


int main() {
  instruction instruction_memory[INSTRUCTION_MEMORY_SIZE];
  int data_memory[DATA_MEMORY_SIZE] = {0};
  reset_instruction_memory(instruction_memory);
  data_memory[100] = 20; 
  data_memory[101] = 5;  
  int op1[] = {1, 100};
  put(instruction_memory, 0, 4, 2, op1);
  int op2[] = {2, 101};
  put(instruction_memory, 1, 4, 2, op2);
  int op3[] = {0, 1, 2};
  put(instruction_memory, 2, 0, 3, op3);
  int op4[] = {3, 1, 2};
  put(instruction_memory, 3, 1, 3, op4);
  int op5[] = {1, 0, 2};
  put(instruction_memory, 4, 2, 3, op5);
  int op6[] = {2, 1, 3};
  put(instruction_memory, 5, 3, 3, op6);
  int op7[] = {0};
  put(instruction_memory, 6, 10, 1, op7);
  int op8[] = {1};
  put(instruction_memory, 7, 10, 1, op8);
  int op9[] = {2};
  int op10[] = {3};
  put(instruction_memory, 9, 10, 1, op10);
  put(instruction_memory, 10, 12, 0, NULL);
  exec(instruction_memory, data_memory);

  return 0;
}
