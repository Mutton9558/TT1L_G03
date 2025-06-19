#include <iostream>
#include <vector>

using namespace std;

bool isMemoryAccess = false;

struct VirtualMachine
{
    char registers[8];
    bool OF;
    bool UF;
    bool ZF;
    bool CF;
    unsigned char PC = 0;
    char memoryAddresses[64];
};

void initVM(VirtualMachine &vm)
{
    for (int i = 0; i < 8; i++)
    {
        vm.registers[i] = 0;
    }

    vm.OF = vm.UF = vm.ZF = vm.CF = 0;

    for (int i = 0; i < 64; i++)
    {
        vm.memoryAddresses[i] = 0;
    }
}

bool isNumber(const string checker)
{

    for (auto a : checker)
    {
        if (!isdigit(a))
        {
            return false;
        }
    }
    return true;
}

int getRegisterNumber(string name, bool &isMemoryAccess, VirtualMachine vm)
{
    if (name.length() == 2 && isdigit(name[1]) && name[0] == 'R')
    {
        if (name[1] - '0' < 0 || name[1] - '0' > 7)
        {
            cout << "Presence of Invalid Register Number at line " << static_cast<int>(vm.PC) << endl;
            exit(-1);
        }
        else
        {
            isMemoryAccess = false;
            return (static_cast<int>(name[1] - '0'));
        }
    }
    else
    {
        cout << "Invalid Register!" << endl;
    }
    return -1;
}

int getMemoryAddress(string name, bool &isMemoryAccess, VirtualMachine vm)
{
    if (name[1] == 'R' && name.length() == 4 && isdigit(name[2]) && name[0] == '[' && name[3] == ']')
    {
        if (name[2] - '0' < 0 || name[2] - '0' > 7)
        {
            cout << "Presence of Invalid Register Number at line " << static_cast<int>(vm.PC) << endl;
            exit(-1);
        }
        else
        {
            isMemoryAccess = true;
            return (vm.registers[name[2] - '0']);
        }
    }
    else if (name.length() > 2 && name.length() < 5 && name[0] == '[' && name[3] == ']')
    {
        string memoryLoc = name.substr(1, name.length() - 2);
        if (isNumber(memoryLoc))
        {
            int memoryAddr = stoi(memoryLoc);
            if (memoryAddr < 0 || memoryAddr > 63)
            {
                cout << "Presence of invalid memory address at line " << static_cast<int>(vm.PC) << endl;
            }
            else
            {
                isMemoryAccess = true;
                return (stoi(memoryLoc));
            }
        }
        else
        {
            cout << "No such memory location named " << memoryLoc << " at line " << static_cast<int>(vm.PC) << "!" << endl;
            exit(-1);
        }
    }
    return -1;
}

int getSource(string name, VirtualMachine vm)
{
    int src;
    if (name[0] == 'R')
    {
        src = getRegisterNumber(name, isMemoryAccess, vm);
        return src;
    }
    else if (name[0] == '[' && name[name.size() - 1] == ']')
    {
        src = getMemoryAddress(name, isMemoryAccess, vm);
        return src;
    }
    else
    {
        cout << "Invalid register or memory value at " << static_cast<int>(vm.PC) << "!" << endl;
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

void input(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 2)
    {
        cout << "Invalid length for command INPUT at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int num = getRegisterNumber(command[1], isMemoryAccess, vm);
    char res;
    cout << "?";
    cin >> res;
    checkByteRange(static_cast<int>(res), vm);
    vm.registers[num] = res;
}

void display(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 2)
    {
        cout << "Invalid length for command DISPLAY at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg = getRegisterNumber(command[1], isMemoryAccess, vm);

    cout << static_cast<int>(vm.registers[reg]) << endl;
}

void mov(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command MOV at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    string first = command[1];
    string second = command[2];
    int num2 = getRegisterNumber(second, isMemoryAccess, vm);
    if (isNumber(first))
    {
        int num1 = stoi(command[1]);
        vm.registers[num2] = num1;
    }
    else
    {
        int num1 = getSource(first, vm);
        if (isMemoryAccess)
        {
            vm.registers[num2] = vm.memoryAddresses[num1];
        }
        else
        {
            vm.registers[num2] = vm.registers[num1];
        }
    }
}

void add(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command ADD at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg1;
    if (isNumber(command[1]))
    {
        reg1 = stoi(command[1]);
    }
    else
    {
        reg1 = getRegisterNumber(command[1], isMemoryAccess, vm);
    }
    int reg2 = getRegisterNumber(command[2], isMemoryAccess, vm);

    vm.registers[reg2] = static_cast<int>(vm.registers[reg1]) + static_cast<int>(vm.registers[reg2]);

    checkByteRange(static_cast<int>(vm.registers[reg2]), vm);
    vm.CF = (static_cast<int>(vm.registers[reg2]) > 127);
}

void sub(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command SUB at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg1;
    if (isNumber(command[1]))
    {
        reg1 = stoi(command[1]);
    }
    else
    {
        reg1 = getRegisterNumber(command[1], isMemoryAccess, vm);
    }
    int reg2 = getRegisterNumber(command[2], isMemoryAccess, vm);
    int r1 = static_cast<int>(vm.registers[reg1]);
    int r2 = static_cast<int>(vm.registers[reg2]);
    bool isLower = (r2 < r1 && r1 > 0);
    vm.registers[reg2] = r2 - r1;
    r2 = static_cast<int>(vm.registers[reg2]);
    checkByteRange(r2, vm);
    vm.CF = (r2 > 127 || isLower);
}

void mul(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command MUL at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg1;
    if (isNumber(command[1]))
    {
        reg1 = stoi(command[1]);
    }
    else
    {
        reg1 = getRegisterNumber(command[1], isMemoryAccess, vm);
    }
    int reg2 = getRegisterNumber(command[2], isMemoryAccess, vm);

    int result = static_cast<int>(vm.registers[reg1]) * static_cast<int>(vm.registers[reg2]);
    checkByteRange(result, vm);
    vm.CF = (result > 127);
    vm.registers[reg2] = result;
}

void div(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command DIV at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg1;
    if (isNumber(command[1]))
    {
        reg1 = stoi(command[1]);
    }
    else
    {
        reg1 = getRegisterNumber(command[1], isMemoryAccess, vm);
    }
    int reg2 = getRegisterNumber(command[2], isMemoryAccess, vm);

    if (vm.registers[reg2] == 0)
    {
        cout << "Division by zero in DIV at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int result = static_cast<int>(vm.registers[reg1]) / static_cast<int>(vm.registers[reg2]);
    checkByteRange(result, vm);
    vm.CF = (result < -128);
    vm.registers[reg2] = static_cast<char>(result);
}

void inc(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 2)
    {
        cout << "Invalid length for command INC at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg = getRegisterNumber(command[1], isMemoryAccess, vm);
    int result = static_cast<int>(vm.registers[reg]);
    result++;
    checkByteRange(result, vm);
    vm.CF = (result > 127);
    vm.registers[reg] = static_cast<char>(result);
}

void dec(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 2)
    {
        cout << "Invalid length for command DEC at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    int reg = getRegisterNumber(command[1], isMemoryAccess, vm);
    int result = static_cast<int>(vm.registers[reg]);
    result--;
    checkByteRange(result, vm);
    vm.CF = (result < -128);
    vm.registers[reg] = static_cast<char>(result);
}

void rol(VirtualMachine &vm, vector<string> command)
{

    if (command.size() != 3)
    {
        cout << "Invalid length for command ROL at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    if (isNumber(command[2]))
    {
        int rotateAmount = stoi(command[2]);
        int num = getRegisterNumber(command[1], isMemoryAccess, vm);
        vm.registers[num] = (vm.registers[num] << rotateAmount) | (vm.registers[num] >> (8 - rotateAmount));
    }
    else
    {
        cout << "Rotate amount not a number at line " << static_cast<int>(vm.PC) << endl;
    }
}

void ror(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command ROR at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    if (isNumber(command[2]))
    {
        int rotateAmount = stoi(command[2]);
        int num = getRegisterNumber(command[1], isMemoryAccess, vm);
        vm.registers[num] = (vm.registers[num] >> rotateAmount) | (vm.registers[num] << (8 - rotateAmount));
    }
    else
    {
        cout << "Rotate amount not a number at line " << static_cast<int>(vm.PC) << endl;
    }
}

void shl(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command SHL at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    if (isNumber(command[2]))
    {
        int shiftCount = stoi(command[2]);
        int num = getRegisterNumber(command[1], isMemoryAccess, vm);
        if (shiftCount > 8)
            vm.CF = 0;
        else
            vm.CF = (vm.registers[num] >> (8 - shiftCount) & 1);
        vm.registers[num] = vm.registers[num] << shiftCount;
        checkByteRange(vm.registers[num], vm);
    }
    else
    {
        cout << "Shift amount is not a number at line " << static_cast<int>(vm.PC) << endl;
    }
}

void shr(VirtualMachine &vm, vector<string> command)
{
    if (command.size() != 3)
    {
        cout << "Invalid length for command SHL at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
    if (isNumber(command[2]))
    {
        int shiftCount = stoi(command[2]);
        int num = getRegisterNumber(command[1], isMemoryAccess, vm);
        if (shiftCount > 8)
            vm.CF = 0;
        else
            vm.CF = (vm.registers[num] << (8 - shiftCount) & 1);
        vm.registers[num] = vm.registers[num] >> shiftCount;
        checkByteRange(vm.registers[num], vm);
    }
    else
    {
        cout << "Shift amount is not a number at line " << static_cast<int>(vm.PC) << endl;
    }
}

void load(VirtualMachine &vm, vector<string> command)
{
    int num1;
    if (isNumber(command[1]))
    {
        num1 = stoi(command[1]);
        if (num1 < 0 || num1 > 63)
        {
            cout << "Presence of invalid memory address at line " << static_cast<int>(vm.PC) << "!" << endl;
            exit(-1);
        }
    }
    else
    {
        num1 = getMemoryAddress(command[1], isMemoryAccess, vm);
    }
    int num2 = getRegisterNumber(command[2], isMemoryAccess, vm);
    vm.registers[num2] = vm.memoryAddresses[num1];
}

void store(VirtualMachine &vm, vector<string> command)
{
    int num1 = getRegisterNumber(command[1], isMemoryAccess, vm);
    int num2;
    if (isNumber(command[2]))
    {
        num2 = stoi(command[2]);
        if (num2 < 0 || num2 > 63)
        {
            cout << "Presence of invalid memory address at line " << static_cast<int>(vm.PC) << "!" << endl;
            exit(-1);
        }
    }
    else
    {
        num2 = getMemoryAddress(command[2], isMemoryAccess, vm);
    }
    vm.memoryAddresses[num2] = vm.registers[num1];
}
