#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <string>
#include <fstream>
#include <vector>

struct VirtualMachine
{
    std::map<int, char> registers = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}};
    std::map<std::string, bool> flags = {{"OF", 0}, {"UF", 0}, {"CF", 0}, {"ZF", 0}};
    int PC = 0;
    std::map<int, char> memoryAddresses;
};

// Memory locations
std::stringstream memoryItem;

// Registers
std::map<int, std::string> registers;

void rotateLeft(int &num, int rotateAmount)
{
    num = (num << rotateAmount) | (num >> 8 - rotateAmount);
}

void rotateRight(int &num, int rotateAmount)
{
    num = (num >> rotateAmount) | (num << 8 - rotateAmount);
}

void checkByteRange(signed char x, VirtualMachine &vm)
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
    std::ostringstream memoryItem;

    registerText << "Registers: ";
    for (int i = 0; i < 8; i++)
    {
        memoryItem.str("");
        memoryItem.clear();
        memoryItem << std::uppercase << std::setfill('0') << std::setw(2)
                   << std::hex << (static_cast<int>(vm.registers[i]) & 0xFF);
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
        memoryItem << std::uppercase << std::setfill('0') << std::setw(2)
                   << std::hex << (static_cast<int>(vm.memoryAddresses[i]) & 0xFF);
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

void INPUT(int regNum, VirtualMachine &vm)
{
    char res;
    std::cout << "?";
    std::cin >> res;
    std::cin.ignore(80, '\n');
    std::cout << std::endl;
    vm.registers[regNum] = res;
}

int main()
{
    // Initilize vm
    VirtualMachine vm;

    // initialize memory
    for (int i = 0; i < 64; i++)
    {
        // addToMemory(i, 0);
        vm.memoryAddresses[i] = 0;
    }
    std::ifstream assmeblyProgram("test1.asm");
    std::string instruction, command;
    while (getline(assmeblyProgram, instruction))
    {
        std::vector<std::string> command;
        std::string tempBuffer;
        for (char ins : instruction)
        {
            if (ins != ' ')
            {
                tempBuffer += ins;
            }
            else
            {
                command.push_back(tempBuffer);
                tempBuffer = "";
                continue;
            }
        }
        // if statement here
        if (command[0] == "INPUT")
        {
        }
        continue;
    }

    outputToFile(vm);
    return 0;
}