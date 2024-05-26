#ifndef _READ_
#define _READ_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
class read;
class read
{
public:
    std::vector<std::string> processedLines;

    read(const char *path = {}) : path(path)
    {
        FILE *file;
        file = fopen(path, "r");
        if (file == nullptr)
        {
            std::cerr << "File open failed" << std::endl;
            exit(1);
        }
        processFile(file);
        fclose(file);
    }

private:
    const char *path;
    void processFile(FILE *file)
    {
        char buffer[1024];
        while (fgets(buffer, 1024, file) != NULL)
        {
            std::string line(buffer);
            processedLines = processLine(line);
        }
    }

    std::vector<std::string> processLine(const std::string &line)
    {
        std::string processedLine = {};
        for (char c : line)
        {
            if (c >= 'a' && c <= 'z')
            {
                processedLine += c;
            }
            else if (c >= 'A' && c <= 'Z')
            {
                processedLine += c - 'A' + 'a';
            }
            else
            {
                if (processedLine != "")
                    processedLines.push_back(processedLine);
                processedLine = {};
            }
        }
        return processedLines;
    }
};

#endif