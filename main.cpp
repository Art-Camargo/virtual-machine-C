#include <iostream>
#include <string>
#include "vm.cpp"

using namespace std;

#define MEMORY_SIZE  320

int main() {
    instruction memory[MEMORY_SIZE];
    reset_memory(memory);

    int operands[3] = {0, 1, 2};
    put(memory, 0, 0, 3, operands);  
    put(memory, 1, 1, 3, operands);  
    put(memory, 2, 2, 3, operands);  
    put(memory, 3, 3, 3, operands);  
    put(memory, 4, 4, 2, operands); 
    put(memory, 5, 5, 2, operands);  
    put(memory, 10, 10, 1, operands); 
    put(memory, 11, 11, 1, operands); 
    put(memory, 12, 12, 0, operands); 
    put(memory, 13, 0, 3, operands); 


    exec(memory);
    return 0;
}
