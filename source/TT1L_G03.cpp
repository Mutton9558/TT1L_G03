#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include "virtualmachines.h"

using namespace std;

// This function uses binary search to find the instruction (word) from a list of sorted instructions
// That way it can determine if the user entered a valid instruction
bool searchForInstruction(string word)
{
    const string instructionSet[15] = {
        "ADD", "DEC", "DISPLAY", "DIV", "INC", "INPUT", "LOAD", "MOV",
        "MUL", "ROL", "ROR", "SHL", "SHR", "STORE", "SUB"};
    int left = 0, right = 14;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (instructionSet[mid] == word)
        {
            return true;
        }
        else if (instructionSet[mid] < word)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return false;
}

// Format output for register contents
string registerOutput(VirtualMachine &vm)
{
    ostringstream registerText;
    registerText << "Registers: ";
    for (int i = 0; i < 8; i++)
    {

        int item = static_cast<int>(vm.registers[i]);
        if (item < 0)
        {
            registerText << setfill(' ') << setw(4) << item;
        }
        else
        {
            registerText << setfill('0') << setw(3) << item;
        }
        if (i == 7)
        {
            registerText << "#";
        }
        else
        {
            registerText << "  ";
        }
    }
    return registerText.str();
}

// Format output for memory contents
string memoryOutput(VirtualMachine &vm)
{
    ostringstream memoryText;
    memoryText << "Memory   : \n";
    for (int i = 0; i < 64; i++)
    {
        int item = static_cast<int>(vm.memoryAddresses[i]);
        if (item < 0)
        {
            memoryText << setfill(' ') << setw(4) << item << "  ";
        }
        else
        {
            memoryText << setfill('0') << setw(3) << item << "  ";
        }

        if ((i + 1) % 8 == 0)
            memoryText << "\n";
    }
    memoryText << "#";
    return memoryText.str();
}

// This function formats the output and puts it to a file and to the screen (terminal)
void outputToFile(VirtualMachine &vm)
{
    ofstream fileOutput;
    string registerText = registerOutput(vm);
    ostringstream flagText;
    flagText << "Flags    : ";
    flagText
        << vm.CF << "   "
        << vm.OF << "   "
        << vm.UF << "   "
        << vm.ZF << "#";
    ostringstream pcText;
    pcText << "PC       : " << static_cast<int>(vm.PC);
    string memoryText = memoryOutput(vm);
    fileOutput.open("output.txt");
    fileOutput << registerText << endl;
    fileOutput << flagText.str() << endl;
    fileOutput << pcText.str() << endl
               << endl;
    fileOutput << memoryText << endl;
    cout << registerText << endl;
    cout << flagText.str() << endl;
    cout << pcText.str() << endl
         << endl;
    cout << memoryText << endl;
    cout << "Result stored in output.txt" << endl;
    fileOutput.close();
}

// Checks if the command is INPUT or DISPLAY
bool IOInstructions(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (cmd == "INPUT")
        input(vm, command);
    else if (cmd == "DISPLAY")
        display(vm, command);
    else
        return false;
    return true;
}

// Checks if the command is an Arithmetic command (ADD, SUB, MUL, DIV)
bool arithmeticOperations(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (cmd == "ADD")
        add(vm, command);
    else if (cmd == "SUB")
        sub(vm, command);
    else if (cmd == "MUL")
        mul(vm, command);
    else if (cmd == "DIV")
        div(vm, command);
    else
        return false;
    return true;
}

// Checks if the command is either INC (increment) or DEC (decrement)
bool incrementOrDecrementOperations(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (cmd == "DEC")
        dec(vm, command);
    else if (cmd == "INC")
        inc(vm, command);
    else
        return false;
    return true;
}

// Checks if the command is either SHL, SHR, ROR or ROL
bool rotateOrShiftOperations(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (cmd == "ROL")
        rol(vm, command);
    else if (cmd == "ROR")
        ror(vm, command);
    else if (cmd == "SHL")
        shl(vm, command);
    else if (cmd == "SHR")
        shr(vm, command);
    else
        return false;
    return true;
}

// Checks if command is either LOAD or STORE
bool loadOrStoreOperations(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (cmd == "LOAD")
        load(vm, command);
    else if (cmd == "STORE")
        store(vm, command);
    else
        return false;
    return true;
}

// Runs command checking and runs their functions
void runInstruction(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (IOInstructions(vm, command))
        return;
    else if (cmd == "MOV")
    {
        mov(vm, command);
        return;
    }
    else if (arithmeticOperations(vm, command))
        return;
    else if (incrementOrDecrementOperations(vm, command))
        return;
    else if (rotateOrShiftOperations(vm, command))
        return;
    else if (loadOrStoreOperations(vm, command))
        return;
    else
    {
        cout << "Invalid instruction " << cmd << "at line " << static_cast<int>(vm.PC) << endl;
        exit(-1);
    }
}

// Splits commands into "tokens" and from tokens, carries out the functions (format: opcode source destination)
void getInstruction(VirtualMachine &vm, vector<string> &command, string instruction)
{
    command.clear();
    int count = 0;
    vm.PC++;
    istringstream ss(instruction);
    string line;
    while (ss >> line)
    {
        if (!line.empty() && line.back() == ',')
            line.pop_back();
        command.push_back(line);
    }
    for (int i = 0; i < command.size(); i++)
    {
        if (searchForInstruction(command[i]))
        {
            if (++count >= 2)
            {
                cout << "Error! Two or more instructions found at line " << static_cast<int>(vm.PC) << "!" << endl;
                exit(-1);
            }
        }
    }
    runInstruction(vm, command);
}

// The main runner function
// It initialize the virtual machine, then checks for a file and opens it.
// It will read each line of instruction, decode and run the specified instructions
void runner()
{
    VirtualMachine vm;
    initVM(vm);
    ifstream assemblyProgram;
    string filename;
    do
    {
        cout << "Type the name of a file (must include .asm extension)" << std::endl;
        cin >> filename;
        assemblyProgram.open(filename);
        if (!assemblyProgram.is_open())
        {
            cout << "Error! No file named " << filename << " was found!" << endl;
        }
    } while (!assemblyProgram.is_open());

    string instruction;
    vector<string> command;
    while (getline(assemblyProgram, instruction))
    {
        getInstruction(vm, command, instruction);
    }
    assemblyProgram.close();
    vm.PC++;
    outputToFile(vm);
}

// main function that calls the runner
int main()
{
    runner();
    return 0;
}