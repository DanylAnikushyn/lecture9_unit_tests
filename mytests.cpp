#include <gtest/gtest.h>
#include <memory>
#include "compiler.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"

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

TEST(parser, parser)
{
    // try just single number
    auto lexer = std::make_unique<Lexer>(std::string{"2"});
    auto parser = std::make_unique<Parser>(std::move(lexer));
    auto root = parser->parse();

    GTEST_ASSERT_EQ(root->type, ASTType::COMPOUND);
    GTEST_ASSERT_EQ(root->left, nullptr);
    GTEST_ASSERT_EQ(root->right, nullptr);
    GTEST_ASSERT_EQ(root->value->type, ASTType::INT);
    GTEST_ASSERT_EQ(root->value->op, TokenType::INT);
    GTEST_ASSERT_EQ(root->value->int_value, 2);
    GTEST_ASSERT_EQ(root->value->left, nullptr);
    GTEST_ASSERT_EQ(root->value->right, nullptr);

    // finding root of arithmetic expression
    lexer = std::make_unique<Lexer>(std::string{"2 + 2"});
    parser = std::make_unique<Parser>(std::move(lexer));
    root = parser->parse();

    GTEST_ASSERT_EQ(root->type, ASTType::COMPOUND);
    GTEST_ASSERT_EQ(root->left, nullptr);
    GTEST_ASSERT_EQ(root->right, nullptr);
    GTEST_ASSERT_EQ(root->value->type, ASTType::BINOP);
    GTEST_ASSERT_EQ(root->value->op, TokenType::ADD);
    GTEST_ASSERT_EQ(root->value->left->int_value, 2);
    GTEST_ASSERT_EQ(root->value->left->type, ASTType::INT);
    GTEST_ASSERT_EQ(root->value->left->op, TokenType::INT);
    GTEST_ASSERT_EQ(root->value->right->int_value, 2);
    GTEST_ASSERT_EQ(root->value->right->type, ASTType::INT);
    GTEST_ASSERT_EQ(root->value->right->op, TokenType::INT);
    GTEST_ASSERT_EQ(root->value->value, nullptr);
    GTEST_ASSERT_EQ(root->value->right->right, nullptr);
    GTEST_ASSERT_EQ(root->value->right->value, nullptr);
    GTEST_ASSERT_EQ(root->value->right->left, nullptr);
    GTEST_ASSERT_EQ(root->value->left->right, nullptr);
    GTEST_ASSERT_EQ(root->value->left->value, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left, nullptr);

    // trickier arithmetic expression
    lexer = std::make_unique<Lexer>(std::string{"(1 - 2) * 8 - 10"});
    parser = std::make_unique<Parser>(std::move(lexer));
    root = parser->parse();
    
    GTEST_ASSERT_EQ(root->type, ASTType::COMPOUND);
    GTEST_ASSERT_EQ(root->left, nullptr);
    GTEST_ASSERT_EQ(root->right, nullptr);
    GTEST_ASSERT_EQ(root->value->type, ASTType::BINOP);
    GTEST_ASSERT_EQ(root->value->op, TokenType::SUB);
    GTEST_ASSERT_EQ(root->value->left->type, ASTType::BINOP);
    GTEST_ASSERT_EQ(root->value->left->op, TokenType::MUL);
    GTEST_ASSERT_EQ(root->value->left->value, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->type, ASTType::COMPOUND);
    GTEST_ASSERT_EQ(root->value->left->left->op, TokenType::NOT);
    GTEST_ASSERT_EQ(root->value->left->left->left, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->right, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->type, ASTType::BINOP);
    GTEST_ASSERT_EQ(root->value->left->left->value->op, TokenType::SUB);
    GTEST_ASSERT_EQ(root->value->left->left->value->left->type, ASTType::INT);
    GTEST_ASSERT_EQ(root->value->left->left->value->left->op, TokenType::INT);
    GTEST_ASSERT_EQ(root->value->left->left->value->left->int_value, 1);
    GTEST_ASSERT_EQ(root->value->left->left->value->left->left, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->left->value, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->left->right, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->value, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->right->type, ASTType::INT);
    GTEST_ASSERT_EQ(root->value->left->left->value->right->op, TokenType::INT);
    GTEST_ASSERT_EQ(root->value->left->left->value->right->int_value, 2);
    GTEST_ASSERT_EQ(root->value->left->left->value->right->left, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->right->value, nullptr);
    GTEST_ASSERT_EQ(root->value->left->left->value->right->right, nullptr);
    GTEST_ASSERT_EQ(root->value->right->type, ASTType::INT);
    GTEST_ASSERT_EQ(root->value->right->op, TokenType::INT);
    GTEST_ASSERT_EQ(root->value->right->int_value, 10);
    GTEST_ASSERT_EQ(root->value->right->left, nullptr);
    GTEST_ASSERT_EQ(root->value->right->value, nullptr);
    GTEST_ASSERT_EQ(root->value->right->right, nullptr);
}

TEST(compiler, compiler)
{
    std::unique_ptr<Compiler> compiler;
    EXPECT_EQ(compiler->compile(std::string{"2"}), 2);
    EXPECT_EQ(compiler->compile(std::string{"2 + 2"}), 4);
    EXPECT_EQ(compiler->compile(std::string{"(1 - 2) * 8 - 10"}), -18);
    EXPECT_EQ(compiler->compile(std::string{"10 / (5 - 3) - 2 + 10 / 2"}), 8);
    EXPECT_EQ(compiler->compile(std::string{"(40 / 2 / 2 - 1) * 10"}), 90);
    EXPECT_EQ(compiler->compile(std::string{"(40 / 2 / 2 / 2 - 1) * 10"}), 40); // !!!!!!
    EXPECT_EQ(compiler->compile(std::string{"40 * 2 * 2 - 1"}), 159);
    EXPECT_EQ(compiler->compile(std::string{"40 * 2 * 2 * 2 - 1"}), 319);
    EXPECT_EQ(compiler->compile(std::string{"(10 - (10 - 6 / 2) / 1) + 10 / 2 * 2"}), 13);
    EXPECT_EQ(compiler->compile(std::string{"10 / 2 * 3 / 5 "}), 3); // !!!!!!!

    EXPECT_THROW(compiler->compile(std::string{"(10 - 2) / /"}), std::runtime_error); // unexpected token
    EXPECT_THROW(compiler->compile(std::string{"(10 - 2) / ()"}), std::runtime_error); // empty parentheses
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}