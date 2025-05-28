#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <fstream>
#include <vector>
#include <unordered_set>

struct VirtualMachine
{
    std::map<std::string, char> registers = {{"R0", 0}, {"R1", 0}, {"R2", 0}, {"R3", 0}, {"R4", 0}, {"R5", 0}, {"R6", 0}, {"R7", 0}};
    std::map<std::string, bool> flags = {{"OF", 0}, {"UF", 0}, {"CF", 0}, {"ZF", 0}};
    int PC = 0;
    std::map<int, char> memoryAddresses;
};

std::unordered_set<std::string> instructionSet = {
    "INPUT", "DISPLAY", "MOV", "ADD", "SUB", "MUL", "DIV",
    "INC", "DEC", "ROL", "ROR", "SHL", "SHR", "LOAD", "STORE"};

std::stringstream memoryItem;

void checkByteRange(int x, VirtualMachine &vm)
{
    if (x > 127)
    {
        vm.flags["OF"] = 1;
        x -= 127;
    }
    else if (x < -128)
    {
        vm.flags["UF"] = 1;
        x += 128;
    }
    else if (x == 0)
    {
        vm.flags["ZF"] = 1;
    }
}

void outputToFile(VirtualMachine &vm)
{
    std::ostringstream registerText;
    std::ostringstream memoryText;

    registerText << "Registers: ";
    for (const auto &regPair : vm.registers)
    {
        memoryItem.str("");
        memoryItem.clear();
        memoryItem << std::uppercase << std::setfill('0') << std::setw(3) << static_cast<int>(regPair.second);
        registerText << memoryItem.str();

        if (regPair.first == "R7")
        {
            registerText << "#";
        }
        else
        {
            registerText << "  ";
        }
    }

    std::ostringstream flagText;
    flagText << "Flags    : ";
    flagText
        << vm.flags["OF"] << "   "
        << vm.flags["UF"] << "   "
        << vm.flags["CF"] << "   "
        << vm.flags["ZF"] << "#";

    std::ostringstream pcText;
    pcText << "PC       : " << vm.PC;

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

    // temporary for testing purposes
    std::cout << registerText.str() << std::endl;
    std::cout << flagText.str() << std::endl;
    std::cout << pcText.str() << std::endl;
    std::cout << memoryText.str() << std::endl;
}

// Instructions
void INPUT(std::string regNum, VirtualMachine &vm)
{
    // subject to change
    std::string res;
    std::cout << "?";
    std::getline(std::cin, res);
    if (res.length() > 1)
    {
        std::cout << "Error! More than one digit/letter found!" << std::endl;
        exit(-1);
    }
    std::cout << std::endl;
    checkByteRange(static_cast<int>(res[0]), vm);
    vm.registers[regNum] = res[0];
}

void rotateLeft(VirtualMachine &vm, std::string regNum, int rotateAmount)
{
    vm.registers[regNum] = (vm.registers[regNum] << rotateAmount) | (vm.registers[regNum] >> (8 - rotateAmount));
}

void rotateRight(VirtualMachine &vm, std::string regNum, int rotateAmount)
{
    vm.registers[regNum] = (vm.registers[regNum] >> rotateAmount) | (vm.registers[regNum] << (8 - rotateAmount));
}

int main()
{
    VirtualMachine vm;

    // initialize memory
    for (int i = 0; i < 64; i++)
    {
        // addToMemory(i, 0);
        vm.memoryAddresses[i] = 0;
    }
    std::ifstream assemblyProgram;
    assemblyProgram.open("test1.asm");
    if (!assemblyProgram.is_open())
    {
        std::cout << "Error! File not found!" << std::endl;
        exit(-1);
    }
    std::string instruction;
    std::vector<std::string> command;
    std::string tempBuffer;
    int count;
    while (getline(assemblyProgram, instruction))
    {
        count = 0;
        vm.PC++;
        // change to stringstream?
        for (char ins : instruction)
        {
            if (ins != ' ' && ins != ',')
            {
                tempBuffer += ins;
            }
            else
            {
                if (tempBuffer != "")
                {
                    if (instructionSet.count(tempBuffer))
                    {
                        if (++count == 2)
                        {
                            std::cout << "Error! Two or more instructions found at line " << vm.PC << "!" << std::endl;
                            exit(-1);
                        }
                    }
                    command.push_back(tempBuffer);
                    tempBuffer = "";
                }
            }
        }
        if (!tempBuffer.empty())
        {
            if (instructionSet.count(tempBuffer))
            {
                if (++count == 2)
                {
                    std::cout << "Error! Two or more instructions found at line " << vm.PC << "!" << std::endl;
                    exit(-1);
                }
            }
            command.push_back(tempBuffer);
            tempBuffer = "";
        }
        // if statement here
        if (command[0] == "INPUT")
        {
            if (command.size() != 2)
            {
                std::cout << "Invalid length for command INPUT at line " << vm.PC << std::endl;
                exit(-1);
            }
            else
            {
                INPUT(command[1], vm);
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
                rotateLeft(vm, command[1], std::stoi(command[2]));
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
                rotateRight(vm, command[1], std::stoi(command[2]));
            }
        }
        command.clear();
    }
    assemblyProgram.close();
    vm.PC++;
    outputToFile(vm);
    return 0;
}