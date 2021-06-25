#ifndef COMPILER_H
#define COMPILER_H

#include "AST.h"

#include <memory>
#include <string>

class Compiler {
public:
    int compile(const std::string& expression);
    int calculate(AST* root);
};

#endif
