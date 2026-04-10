#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
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

    if (!sourceFileStream.is_open()) {
        std::cout << "Could not open file: " << argv[1] << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << sourceFileStream.rdbuf();
    std::string sourceCode = buffer.str();

    char temp;

    std::cout << "This is the source code " << std::endl << sourceCode;

    Lexer lexer(sourceCode);
    std::vector <Token *> tokens = lexer.tokenize();
    int counter = 0;
    std::cout << "[*] TOKENIZED SUCCESSFULLY" << std::endl;

    for (Token * temp : tokens) {
        counter++;
        std::cout << counter <<  ") " << temp->VALUE << " " << typeToString(temp->TYPE)<< std::endl;
    }
    std::cout << "this is the end of the program " << std::endl;

    return 0;
}