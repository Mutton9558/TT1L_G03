#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include "virtualmachines.h"

const std::string instructionSet[15] = {
    "ADD", "DEC", "DISPLAY", "DIV", "INC", "INPUT", "LOAD", "MOV",
    "MUL", "ROL", "ROR", "SHL", "SHR", "STORE", "SUB"};

std::stringstream memoryItem;

bool searchForInstruction(std::string word)
{
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

std::string registerOutput(VirtualMachine &vm)
{
    std::ostringstream registerText;
    registerText << "Registers: ";
    for (int i = 0; i < 8; i++)
    {
        memoryItem.str("");
        memoryItem.clear();
        memoryItem << std::uppercase << std::setfill('0') << std::setw(3) << static_cast<int>(vm.registers[i]);
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

std::string memoryOutput(VirtualMachine &vm)
{
    std::ostringstream memoryText;
    memoryText << "Memory   : \n";
    for (int i = 0; i < 64; i++)
    {
        memoryItem.str("");
        memoryItem.clear();
        memoryItem << std::uppercase << std::setfill('0') << std::setw(3) << static_cast<int>(vm.memoryAddresses[i]);
        memoryText << memoryItem.str() << "  ";

        if ((i + 1) % 8 == 0)
            memoryText << "\n";
    }
    memoryText << "#";
    return memoryText.str();
}

void outputToFile(VirtualMachine &vm)
{
    std::string registerText = registerOutput(vm);

    std::ostringstream flagText;
    flagText << "Flags    : ";
    flagText
        << vm.OF << "   "
        << vm.UF << "   "
        << vm.CF << "   "
        << vm.ZF << "#";

    std::ostringstream pcText;
    pcText << "PC       : " << vm.PC;

    std::string memoryText = memoryOutput(vm);
    // temporary for testing purposes
    std::cout << registerText << std::endl;
    std::cout << flagText.str() << std::endl;
    std::cout << pcText.str() << std::endl;
    std::cout << memoryText << std::endl;
}

int main()
{
    VirtualMachine vm;
    // initialize memory
    for (int i = 0; i < 64; i++)
    {
        vm.memoryAddresses.push_back(0);
    }
    std::ifstream assemblyProgram;
    std::string filename;
    do
    {
        std::cout << "Type the name of a file" << std::endl;
        std::cin >> filename;
        assemblyProgram.open(filename);
        if (!assemblyProgram.is_open())
        {
            std::cout << "Error! No file named " << filename << " was found!" << std::endl;
        }
    } while (!assemblyProgram.is_open());

    std::string instruction;
    std::vector<std::string> command;
    std::stringstream ss;
    std::string word;
    int count;
    while (getline(assemblyProgram, instruction))
    {
        command.clear();
        count = 0;
        vm.PC++;
        ss = std::stringstream(instruction);
        if (std::getline(ss, word, ' ') && !word.empty())
            command.push_back(word);

        if (std::getline(ss, word, ',') && !word.empty())
            command.push_back(word);

        if (std::getline(ss, word) && !word.empty())
            command.push_back(word);
        for (int i = 0; i < command.size(); i++)
        {
            if (searchForInstruction(command[i]))
            {
                if (++count == 2)
                {
                    std::cout << "Error! Two or more instructions found at line " << vm.PC << "!" << std::endl;
                }
            }
        }

        // change to switch case later
        // change for invalid reg num
        if (command[0] == "INPUT")
        {
            if (command.size() != 2)
            {
                std::cout << "Invalid length for command INPUT at line " << vm.PC << std::endl;
                exit(-1);
            }
            else
            {
                input(command[1], vm);
            }
        }
        else if (command[0] == "ROL")
        {
            if (command.size() != 3)
            {
                std::cout << "Invalid length for command ROL at line " << vm.PC << std::endl;
                exit(-1);
            }
            else
            {
                rol(vm, command[1], std::stoi(command[2]));
            }
        }
        else if (command[0] == "ROR")
        {
            if (command.size() != 3)
            {
                std::cout << "Invalid length for command ROR at line " << vm.PC << std::endl;
                exit(-1);
            }
            else
            {
                ror(vm, command[1], std::stoi(command[2]));
            }
        }
    }
    assemblyProgram.close();
    vm.PC++;
    outputToFile(vm);
    return 0;
}