#include <gtest/gtest.h>
#include <memory>
#include "compiler.h"
#include "token.h"
#include "lexer.h"

TEST(lexer, lexer) 
{
    // Lexer with no expression throws exception
    ASSERT_THROW(Lexer(std::string{""}), std::runtime_error);

    // lexing of basic expression    
    Lexer lexer(std::string{"2 + 2"});

    std::unique_ptr<Token> token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"2"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"+"});
    GTEST_ASSERT_EQ(token->type, TokenType::ADD);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"2"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{""});
    GTEST_ASSERT_EQ(token->type, TokenType::END);

    // checking lexer for lexing error
    lexer = Lexer(std::string{"2 ?"});
    token = lexer.next_token();
    ASSERT_THROW(lexer.next_token(), std::runtime_error);
}

TEST(compiler, compiler)
{
    std::unique_ptr<Compiler> compiler;
    GTEST_ASSERT_EQ(compiler->compile("2 + 2"), 4);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}