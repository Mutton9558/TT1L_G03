#include <iostream>
#include <vector>

using namespace std;

#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

struct VirtualMachine
{
    char registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool OF = false;
    bool UF = false;
    bool ZF = false;
    bool CF = false;
    int PC = 0;
    vector<char> memoryAddresses;
};

#endif

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
    if (command[1].size() != 2)
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
    if (command[1].size() != 2)
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

    if (command[1].size() != 2)
    {
        cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
        exit(-1);
    }

    int rotateAmount = stoi(command[2]);
    char num = command[1][1] - '0';
    vm.registers[num] = (vm.registers[num] >> rotateAmount) | (vm.registers[num] << (8 - rotateAmount));
}