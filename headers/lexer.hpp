#ifndef __LEXER_H
#define __LEXER_H
#include <string>


enum type {
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
};

struct Token {
    enum type TYPE;
    std::string VALUE;

};

class Lexer
{
    public:
    Lexer(std::string sourceCode)
    {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();

        // Safety check: only call .at() if the string isn't empty!
        if (size > 0) {
            current = source.at(cursor);
        } else {
            current = '\0';
        }
    }

    char advance ()
    {
        if (cursor < size) {
            char temp = current;
            cursor++;
            current = (cursor < size ) ? source[cursor] : '\0';

            return temp;
        }
        else {
            return '\0';
        }
    }

    char peak (int offset = 0)
    {
        if (cursor + offset < size) {
            return source[cursor + offset];
        }
        else {
            return '\0';
        }
    }
    private:
    std::string source;
    int cursor;
    int size;
    char current;


};


#endif