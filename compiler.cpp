#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"

#include <string>
#include <memory>

int Compiler::compile(const std::string& expression) {
    auto lexer = std::make_unique<Lexer>(expression);
    auto parser = std::make_unique<Parser>(std::move(lexer));
    auto root = parser->parse();

    return calculate(std::move(root));
}

int Compiler::calculate(std::unique_ptr<AST> root) {
    return 4;
} 