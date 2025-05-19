#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>

struct VirtualMachine
{
    char registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool OF, UF, CF, ZF;
    int PC = 0;
    std::vector<char> memoryAddresses;
};

std::string instructionSet[15] = {
    "INPUT", "DISPLAY", "MOV", "ADD", "SUB", "MUL", "DIV",
    "INC", "DEC", "ROL", "ROR", "SHL", "SHR", "LOAD", "STORE"};

std::stringstream memoryItem;

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

void outputToFile(VirtualMachine &vm)
{
    std::ostringstream registerText;
    std::ostringstream memoryText;

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

    std::ostringstream flagText;
    flagText << "Flags    : ";
    flagText
        << vm.OF << "   "
        << vm.UF << "   "
        << vm.CF << "   "
        << vm.ZF << "#";

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
    vm.registers[regNum[1]] = res[0];
}

void rotateLeft(VirtualMachine &vm, std::string regNum, int rotateAmount)
{
    vm.registers[regNum[1]] = (vm.registers[regNum[1]] << rotateAmount) | (vm.registers[regNum[1]] >> (8 - rotateAmount));
}

void rotateRight(VirtualMachine &vm, std::string regNum, int rotateAmount)
{
    vm.registers[regNum[1]] = (vm.registers[regNum[1]] >> rotateAmount) | (vm.registers[regNum[1]] << (8 - rotateAmount));
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
    std::stringstream ss;
    std::string word;
    int count;
    while (getline(assemblyProgram, instruction))
    {
        count = 0;
        vm.PC++;
        ss << instruction;
        getline(ss, word, ' ');
        command.push_back(word);
        word = "";
        getline(ss, word, ',');
        command.push_back(word);
        word = "";
        getline(ss, word, ',');
        command.push_back(word);
        word = "";
        // change to stringstream?
        // for (char ins : instruction)
        // {
        //     if (ins != ' ' && ins != ',')
        //     {
        //         tempBuffer += ins;
        //     }
        //     else
        //     {
        //         if (tempBuffer != "")
        //         {
        //             if (instructionSet.count(tempBuffer))
        //             {
        //                 if (++count == 2)
        //                 {
        //                     std::cout << "Error! Two or more instructions found at line " << vm.PC << "!" << std::endl;
        //                     exit(-1);
        //                 }
        //             }
        //             command.push_back(tempBuffer);
        //             tempBuffer = "";
        //         }
        //     }
        // }
        // if (!tempBuffer.empty())
        // {
        //     if (instructionSet.count(tempBuffer))
        //     {
        //         if (++count == 2)
        //         {
        //             std::cout << "Error! Two or more instructions found at line " << vm.PC << "!" << std::endl;
        //             exit(-1);
        //         }
        //     }
        //     command.push_back(tempBuffer);
        //     tempBuffer = "";
        // }
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