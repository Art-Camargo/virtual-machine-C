// WE ARE CREATING A VIRTUAL MACHINE! 
// OPERATIONS, OPCODES, AND QUANTITY OF OPERANDS
// (add) a0 = a1 + a2 (00) (3 operands) 
// sub a0 = a1 - a2 01 3 
// mul a0 = a1 * a2 03 3 
// div a0 = a1 / a2 03 3
// mv ao = mem[op] 04 2
// st mem[op] = a0 05 2
// jmp jmp op 06 1 
// jeq jeq a0 a1 op 07 3
// jgt jgq a0 a1 op 08 3 
// jlt jlq a0 a1 op 09 3
// w Print [op] 10 1 
// r Read [op] 11 1
// stp Stop machine 12 0


#include <iostream>
#include <string>

using namespace std;
#define MEMORY_SIZE  320

typedef struct  {
  int opcode;
  int operands[3]; 
  int operand_count;
} instruction;


void reset_memory(instruction memory[]) {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    memory[i].opcode = -1; // -1 indicates an empty instruction
    for (int j = 0; j < 3; j++) {
      memory[i].operands[j] = 0;
    }
    memory[i].operand_count = 0;
  }
}

void put(instruction memory[], int pc, int opcode, int operand_count, int operands[]) {
  memory[pc].opcode = opcode;
  memory[pc].operand_count = operand_count;
  for (int i = 0; i < operand_count; i++) {
    memory[pc].operands[i] = operands[i];
  }
} 

int get_opcode(instruction memory[], int pc) {
  return memory[pc].opcode;
}


void exec(instruction mem[]) {
  int pc = 0, stopped = 0, opcode = -1;
  int registers[4] = {0};
  while(pc < MEMORY_SIZE && !stopped) {
    opcode = get_opcode(mem, pc);
    if (opcode == -1) {
      pc++;
      continue;
    }; 
    switch(opcode) {
      case 00: 
        registers[mem[pc].operands[0]] = registers[mem[pc].operands[1]] + registers[mem[pc].operands[2]];
        break;
      case 01: 
        registers[mem[pc].operands[0]] = registers[mem[pc].operands[1]] - registers[mem[pc].operands[2]];
        break;
      case 02: 
        registers[mem[pc].operands[0]] = registers[mem[pc].operands[1]] * registers[mem[pc].operands[2]];
        break;
      case 03: 
        if (registers[mem[pc].operands[2]] == 0) registers[mem[pc].operands[0]] = 0; // Avoid division by zero
        else registers[mem[pc].operands[0]] = registers[mem[pc].operands[1]] / registers[mem[pc].operands[2]];
        break;
      case 04: 
        registers[mem[pc].operands[0]] = mem[mem[pc].operands[1]].operands[0];
        break;
      case 05: 
        mem[mem[pc].operands[0]].operands[0] = registers[mem[pc].operands[1]];
        break;
      case 06: 
        pc = mem[pc].operands[0];
        break;
      case 07: 
        if (registers[mem[pc].operands[0]] == registers[mem[pc].operands[1]]) pc = mem[pc].operands[2];
        break;
      case 8: 
        if (registers[mem[pc].operands[0]] > registers[mem[pc].operands[1]]) pc = mem[pc].operands[2];
        break;
      case 9: 
        if (registers[mem[pc].operands[0]] < registers[mem[pc].operands[1]]) pc = mem[pc].operands[2];
        break;
      case 10: 
        cout << "Output: " << registers[mem[pc].operands[0]] << endl;
        break;
      case 11: 
        cout << "Input: ";
        cin >> registers[mem[pc].operands[0]];
        break;
      case 12: 
        stopped = 1;
        break;
      default:
        break; 
    }
    pc++;
  }

}


int main() {
  instruction memory[MEMORY_SIZE];
  reset_memory(memory);
  int operands[3] = {0, 1, 2};
  put(memory, 0, 0, 3, operands); // add a0 = a1 + a2
  put(memory, 1, 1, 3, operands); // sub a0 = a1 - a2
  put(memory, 2, 2, 3, operands); // mul a0 = a1 * a2
  put(memory, 3, 3, 3, operands); // div a0 = a1 / a2
  put(memory, 4, 4, 2, operands); // mv a0 = mem[op]
  put(memory, 5, 5, 2, operands); // st mem[op] = a0
  put(memory, 10, 10, 1, operands); // w Print [op]
  put(memory, 11, 11, 1, operands); // r Read [op]
  put(memory, 12, 12, 0, operands); // stp Stop machine
  put(memory, 13, 0, 3, operands); // add a0 = a1 + a2
  put(memory, 14, 12, 0, operands); // stp Stop machine
  exec(memory);
  return 0; 
}