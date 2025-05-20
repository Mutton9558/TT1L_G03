#include <iostream>
#include <vector>

#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

struct VirtualMachine
{
    char registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool OF, UF, CF, ZF;
    int PC = 0;
    std::vector<char> memoryAddresses;
};

#endif

void checkByteRange(int x, VirtualMachine &vm)
{
    if (x > 127)
    {
        vm.OF = 1;
        x -= 127;
    }
    else if (x < -128)
    {
        vm.UF = 1;
        x += 128;
    }
    else if (x == 0)
    {
        vm.ZF = 1;
    }
}

void input(std::string regNum, VirtualMachine &vm)
{
    std::string res;
    std::cout << "?";
    std::cin.ignore(80, '\n');
    std::getline(std::cin, res);
    if (res.length() > 1)
    {
        std::cout << "Error! More than one digit/letter found!" << std::endl;
        exit(-1);
    }
    checkByteRange(static_cast<int>(res[0]), vm);
    vm.registers[regNum[1] - '0'] = res[0];
}

void rol(VirtualMachine &vm, std::string regNum, int rotateAmount)
{
    char num = regNum[1] - '0';
    std::cout << vm.registers[num] << std::endl;
    vm.registers[num] = (vm.registers[num] << rotateAmount) | (vm.registers[num] >> (8 - rotateAmount));
}

void ror(VirtualMachine &vm, std::string regNum, int rotateAmount)
{
    char num = regNum[1] - '0';
    vm.registers[num] = (vm.registers[num] >> rotateAmount) | (vm.registers[num] << (8 - rotateAmount));
}