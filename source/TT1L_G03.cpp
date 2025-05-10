#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <string>

// Flags
bool OF, UF, CF, ZF;

// Memory locations
std::map<int, std::string> memoryAddresses;
std::stringstream memoryItem;

std::string outputMemory()
{
    // declare ostringstream to write string data more efficiently
    std::ostringstream memoryData;
    for (int i = 0; i < 64; i++)
    {
        if (i % 8 == 0 && i != 0)
        {
            memoryData << ("\n");
        }

        memoryData << (memoryAddresses[i] + " ");
    }

    return memoryData.str();
}

char checkByteRange(signed char x)
{
    if (x > 127)
    {
        OF = 1;
        x -= 127;
    }
    else if (x < -128)
    {
        UF = 1;
        x += 128;
    }

    return x;
}

void addToMemory(int memoryAddress, int val)
{
    memoryItem.str("");
    memoryItem << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (val & 0xFF);
    memoryAddresses[memoryAddress] = memoryItem.str();
}

int main()
{
    // initialize memory
    for (int i = 0; i < 64; i++)
    {
        addToMemory(i, 0);
    }

    // test
    addToMemory(10, 14);
    std::string memoryData = outputMemory();
    std::cout << memoryData << std::endl;
    return 0;
}