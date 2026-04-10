#ifndef __LEXER_H
#define __LEXER_H
#include <string>
#include <vector>
#include <sstream>

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
            // Update 'current' for the new position, or set to null if we hit the end.
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
            advance();
        }
    }

    /**
     * Tokenize Identifier: Gathers characters to form a word or keyword.
     * Triggered when the Lexer sees a letter or an underscore.
     */
    Token * tokenizeID() {
        std::stringstream buffer;
        // As long as characters are letters, numbers, or underscores, they belong to the ID.
        while (isalnum(current) || current == '_') {
            buffer << advance();
        }
        // Note: 'new' creates the token on the HEAP so it persists after this function ends.
        return new Token{TOKEN_ID, buffer.str()};
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
            if (isalpha(current) || current == '_') {
                tokens.push_back(tokenizeID());
                continue;
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
};

#endif