#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <algorithm>

#include "ntest.hpp"
#include "../src/lexer.hpp"
#include "../src/util.hpp"
#include "../src/term.hpp"

int main() {
  ntest::init();

  // lexer testing

  using lexer::TokenType;
  {
    std::vector<lexer::Token> const expected {
      { TokenType::KEYWORD_INT, 1-1, 3 },
      { TokenType::IDENTIFIER, 5-1, 4 },
      { TokenType::SPECIAL_PAREN_OPEN, 9-1, 1 },
      { TokenType::KEYWORD_INT, 10-1, 3 },
      { TokenType::IDENTIFIER, 14-1, 4 },
      { TokenType::SPECIAL_COMMA, 18-1, 1 },
      { TokenType::KEYWORD_CHAR, 20-1, 4 },
      { TokenType::KEYWORD_CONST, 25-1, 5 },
      { TokenType::OPER_STAR, 31-1, 1 },
      { TokenType::OPER_STAR, 32-1, 1 },
      { TokenType::IDENTIFIER, 33-1, 4 },
      { TokenType::SPECIAL_PAREN_CLOSE, 37-1, 1 },
      { TokenType::SPECIAL_BRACE_OPEN, 38-1, 1 },
      { TokenType::KEYWORD_RETURN, 39-1, 6 },
      { TokenType::LITERAL_NUM, 46-1, 1 },
      { TokenType::SPECIAL_SEMICOLON, 47-1, 1 },
      { TokenType::SPECIAL_BRACE_CLOSE, 48-1, 1 },
      { TokenType::NEWLINE, 49-1, 1 },
    };
    std::string const text = util::extract_txt_file_contents("basic/tiny_main.c");
    std::vector<lexer::Token> const actual = lexer::lex(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      { TokenType::PREPRO_DIR_INCLUDE, 1-1, 8 },
      { TokenType::IMPLEMENTATION_DEFINED_HEADER, 10-1, 9 },
      { TokenType::NEWLINE, 19-1, 1 },

      { TokenType::COMMENT_SINGLELINE, 20-1, 22 },
      { TokenType::NEWLINE, 42-1, 1 },

      { TokenType::PREPRO_DIR_DEFINE, 43-1, 8 },
      { TokenType::IDENTIFIER, 52-1, 2 },
      { TokenType::LITERAL_NUM, 55-1, 4 },
      { TokenType::NEWLINE, 59-1, 1 },
    };
    std::string const text = util::extract_txt_file_contents("basic/tiny_prepro.c");
    std::vector<lexer::Token> const actual = lexer::lex(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      { TokenType::KEYWORD_CHAR,       1-1, 4 },
      { TokenType::IDENTIFIER,         6-1, 1 },
      { TokenType::OPER_ASSIGN,        7-1, 1 },
      { TokenType::LITERAL_CHAR,       8-1, 3 },
      { TokenType::SPECIAL_SEMICOLON, 11-1, 1 },

      { TokenType::KEYWORD_CHAR,      12-1, 4 },
      { TokenType::IDENTIFIER,        17-1, 1 },
      { TokenType::OPER_ASSIGN,       18-1, 1 },
      { TokenType::LITERAL_CHAR,      19-1, 4 },
      { TokenType::SPECIAL_SEMICOLON, 23-1, 1 },

      { TokenType::KEYWORD_CHAR,      24-1, 4 },
      { TokenType::IDENTIFIER,        29-1, 1 },
      { TokenType::OPER_ASSIGN,       30-1, 1 },
      { TokenType::LITERAL_CHAR,      31-1, 4 },
      { TokenType::SPECIAL_SEMICOLON, 35-1, 1 },

      { TokenType::KEYWORD_CHAR,      36-1, 4 },
      { TokenType::IDENTIFIER,        41-1, 1 },
      { TokenType::OPER_ASSIGN,       42-1, 1 },
      { TokenType::LITERAL_CHAR,      43-1, 4 },
      { TokenType::SPECIAL_SEMICOLON, 47-1, 1 },

      { TokenType::NEWLINE, 48-1, 1 },
    };
    std::string const text = util::extract_txt_file_contents("basic/tiny_charliterals.c");
    std::vector<lexer::Token> const actual = lexer::lex(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      { TokenType::KEYWORD_CHAR,       1-1, 4 },
      { TokenType::OPER_STAR,          6-1, 1 },
      { TokenType::IDENTIFIER,         7-1, 1 },
      { TokenType::OPER_ASSIGN,        8-1, 1 },
      { TokenType::LITERAL_STR,        9-1, 2 },
      { TokenType::SPECIAL_SEMICOLON, 11-1, 1 },
      { TokenType::NEWLINE,           12-1, 1 },

      { TokenType::KEYWORD_CHAR,      13-1, 4 },
      { TokenType::OPER_STAR,         18-1, 1 },
      { TokenType::IDENTIFIER,        19-1, 1 },
      { TokenType::OPER_ASSIGN,       20-1, 1 },
      { TokenType::LITERAL_STR,       21-1, 4 },
      { TokenType::SPECIAL_SEMICOLON, 25-1, 1 },
      { TokenType::NEWLINE,           26-1, 1 },

      { TokenType::KEYWORD_CHAR,      27-1, 4 },
      { TokenType::OPER_STAR,         32-1, 1 },
      { TokenType::IDENTIFIER,        33-1, 1 },
      { TokenType::OPER_ASSIGN,       34-1, 1 },
      { TokenType::LITERAL_STR,       35-1, 5 },
      { TokenType::SPECIAL_SEMICOLON, 40-1, 1 },
      { TokenType::NEWLINE,           41-1, 1 },

      { TokenType::KEYWORD_CHAR,      42-1, 4 },
      { TokenType::OPER_STAR,         47-1, 1 },
      { TokenType::IDENTIFIER,        48-1, 1 },
      { TokenType::OPER_ASSIGN,       49-1, 1 },
      { TokenType::LITERAL_STR,       50-1, 6 },
      { TokenType::SPECIAL_SEMICOLON, 56-1, 1 },
      { TokenType::NEWLINE,           57-1, 1 },
    };
    std::string const text = util::extract_txt_file_contents("basic/tiny_stringliterals.c");
    std::vector<lexer::Token> const actual = lexer::lex(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }

  {
    using namespace term::color;
    auto const res = ntest::generate_report("Ctruct");
    if (res.num_fails > 0) {
      printf(fore::RED | back::BLACK, "%zu tests failed\n", res.num_fails);
    } else {
      printf(fore::GREEN | back::BLACK, "all %zu tests passed\n", res.num_passes);
    }
  }

  return 0;
}
