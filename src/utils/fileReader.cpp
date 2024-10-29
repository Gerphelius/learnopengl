#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fileReader.h"

// TODO: refactor this shit
std::string readFile(const char* path)
{
    std::string fileContent{ "" };
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(path);

        std::stringstream stream;
        stream << file.rdbuf();

        file.close();

        fileContent = stream.str();
    }
    catch (const std::ifstream::failure e)
    {
        std::cout << "ERROR::FILE::FILE_READ_FAILED\n";
    }

    return fileContent;
}
