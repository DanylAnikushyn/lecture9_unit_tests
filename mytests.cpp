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

    // lexing of more complex expression
    lexer = Lexer(std::string{"(1 - 2) * 8 - 10 / 123"});
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"("});
    GTEST_ASSERT_EQ(token->type, TokenType::LPA);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"1"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"-"});
    GTEST_ASSERT_EQ(token->type, TokenType::SUB);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"2"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{")"});
    GTEST_ASSERT_EQ(token->type, TokenType::RPA);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"*"});
    GTEST_ASSERT_EQ(token->type, TokenType::MUL);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"8"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"-"});
    GTEST_ASSERT_EQ(token->type, TokenType::SUB);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"10"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"/"});
    GTEST_ASSERT_EQ(token->type, TokenType::DIV);
    token = lexer.next_token();
    GTEST_ASSERT_EQ(token->value, std::string{"123"});
    GTEST_ASSERT_EQ(token->type, TokenType::INT);

    // encountering lexing error
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