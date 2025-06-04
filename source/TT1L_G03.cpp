#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include "virtualmachines.h"

using namespace std;

stringstream memoryItem;

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
        memoryItem.str("");
        memoryItem.clear();
        int item = static_cast<int>(vm.registers[i]);
        if (item < 0)
        {
            memoryItem << item;
        }
        else
        {
            memoryItem << uppercase << setfill('0') << setw(3) << item;
        }
        registerText << memoryItem.str();
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
        memoryItem.str("");
        memoryItem.clear();
        int item = static_cast<int>(vm.memoryAddresses[i]);
        if (item < 0)
        {
            memoryItem << item;
        }
        else
        {
            memoryItem << uppercase << setfill('0') << setw(3) << item;
        }

        memoryText << memoryItem.str() << "  ";

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
    pcText << "PC       : " << unsigned(vm.PC);

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
    // temporary for testing purposes
    std::cout << registerText << std::endl;
    std::cout << flagText.str() << std::endl;
    std::cout << pcText.str() << std::endl;
    std::cout << memoryText << std::endl;
}

int main()
{
    VirtualMachine vm;

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
        command.clear();
        int count = 0;
        vm.PC++;
        istringstream ss(instruction);
        string line;
        while (ss >> line)
        {
            if (!line.empty() && line.back() == ',')
            {
                line.pop_back();
            }

            command.push_back(line);
        }
        for (int i = 0; i < command.size(); i++)
        {
            if (searchForInstruction(command[i]))
            {
                if (++count >= 2)
                {
                    cout << "Error! Two or more instructions found at line " << unsigned(vm.PC) << "!" << endl;
                    exit(-1);
                }
            }
        }

        // change to switch case later
        if (command[0] == "INPUT")
        {
            input(vm, command);
        }
        else if (command[0] == "DISPLAY")
        {
            display(vm, command);
        }
        else if (command[0] == "MOV")
        {
            mov(vm, command);
        }
        else if (command[0] == "ADD")
        {
            add(vm, command);
        }
        else if (command[0] == "SUB")
        {
            sub(vm, command);
        }
        else if (command[0] == "MUL")
        {
            mul(vm, command);
        }
        else if (command[0] == "DIV")
        {
            div(vm, command);
        }
        else if (command[0] == "INC")
        {
            inc(vm, command);
        }
        else if (command[0] == "DEC")
        {
            dec(vm, command);
        }
        else if (command[0] == "ROL")
        {
            rol(vm, command);
        }
        else if (command[0] == "ROR")
        {
            rol(vm, command);
        }
        else if (command[0] == "SHL")
        {
            shl(vm, command);
        }
        else if (command[0] == "SHR")
        {
            shr(vm, command);
        }
        else if (command[0] == "LOAD")
        {
            load(vm, command);
        }
        else if (command[0] == "STORE")
        {
            store(vm, command);
        }
        else
        {
            cout << "Invalid instruction name " << command[0] << " at line " << unsigned(vm.PC) << "!" << endl;
            exit(-1);
        }
    }
    assemblyProgram.close();
    vm.PC++;
    outputToFile(vm);
    return 0;
}