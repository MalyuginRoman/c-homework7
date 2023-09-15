#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

struct Command {
    std::vector<std::string> commandVectorStatic;
    std::vector<std::string> commandVectorDynamic;
    size_t count_st = 0;
    size_t count_dy = 0;
    size_t max_size = 3;
    int dynamicCount = 0;
    bool isActDyn = false;
    std::string st_file;
    std::string dy_file;
    std::chrono::system_clock::time_point file_time;
};

bool endStaticBlock(std::string text) // end static block
{
    if(text == "EOF" || text == "{")
        return true;
    else
        return false;
}
void PrintBlock(std::vector<std::string> vector, size_t count)
{
    std::cout << "bulk: ";
    for(size_t i = 0; i < count; ++i)
    {
        std::cout << vector[i];
        if (i < count - 1) std::cout << ", ";
        sleep(1);
    }
    std::cout << std::endl;
}

void SaveFile(Command &com, bool isUse)
{
    std::vector<std::string> vector;
    std::string name;
    size_t count;
    if(isUse) {
        name = com.st_file;
        count = com.count_st;
        vector = com.commandVectorStatic;
    }
    else {
        name = com.dy_file;
        count = com.count_dy;
        vector = com.commandVectorDynamic;
    }
    PrintBlock(vector, count);
    std::ofstream out;
    out.open(name);
    if(out.is_open()) {
        for(size_t i = 0; i < count; ++i)
        {
            out << vector[i];
            if (i < count - 1) out << ", ";
        }
        out.close();
    }
    else
        std::cout << "File is not open" << std::endl;
}

std::string GetFileName(Command &com)
{
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
    com.file_time.time_since_epoch()).count();
    std::stringstream filename;
    filename << "bulk" << seconds << ".log";
    return filename.str();
}

void Synhronize(Command &com, bool isUse)
{
    if(isUse) {
        com.commandVectorStatic.clear();
        com.count_st = com.commandVectorStatic.size();
    }
    else {
        com.commandVectorDynamic.clear();
        com.count_dy = com.commandVectorStatic.size();
    }
}

void RunBulk(Command &com, std::string text)
{
    if(!com.isActDyn)                                   // static block
    {
        if(!endStaticBlock(text))                   // text != "EOF" || text != "{"
        {
            if (!com.count_st)
                com.st_file = GetFileName(com);
            com.commandVectorStatic.push_back(text);
            com.count_st = com.commandVectorStatic.size();
            if(com.count_st == com.max_size)
            {
                SaveFile(com, true);
                Synhronize(com, true);
            }
        }
        if(endStaticBlock(text))                    // text == "EOF" || text == "{"
        {
            com.count_st = com.commandVectorStatic.size();
            if(com.count_st)
                SaveFile(com, true);
            Synhronize(com, true);
            if(text == "{")
            {
                com.isActDyn = true;
                com.dynamicCount ++;
            }
        }
    }
    else                                            // dynamic block
    {
        if(text != "{" && text != "}" && text != "EOF")
        {
            if (!com.count_dy)
                com.dy_file = GetFileName(com);
            com.commandVectorDynamic.push_back(text);
            com.count_dy = com.commandVectorDynamic.size();
        }
        if(text == "{") com.dynamicCount ++;
        if(text == "}") com.dynamicCount --;
        if(text == "EOF")
            com.commandVectorDynamic.clear();
        if (com.dynamicCount == 0)                      // end of dynamic block
        {
            SaveFile(com, false);
            Synhronize(com, false);
        }
    }
}

int main(int, char **)
{
    Command com;
    std::string text;
    while (std::getline(std::cin, text))
    {
        com.file_time = std::chrono::system_clock::now();
        RunBulk(com, text);
    }
}
