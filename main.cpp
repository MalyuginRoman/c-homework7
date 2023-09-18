#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include "MyList.h"

namespace command
{
    struct command_block
    {
        std::vector<std::string> command;
        size_t count = 0;
        std::string file_name;
    };

    bool isActDyn = false;
    std::chrono::system_clock::time_point file_time;
    int dynamicCount = 0;
    size_t max_size = 3;

    bool endStaticBlock(std::string text) // end static block
    {
        if(text == "EOF" || text == "{")
            return true;
        else
            return false;
    }

    void PrintBlock(command_block vector)
    {
        std::cout << "bulk: ";
        for(size_t i = 0; i < vector.count; ++i)
        {
            std::cout << vector.command[i];
            if (i < vector.count - 1) std::cout << ", ";
            Sleep(1000);
        }
        std::cout << std::endl;
    }

    void SaveFile(command_block vector)
    {
        PrintBlock(vector);
        std::ofstream out;
        out.open(vector.file_name);
        if(out.is_open()) {
            for(size_t i = 0; i < vector.count; ++i)
            {
                out << vector.command[i];
                if (i < vector.count - 1) out << ", ";
            }
            out.close();
        }
        else
            std::cout << "File is not open" << std::endl;
    }

    std::string GetFileName()
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>
        (file_time.time_since_epoch()).count();
        std::stringstream filename;
        filename << "bulk" << seconds << ".log";
        return filename.str();
    }

    void Synhronize(command_block &vector)
    {
        vector.command.clear();
        vector.count = vector.command.size();
    }

    void RunBulk()
    {
        std::string text;
        command_block static_block;
        command_block dynamic_block;
        while (std::getline(std::cin, text))
        {
            file_time = std::chrono::system_clock::now();
            if(!isActDyn)                                   // static block
            {
                if(!endStaticBlock(text))                   // text != "EOF" || text != "{"
                {
                    if (!static_block.count)
                        static_block.file_name = GetFileName();
                    static_block.command.push_back(text);
                    static_block.count = static_block.command.size();
                    if(static_block.count == max_size)
                    {
                        SaveFile(static_block);
                        Synhronize(static_block);
                    }
                }
                if(endStaticBlock(text))                    // text == "EOF" || text == "{"
                {
                    static_block.count = static_block.command.size();
                    if(static_block.count)
                        SaveFile(static_block);
                    Synhronize(static_block);
                    if(text == "{")
                    {
                        isActDyn = true;
                        dynamicCount ++;
                    }
                }
            }
            else                                            // dynamic block
            {
                if(text != "{" && text != "}" && text != "EOF")
                {
                    if (!dynamic_block.count)
                        dynamic_block.file_name = GetFileName();
                    dynamic_block.command.push_back(text);
                    dynamic_block.count = dynamic_block.command.size();
                }
                if(text == "{") dynamicCount ++;
                if(text == "}") dynamicCount --;
                if(text == "EOF")
                    dynamic_block.command.clear();
                if (dynamicCount == 0)                      // end of dynamic block
                {
                    SaveFile(dynamic_block);
                    Synhronize(dynamic_block);
                    isActDyn = false;
                }
            }
        }
    }
}

int main()
{
    command::RunBulk();
    return 0;
}
