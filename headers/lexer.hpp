#ifndef __LEXER_H
#define __LEXER_H
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

/**
 * Token Types
 * These categorize the "words" of our language.
 * Using an enum makes the code more readable than using raw numbers.
 */
enum type {
    TOKEN_ID,        // Variable names or keywords (e.g., "number1", "return")
    TOKEN_INT,       // Whole numbers (e.g., "141")
    TOKEN_EQUALS,    // The '=' character
    TOKEN_SEMICOLON, // The ';' character
    TOKEN_LEFT_PAREN,  // '('
    TOKEN_RIGHT_PAREN, // ')'
    TOKEN_KEYWORD,
    TOKEN_EOF,
};

/**
 * Token Structure
 * A Token represents a single "atom" of the source code.
 * It pairs a category (TYPE) with its actual text (VALUE).
 */
struct Token {
    enum type TYPE;
    std::string VALUE;
};

std::string typeToString(enum type TYPE) {
    switch (TYPE) {
        case TOKEN_ID : return "TOKEN_ID";
        case TOKEN_INT : return "TOKEN_INT";
        case TOKEN_EQUALS : return "TOKEN_EQUALS";
        case TOKEN_SEMICOLON : return "TOKEN_SEMICOLON";
        case TOKEN_LEFT_PAREN : return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN : return "TOKEN_RIGHT_PAREN";
        case TOKEN_KEYWORD : return "TOKEN_KEYWORD";
        case TOKEN_EOF : return "TOKEN_EOF";
        default: return "UNKNOWN_TOKEN";
    }
}



class Lexer
{
    public:
    /**
     * Constructor: Initializes the Lexer with the source code.
     * We cache the length (size) to avoid calling .length() repeatedly,
     * which improves performance during large file scans.
     */
    explicit Lexer(const std::string &sourceCode)
    {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();
        lineNumber = 1;
        characterNumber = 1;

        // Safety: Prevent crashing if the user provides an empty file.
        if (size > 0) {
            current = source.at(cursor);
        } else {
            current = '\0'; // Null terminator signals the end of input.
        }
    }

    /**
     * Advance: Moves the cursor one step forward.
     * @return The character we were standing on before we moved.
     */
    char advance ()
    {
        if (cursor < size) {
            const char temp = current;
            cursor++;
            characterNumber++; // Keep track of the column for every single character
            current = (cursor < size ) ? source[cursor] : '\0';
            return temp;
        }
        return '\0';
    }
    /**
     * Skip Whitespace: Drains non-functional characters (spaces, tabs, newlines).
     * These characters help humans read code but are ignored by the compiler logic.
     */
    void checkAndSkip() {
        while (current == ' ' || current == '\t' || current == '\n' || current == '\r') {
            if (current == '\n') {
                lineNumber++;
                characterNumber = 0;
            }
            else {
                characterNumber++;
            }
            advance();
        }
    }

    /**
     * Tokenize Identifier: Gathers characters to form a word or keyword.
     * Triggered when the Lexer sees a letter or an underscore.
     */
    std::vector <std::string> keywords = {"return", "print"};
    Token * tokenizeID_KEYWORD() {
        std::stringstream buffer;
        // As long as characters are letters, numbers, or underscores, they belong to the ID.
        while (isalnum(current) || current == '_') {
            buffer << advance();
        }
        Token * newToken = new Token();

        newToken->TYPE = (std::find(keywords.begin(), keywords.end(), buffer.str()) != keywords.end()) ? TOKEN_KEYWORD : TOKEN_ID;
        newToken->VALUE = buffer.str();
        // Note: 'new' creates the token on the HEAP so it persists after this function ends.
        return newToken;
    }
    Token * tokenizeSPECIAL(enum type TYPE) {
        Token * newToken = new Token();
        newToken->TYPE = TYPE;
        newToken->VALUE = std::string(1, advance());
        return newToken;
    }
    Token * tokenizeINT() {
        std::stringstream buffer;
        while (isdigit(current)) {
            buffer << advance();
        }
        Token * newToken = new Token();
        newToken->TYPE = TOKEN_INT;
        newToken->VALUE = buffer.str();

        return newToken;
    }

    /**
     * Main Lexing Loop: Walks through the entire file and generates a list of tokens.
     * @return A vector (dynamic list) of pointers to the tokens found.
     */
    std::vector<Token *> tokenize()
    {
        std::vector<Token *> tokens;

        while (cursor < size && current != '\0') {
            checkAndSkip();

            // Logic Branch: If it starts with a letter, it must be an Identifier.
            if (isalpha(current) || current == '_') // this is the logic for ids
                {
                tokens.push_back(tokenizeID_KEYWORD());
                continue;
            }

            if (isdigit (current)) // this is the logic for integer literals
            {
                tokens.push_back(tokenizeINT());
                continue;
            }

            switch(current) {
                case ';':
                {
                tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));
                continue;
                }

                case '=':
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                    continue;
                }

                case '(':
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_PAREN));
                    continue;
                }

                case ')':
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PAREN));
                    continue;
                }

                case 0:
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EOF));
                    continue;
                }

                default: {
                    std::cerr << "[!] LEXER ERROR: Unexpected character '" << current << "'" << std::endl;
                    std::cerr << "At line " << lineNumber << ", column " << characterNumber << std::endl;
                    exit(1);
                }
            }

            // Fallback: If we don't recognize a character, we must skip or handle it
            // to avoid an infinite loop.
            if (current != '\0') advance();
        }
        return tokens;
    }

    /**
     * Peek: Look ahead at future characters without moving the cursor.
     * Essential for "multi-character" symbols like '==' or '!='.
     */
    char peak (const int offset = 0) const {
        if (cursor + offset < size) {
            return source[cursor + offset];
        }
        return '\0';
    }

    private:
    std::string source; // The raw source code text
    int cursor;         // Current reading position (index)
    int size;           // Total length of the source code
    char current;       // The character currently being inspected
    int lineNumber;     // The line number in which an error occurred
    int characterNumber;


};

#endif