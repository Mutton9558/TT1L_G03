#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

bool isMemoryAccess = false;

#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

struct VirtualMachine
{
    char registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool OF = false;
    bool UF = false;
    bool ZF = false;
    bool CF = false;
    uint8_t PC = 0;
    char memoryAddresses[64] = {0};
};

#endif

int getRegisterNumber(string name, bool &isMemoryAccess)
{
    if (name.length() == 2 && isdigit(name[1]))
    {
        isMemoryAccess = false;
        return (static_cast<int>(name[1] - '0'));
    }
    return -1;
}

int getMemoryAddress(string name, bool &isMemoryAccess, VirtualMachine vm)
{
    if (name[1] == 'R' && name.length() == 4 && isdigit(name[2]))
    {
        isMemoryAccess = true;
        return (vm.registers[static_cast<int>(name[1])]);
    }
    else if (name.length() > 2 && name.length() < 5)
    {
        string memoryLoc = name.substr(1, name.length() - 2);
        try
        {
            isMemoryAccess = true;
            return (stoi(memoryLoc));
        }
        catch (...)
        {
            cout << "No such memory location named " << memoryLoc << " at location " << vm.PC << "!" << endl;
            exit(-1);
        }
    }
}

int getSource(string name, VirtualMachine vm)
{
    int src;
    if (name[0] == 'R')
    {
        src = getRegisterNumber(name, isMemoryAccess);
        return src;
    }
    else if (name[0] == '[' && name[-1] == ']')
    {
        src = getMemoryAddress(name, isMemoryAccess, vm);
        return src;
    }
    else
    {
        cout << "Invalid memory location at " << vm.PC << "!" << endl;
        exit(-1);
    }

    return -1;
}

void checkByteRange(int x, VirtualMachine &vm)
{
    if (x > 127)
    {
        vm.OF = 1;
    }
    else if (x < -128)
    {
        vm.UF = 1;
    }
    else if (x == 0)
    {
        vm.ZF = 1;
    }
}

void input(vector<string> command, VirtualMachine &vm)
{
    if (command.size() != 2)
    {
        cout << "Invalid length for command INPUT at line " << vm.PC << endl;
        exit(-1);
    }
    if (command[1].size() != 2 || !isdigit(command[1][1]))
    {
        cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
        exit(-1);
    }
    string res;
    do
    {
        cout << "?";
        cin >> res;
        cin.ignore(80, '\n');
        if (res.length() > 1)
        {
            cout << "Error! More than one digit/letter found!" << std::endl;
        }
    } while (res.length() > 1);
    checkByteRange(static_cast<int>(res[0]), vm);
    vm.registers[command[1][1] - '0'] = res[0];
}

void rol(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 3)
    {
        cout << "Invalid length for command ROL at line " << vm.PC << endl;
        exit(-1);
    }
    if (command[1].size() != 2 || !isdigit(command[1][1]))
    {
        cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
        exit(-1);
    }

    int rotateAmount = stoi(command[2]);

    char num = command[1][1] - '0';
    cout << vm.registers[num] << endl;
    vm.registers[num] = (vm.registers[num] << rotateAmount) | (vm.registers[num] >> (8 - rotateAmount));
}

void ror(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command ROR at line " << vm.PC << endl;
        exit(-1);
    }

    if (command[1].size() != 2 || !isdigit(command[1][1]))
    {
        cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
        exit(-1);
    }

    try
    {
        int rotateAmount = stoi(command[2]);
        std::cout << "Value: " << rotateAmount << std::endl;
        char num = command[1][1] - '0';
        vm.registers[num] = (vm.registers[num] >> rotateAmount) | (vm.registers[num] << (8 - rotateAmount));
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Invalid argument: not a number\n";
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Out of range: number too big\n";
    }
}

void shl(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command SHL at line " << vm.PC << endl;
        exit(-1);
    }

    if (command[1].size() != 2 || !isdigit(command[1][1]))
    {
        cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
        exit(-1);
    }
}

void mul(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command MUL at line " << vm.PC << endl;
        exit(-1);
    }
    int reg1 = getRegisterNumber(command[1], isMemoryAccess);
    int reg2 = getRegisterNumber(command[2], isMemoryAccess);
    if (reg1 == -1 || reg2 == -1)
    {
        cout << "Invalid register number in MUL at line " << vm.PC << endl;
        exit(-1);
    }
    int result = static_cast<int>(vm.registers[reg1]) * static_cast<int>(vm.registers[reg2]);
    checkByteRange(result, vm);
    vm.registers[reg1] = static_cast<char>(result);
}

void div(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command DIV at line " << vm.PC << endl;
        exit(-1);
    }
    int reg1 = getRegisterNumber(command[1], isMemoryAccess);
    int reg2 = getRegisterNumber(command[2], isMemoryAccess);
    if (reg1 == -1 || reg2 == -1)
    {
        cout << "Invalid register number in DIV at line " << vm.PC << endl;
        exit(-1);
    }
    if (vm.registers[reg2] == 0)
    {
        cout << "Division by zero in DIV at line " << vm.PC << endl;
        exit(-1);
    }
    int result = static_cast<int>(vm.registers[reg1]) / static_cast<int>(vm.registers[reg2]);
    checkByteRange(result, vm);
    vm.registers[reg1] = static_cast<char>(result);
}

void inc(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 2)
    {
        cout << "Invalid length for command INC at line " << vm.PC << endl;
        exit(-1);
    }
    int reg = getRegisterNumber(command[1], isMemoryAccess);
    if (reg == -1)
    {
        cout << "Invalid register number in INC at line " << vm.PC << endl;
        exit(-1);
    }
    int result = static_cast<int>(vm.registers[reg]) + 1;
    checkByteRange(result, vm);
    vm.registers[reg] = static_cast<char>(result);
}

void dec(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 2)
    {
        cout << "Invalid length for command DEC at line " << vm.PC << endl;
        exit(-1);
    }
    int reg = getRegisterNumber(command[1], isMemoryAccess);
    if (reg == -1)
    {
        cout << "Invalid register number in DEC at line " << vm.PC << endl;
        exit(-1);
    }
    int result = static_cast<int>(vm.registers[reg]) - 1;
    checkByteRange(result, vm);
    vm.registers[reg] = static_cast<char>(result);
}