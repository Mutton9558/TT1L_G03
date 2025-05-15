#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <string>
#include <fstream>
#include <vector>

struct VirtualMachine
{
    std::map<std::string, char> registers = {{"R0", 0}, {"R1", 0}, {"R2", 0}, {"R3", 0}, {"R4", 0}, {"R5", 0}, {"R6", 0}, {"R7", 0}};
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
    std::ostringstream memoryItem;

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

void INPUT(std::string regNum, VirtualMachine &vm)
{
    // subject to change
    char res;
    std::cout << "?";
    std::cin >> res;
    std::cin.ignore(80, '\n');
    std::cout << std::endl;
    int ans = static_cast<int>(res);
    checkByteRange(ans, vm);
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
    outputToFile(vm);
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
            }
        }
        if (!tempBuffer.empty())
        {
            command.push_back(tempBuffer);
        }
        // if statement here
        if (command[0] == "INPUT")
        {
            if (command.size() != 2)
            {
                std::cout << "Invalid length for command INPUT" << std::endl;
            }
            else
            {
                INPUT(command[1], vm);
            }
        }

        vm.PC++;
    }

    outputToFile(vm);
    return 0;
}