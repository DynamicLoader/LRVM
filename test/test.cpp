#define DEBUG_BINARY_FILE0 "E:/lrvm/test/data/test"

#include <stdio.h>
#include <stdlib.h>
#include "../lrvm.h"
using namespace LRVM;

int main()
{
    FILE* f = fopen(DEBUG_BINARY_FILE0, "rb");
    if (!f)
        return printf("Error: can not open %s", DEBUG_BINARY_FILE0), -1;
    uint8_t* memory = new uint8_t[1048576]; // Allocate memory for the VM
    printf("TestFile:%s,read %i bytes", DEBUG_BINARY_FILE0, (int)fread(memory, 1, 1048576, f));
    fclose(f);
    VM vm;
    vm.coreAllocMemory(0, (void*)memory, 1048576);
    vm.coreLoadCode(0, memory); // All code is loaded to the memory,so we pass through the address of memory.
    vm.coreSetPC(0, 0x30 >> 2);
    vm.coreSetErrorHandler(0,[](LRVM_ExecutorArgsDef){
        printf("\nError at %x !",vm._pc << 2);
        exit(-1);
    });
    vm.coreEnable(0, true);

    while (1)
        vm.run();
    return 0;
}
