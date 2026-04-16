#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.hpp"
#include <vector>

enum NODE_TYPE {
    NODE_ROOT,
    NODE_ID,
    NODE_RETURN,
    NODE_PRINT,
    NODE_INT,
};
struct AST_NODE {
    enum NODE_TYPE TYPE;
    std::string * VALUE;
    AST_NODE * CHILD;
    std::vector <AST_NODE *> SUB_STATEMENT; // This is only for the root node

}AST_NODE;

class Parser {
    public:
    Parser(std::vector <Token *> tokens) {
        parserTokens = tokens;
        index = 0;
        limit = parserTokens.size();
        current = parserTokens.at(index);

    }
    private:
    int limit;
    int index;
    Token * current;
    std::vector <Token *> parserTokens;

};

#endif