#include <iostream>
#include <string>
#include <unistd.h> 


using namespace std;

#define MEMORY_SIZE 320

typedef struct {
    int opcode;
    int operands[3];
    int operand_count;
} instruction;

typedef void (*Operation)(int*, instruction*, int&, int&);

void add(int* reg, instruction* mem, int& pc, int& stop) {
    reg[mem[pc].operands[0]] = reg[mem[pc].operands[1]] + reg[mem[pc].operands[2]];
}

void sub(int* reg, instruction* mem, int& pc, int& stop) {
    reg[mem[pc].operands[0]] = reg[mem[pc].operands[1]] - reg[mem[pc].operands[2]];
}

void mul(int* reg, instruction* mem, int& pc, int& stop) {
    reg[mem[pc].operands[0]] = reg[mem[pc].operands[1]] * reg[mem[pc].operands[2]];
}

void divi(int* reg, instruction* mem, int& pc, int& stop) {
    if (reg[mem[pc].operands[2]] == 0) reg[mem[pc].operands[0]] = 0;
    else reg[mem[pc].operands[0]] = reg[mem[pc].operands[1]] / reg[mem[pc].operands[2]];
}

void mv(int* reg, instruction* mem, int& pc, int& stop) {
    reg[mem[pc].operands[0]] = mem[mem[pc].operands[1]].operands[0];
}

void st(int* reg, instruction* mem, int& pc, int& stop) {
    mem[mem[pc].operands[0]].operands[0] = reg[mem[pc].operands[1]];
}

void jmp(int* reg, instruction* mem, int& pc, int& stop) {
    pc = mem[pc].operands[0];
}

void jeq(int* reg, instruction* mem, int& pc, int& stop) {
    if (reg[mem[pc].operands[0]] == reg[mem[pc].operands[1]]) pc = mem[pc].operands[2];
}

void jgt(int* reg, instruction* mem, int& pc, int& stop) {
    if (reg[mem[pc].operands[0]] > reg[mem[pc].operands[1]]) pc = mem[pc].operands[2];
}

void jlt(int* reg, instruction* mem, int& pc, int& stop) {
    if (reg[mem[pc].operands[0]] < reg[mem[pc].operands[1]]) pc = mem[pc].operands[2];
}

void w(int* reg, instruction* mem, int& pc, int& stop) {
    cout << "Output: " << reg[mem[pc].operands[0]] << endl;
}

void r(int* reg, instruction* mem, int& pc, int& stop) {
    cout << "Input: ";
    cin >> reg[mem[pc].operands[0]];
}

void stp(int* reg, instruction* mem, int& pc, int& stop) {
    stop = 1;
}

Operation operations[] = {
    add, sub, mul, divi, mv, st, jmp,
    jeq, jgt, jlt, w, r, stp
};

string operation_names[] = {
    "add", "sub", "mul", "divi", "mv", "st", "jmp",
    "jeq", "jgt", "jlt", "w", "r", "stp"
};

void exec(instruction mem[]) {
    int pc = -1, stop = 0;
    int reg[4] = {0};

    while (pc < MEMORY_SIZE && !stop) {
      int current_pc = ++pc;
      int opcode = mem[current_pc].opcode;
  
      if (opcode == -1 || !(opcode >= 0 && opcode < (int)(sizeof(operations) / sizeof(Operation)))) continue;
  
      operations[opcode](reg, mem, current_pc, stop);
  
      printf("Operation Name: %s\n", operation_names[opcode].c_str());
      printf("Opcode: %d\n", opcode);
  
      if (mem[current_pc].operand_count > 0)
          printf("Register with response: %d\n", reg[mem[current_pc].operands[0]]);   
      else
          printf("No register response for this operation.\n");
  
      sleep(4); // Sleep for 4 seconds
  }
  
}

void reset_memory(instruction memory[]) {
    int i, j;
    for (i = 0; i < MEMORY_SIZE; i++) {
        memory[i].opcode = -1;
        memory[i].operand_count = 0;
        for (j = 0; j < 3; j++) {
            memory[i].operands[j] = 0;
        }
    }
}

void put(instruction memory[], int position, int opcode, int operand_count, int operands[]) {
    if (position < 0  || position >= MEMORY_SIZE) return;
    memory[position].opcode = opcode;
    memory[position].operand_count = operand_count;
    for (int i = 0; i < operand_count; i++) memory[position].operands[i] = operands[i];
}
