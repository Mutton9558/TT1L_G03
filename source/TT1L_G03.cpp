#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include "virtualmachines.h"

using namespace std;

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

string registerOutput(VirtualMachine &vm)
{
    ostringstream registerText;
    registerText << "Registers: ";
    for (int i = 0; i < 8; i++)
    {

        int item = static_cast<int>(vm.registers[i]);
        if (item < 0)
        {
            registerText << setw(4) << item;
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

string memoryOutput(VirtualMachine &vm)
{
    ostringstream memoryText;
    memoryText << "Memory   : \n";
    for (int i = 0; i < 64; i++)
    {
        int item = static_cast<int>(vm.memoryAddresses[i]);
        if (item < 0)
        {
            memoryText << setw(4) << item << "  ";
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

void outputToFile(VirtualMachine &vm)
{
    ofstream fileOutput;
    string registerText = registerOutput(vm);

    ostringstream flagText;
    flagText << "Flags    : ";
    flagText
        << vm.OF << "   "
        << vm.UF << "   "
        << vm.CF << "   "
        << vm.ZF << "#";

    ostringstream pcText;
    pcText << "PC       : " << static_cast<int>(vm.PC);

    string memoryText = memoryOutput(vm);

    fileOutput.open("output.txt");
    if (!fileOutput.is_open())
    {
        cout << "Error opening file output.txt" << endl;
        exit(-1);
    }

    fileOutput << registerText << endl;
    fileOutput << flagText.str() << endl;
    fileOutput << pcText.str() << endl
               << endl;
    fileOutput << memoryText << endl;

    fileOutput.close();
}

void runInstruction(VirtualMachine &vm, vector<string> command)
{
    const string cmd = command[0];
    if (cmd == "INPUT")
        input(vm, command);
    else if (cmd == "DISPLAY")
        display(vm, command);
    else if (cmd == "MOV")
        mov(vm, command);
    else if (cmd == "ADD")
        add(vm, command);
    else if (cmd == "SUB")
        sub(vm, command);
    else if (cmd == "MUL")
        mul(vm, command);
    else if (cmd == "DIV")
        div(vm, command);
    else if (cmd == "INC")
        inc(vm, command);
    else if (cmd == "DEC")
        dec(vm, command);
    else if (cmd == "ROL")
        rol(vm, command);
    else if (cmd == "ROR")
        rol(vm, command);
    else if (cmd == "SHL")
        shl(vm, command);
    else if (cmd == "SHR")
        shr(vm, command);
    else if (cmd == "LOAD")
        load(vm, command);
    else if (cmd == "STORE")
        store(vm, command);
    else
    {
        cout << "Invalid instruction name " << cmd << " at line " << static_cast<int>(vm.PC) << "!" << endl;
        exit(-1);
    }
}

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

void runner()
{
    VirtualMachine vm;
    initVM(vm);
    ifstream assemblyProgram;
    string filename;
    do
    {
        cout << "Type the name of a file" << std::endl;
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

int main()
{
    runner();
    return 0;
}