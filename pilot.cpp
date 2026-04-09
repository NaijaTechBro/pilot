#include <iostream>
#include <sstream>
#include <fstream>
#include "headers/lexer.hpp"


int main(int argc, char ** argv) {
    {
        if (argc < 2)
        {
            std::cout << "Please supply the source file";
            exit(1);
        }
    }

    std::cout << "Reading from the file name : " << argv[1] << std::endl;
    std::ifstream sourceFileStream (argv[1]);

    std::stringstream buffer;

    char temp;

    while (sourceFileStream.get(temp))
    {
        buffer << temp;
    }

    std::string sourceCode = buffer.str();

    std::cout << "This is the source code " << std::endl << sourceCode;

    Lexer lexer (sourceCode);

    std::cout << "this is the end of the program " << std::endl;

    return 0;
}