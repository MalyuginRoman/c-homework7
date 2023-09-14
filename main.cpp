#include <iostream>
#include <vector>

#include "lib.h"

bool endStaticBlock(std::string text) // end static block
{
    if(text == "EOF" || text == "{")
        return true;
    else
        return false;
}
void PB(std::vector<std::string> vector, size_t count) // print block
{
    for(size_t i = 0; i < count; ++i)
    {
        std::cout << vector[i];
        if (i < count - 1) std::cout << ", ";
    }
}

int main(int , char **)
{
    std::vector<std::string> commandVectorStatic;
    std::vector<std::string> commandVectorDynamic;
    std::string text;
    size_t count_st = 0;
    size_t count_dy = 0;
    size_t max_size = 3;
    int dynamicCount = 0;
    bool isActDyn = false;
    while (std::getline(std::cin, text))
    {

        if(!isActDyn)                                   // static block
        {
            if(!endStaticBlock(text))                   // text == "EOF" || text == "{"
            {
                commandVectorStatic.push_back(text);
                count_st = commandVectorStatic.size();
                if(count_st == max_size)
                {
                    std::cout << "bulk: ";
                    PB(commandVectorStatic, count_st);
                    std::cout << std::endl;
                    commandVectorStatic.clear();
                }
            }
            if(endStaticBlock(text))                    // text == "EOF" || text == "{"
            {
                count_st = commandVectorStatic.size();
                std::cout << "bulk: ";
                PB(commandVectorStatic, count_st);
                std::cout << std::endl;
                commandVectorStatic.clear();
                if(text == "{")
                {
                    isActDyn = true;
                    dynamicCount ++;
                }
            }
        }
        else                                            // dynamic block
        {
            if(text != "{" && text != "}")
            {
                commandVectorDynamic.push_back(text);
            }
            if(text == "{") dynamicCount ++;
            if(text == "}") dynamicCount --;
            if (dynamicCount == 0)                      // end of dynamic block
            {
                count_dy = commandVectorDynamic.size();
                std::cout << "bulk: ";
                PB(commandVectorDynamic, count_dy);
                std::cout << std::endl;
                commandVectorDynamic.clear();
            }
        }

    }
    return 0;
}
