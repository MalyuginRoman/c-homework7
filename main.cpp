#include <iostream>
#include <string>
#include <vector>

#include "lib.h"

bool endStaticBlock(std::string text) // end static block
{
    if(text == "EOF" || text == "{")
        return true;
    else
        return false;
}
void PrintBlock(std::vector<std::string> vector, size_t count)
{
    for(int i = 0; i < count; ++i)
    {
        std::cout << vector[i];
        if (i < count - 1) std::cout << ", ";
    }
}

struct Command {
    std::vector<std::string> commandVectorStatic;
    std::vector<std::string> commandVectorDynamic;
    size_t count_st = 0;
    size_t count_dy = 0;
    int max_size = 3;
    int dynamicCount = 0;
    bool isActDyn = false;
};

void RunBulk(Command &com, std::string text)
{
    if(!com.isActDyn)                                   // static block
    {
        if(!endStaticBlock(text))                   // text == "EOF" || text == "{"
        {
            com.commandVectorStatic.push_back(text);
            com.count_st = com.commandVectorStatic.size();
            if(com.count_st == com.max_size)
            {
                std::cout << "bulk: ";
                PrintBlock(com.commandVectorStatic, com.count_st);
                std::cout << std::endl;
                com.commandVectorStatic.clear();
            }
        }
        if(endStaticBlock(text))                    // text == "EOF" || text == "{"
        {
            com.count_st = com.commandVectorStatic.size();
            std::cout << "bulk: ";
            PrintBlock(com.commandVectorStatic, com.count_st);
            std::cout << std::endl;
            com.commandVectorStatic.clear();
            if(text == "{")
            {
                com.isActDyn = true;
                com.dynamicCount ++;
            }
        }
    }
    else                                            // dynamic block
    {
        if(text != "{" && text != "}")
        {
            com.commandVectorDynamic.push_back(text);
        }
        if(text == "{") com.dynamicCount ++;
        if(text == "}") com.dynamicCount --;
        if (com.dynamicCount == 0)                      // end of dynamic block
        {
            com.count_dy = com.commandVectorDynamic.size();
            std::cout << "bulk: ";
            PrintBlock(com.commandVectorDynamic, com.count_dy);
            std::cout << std::endl;
            com.commandVectorDynamic.clear();
        }
    }
}

int main(int, char **)
{
    Command com;
    std::string text;
    while (std::getline(std::cin, text))
    {
        RunBulk(com, text);
    }
    return 0;
}
