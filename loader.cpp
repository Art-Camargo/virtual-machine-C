#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;


#define INSTRUCTION_MEMORY_SIZE 320
#define DATA_MEMORY_SIZE 256

typedef struct {
    int opcode;
    int operands[3];
    int operand_count;
} instruction;

string trim_left(const string& str) {
  const string pattern = " \f\n\r\t\v";
  return str.substr(str.find_first_not_of(pattern));
}

string trim_right(const string& str) {
  const string pattern = " \f\n\r\t\v";
  return str.substr(0,str.find_last_not_of(pattern) + 1);
}

string trim(const string& str) {
  return trim_left(trim_right(str));
}

string to_upper(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

int find_opcode(const string mnemonic) {
    if (mnemonic == "ADD") return 0;
    if (mnemonic == "SUB") return 1;
    if (mnemonic == "MUL") return 2;
    if (mnemonic == "DIV") return 3;
    if (mnemonic == "MV") return 4;
    if (mnemonic == "ST") return 5;
    if (mnemonic == "JMP") return 6;
    if (mnemonic == "JEQ") return 7;
    if (mnemonic == "JGT") return 8;
    if (mnemonic == "JLT") return 9;
    if (mnemonic == "W") return 10;
    if (mnemonic == "R") return 11;
    if (mnemonic == "STP") return 12;
    return -1; // Invalid opcode
}

void reset_instruction_memory(instruction memory[]) {
    for (int i = 0; i < INSTRUCTION_MEMORY_SIZE; i++) {
        memory[i].opcode = -1;
        memory[i].operand_count = 0;
        for (int j = 0; j < 3; j++) {
            memory[i].operands[j] = 0;
        }
    }
}

void reset_data_memory(int memory[]) {
    memory = {0};
} 



int get_opcode(instruction memory[], int pc) {
    return memory[pc].opcode;
}


void put(instruction memory[], int position, int opcode, int operand_count, int operands[]) {
    memory[position].opcode = opcode;
    memory[position].operand_count = operand_count;
    for (int i = 0; i < operand_count; i++) {
        memory[position].operands[i] = operands[i];
    }
}

void mock_mem_data(int data_memory[]) {
    data_memory[100] = 5;
    data_memory[101] = 10;
    data_memory[102] = 20;
}

int get_register_value(string register_name) {
    if (register_name == "R0") return 0;
    if (register_name == "R1") return 1;
    if (register_name == "R2") return 2;
    if (register_name == "R3") return 3;
    return stoi(register_name);
}

void load_program(const char *filename, instruction memory[], int data_memory[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    int position = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        string line(buffer);
        line = trim(line);
        if (line.empty()) continue;
        string mnemonic = line.substr(0, line.find(' '));
        
        mnemonic = trim(mnemonic);
        mnemonic = to_upper(mnemonic);
        int opcode = find_opcode(mnemonic);
        if (opcode == -1) {
            printf("Error: Invalid opcode '%s'\n", mnemonic.c_str());
            exit(EXIT_FAILURE);
        }
        line = line.substr(line.find(' ') + 1);
        line = trim(line);
        int operands[3] = {0};
        int operand_count = 0;
        while (!line.empty() && operand_count < 3) {
            string operand = line.substr(0, line.find(' '));
            operand = trim(operand);
            if (operand.empty()) break;
            int op = get_register_value(operand);
            if (op == -1) {
                printf("Error: Invalid operand '%s'\n", operand.c_str());
                exit(EXIT_FAILURE);
            }
            operands[operand_count++] = op;
            line = line.substr(line.find(' ') + 1);
            line = trim(line);
        }
        put(memory, position++, opcode, operand_count, operands);
    }
    mock_mem_data(data_memory);
    fclose(file);
}

