
#include <iostream>
#include <vector>
#include <string>

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
    char memoryAddresses[64] = {0};
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

// check if value is from register or memory address
void getSrcValue(string s, VirtualMachine &vm)
{
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

    int rotateAmount = stoi(command[2]);
    char num = command[1][1] - '0';
    vm.registers[num] = (vm.registers[num] >> rotateAmount) | (vm.registers[num] << (8 - rotateAmount));
}

//===============================================================//

void display(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 2)
    {
        cout << "Invalid length for command DISPLAY at line " << vm.PC << endl;
        exit(-1);
    }
    if (command[1].size() != 2 || !isdigit(command[1][1]))
    {
        cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
        exit(-1);
    }

    //  int Regindex = command[1][1] - '0';
    int Regindex = command[1][1] - '0';

    cout << vm.registers[Regindex] << endl;
}

void mov(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 3)
    {
        cout << "Invalid length for command MOV at line " << vm.PC << endl;
        exit(-1);
    }

    for (int i = 1; i <= 2; i++)
    {
        if (command[i].size() != 2 || !isdigit(command[i][1]))
        {
            cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
            exit(-1);
        }
    }

    string first = command[1];
    string second = command[2];

    // checks if it is in the format of MOV number, register
    if (isNumber(first) == true && isRegister(second) == true)
    {

        int number1 = stoi(command[1]);
        int reg2 = command[2][1] - '0';

        vm.registers[reg2] = static_cast<char>(number1 & 0xFF);

        checkByteRange(static_cast<int>(vm.registers[reg2]), vm);
    }

    // checks if its in the format of MOV register, register
    else if (isRegister(first) == true && isRegister(second) == true)
    {
        int reg1 = command[1][1] - '0';
        int reg2 = command[2][1] - '0';

        vm.registers[reg2] = static_cast<char>(vm.registers[reg1] & 0xFF);

        checkByteRange(static_cast<int>(vm.registers[reg2]), vm);

    }

    // checks if its in the format of MOV memory, register
    else if (isMemory(first) == true && isRegister(second) == true)
    {

    }

    else
    {
        cout << "Invalid MOV syntax at line " << vm.PC << endl;
        exit(-1);
    }
}

// functions for mov //
// checks if there are registers or not.

bool isRegister(const string checker)
{

    if (checker[0] == 'R' && isdigit(checker[1]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// checks if it is a number
bool isNumber(const string checker)
{

    if (isdigit(checker[0]) && isdigit(checker[1]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// checks if is for memory access
bool isMemory(const string checker)
{

    if (checker[0] == '[' && checker[1] == 'R' && isdigit(checker[2]) && checker[3] == ']')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void add(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 3)
    {
        cout << "Invalid length for command ADD at line " << vm.PC << endl;
        exit(-1);
    }

    for (int i = 1; i <= 2; i++)
    {
        if (command[1].size() != 2 || !isdigit(command[1][1]))
        {
            cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
            exit(-1);
        }
    }

    int reg1 = command[1][1] - '0';
    int reg2 = command[2][1] - '0';

    vm.registers[reg1] = static_cast<char>((vm.registers[reg1] - '0') + (vm.registers[reg2] - '0') & 0xFF);

    checkByteRange(static_cast<int>(vm.registers[reg1]), vm);
    // vm.registers[command[1][1] - '0'] = res[0];
}

void sub(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 3)
    {
        cout << "Invalid length for command SUB at line " << vm.PC << endl;
        exit(-1);
    }

    for (int i = 1; i <= 2; i++)
    {
        if (command[1].size() != 2 || !isdigit(command[1][1]))
        {

            cout << "Invalid register number " << command[1] << "at line " << vm.PC << endl;
            exit(-1);
        }
    }

    int reg1 = command[1][1] - '0';
    int reg2 = command[2][1] - '0';

    vm.registers[reg1] = static_cast<char>((vm.registers[reg1] - '0') - (vm.registers[reg2] - '0') & 0xFF);

    checkByteRange(static_cast<int>(vm.registers[reg1]), vm);
}

