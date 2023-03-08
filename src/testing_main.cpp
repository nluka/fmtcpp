#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <algorithm>

#include "ntest.hpp"
#include "lexer.hpp"
#include "util.hpp"
#include "term.hpp"

int main() {
  {
    auto const res = ntest::init();
    std::cout
      << "ntest: cleaned " << res.num_files_removed << " residual files, "
      << res.num_files_failed_to_remove << " failed\n";
  }

  // lexer testing

  using lexer::TokenType;
  {
    std::vector<lexer::Token> const expected {
      { TokenType::PREPRO_DIR_INCLUDE,  0, 18 },
      { TokenType::NEWLINE,             18, 1 },

      { TokenType::COMMENT_SINGLELINE,  19, 22 },
      { TokenType::NEWLINE,             41, 1 },

      { TokenType::PREPRO_DIR_DEFINE,   42, 16 },
      { TokenType::NEWLINE,             58, 1 },
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/prepro.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      { TokenType::KEYWORD_INT,         0, 3 },
      { TokenType::IDENTIFIER,          4, 4 },
      { TokenType::SPECIAL_PAREN_OPEN,  8, 1 },
      { TokenType::KEYWORD_INT,         9, 3 },
      { TokenType::IDENTIFIER,          13, 4 },
      { TokenType::SPECIAL_COMMA,       17, 1 },
      { TokenType::KEYWORD_CHAR,        19, 4 },
      { TokenType::KEYWORD_CONST,       24, 5 },
      { TokenType::OPER_STAR,           30, 1 },
      { TokenType::OPER_STAR,           31, 1 },
      { TokenType::IDENTIFIER,          32, 4 },
      { TokenType::SPECIAL_PAREN_CLOSE, 36, 1 },
      { TokenType::SPECIAL_BRACE_OPEN,  38, 1 },
      { TokenType::NEWLINE,             39, 1 },
      { TokenType::KEYWORD_RETURN,      42, 6 },
      { TokenType::LITERAL_NUM,         49, 1 },
      { TokenType::SPECIAL_SEMICOLON,   50, 1 },
      { TokenType::NEWLINE,             51, 1 },
      { TokenType::SPECIAL_BRACE_CLOSE, 52, 1 },
      { TokenType::NEWLINE,             53, 1 },
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/main.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      { TokenType::KEYWORD_CHAR,       0, 4 },
      { TokenType::IDENTIFIER,         5, 1 },
      { TokenType::OPER_ASSIGN,        7, 1 },
      { TokenType::LITERAL_CHAR,       9, 3 },
      { TokenType::SPECIAL_SEMICOLON, 12, 1 },
      { TokenType::NEWLINE,           13, 1 },

      { TokenType::KEYWORD_CHAR,      14, 4 },
      { TokenType::IDENTIFIER,        19, 1 },
      { TokenType::OPER_ASSIGN,       21, 1 },
      { TokenType::LITERAL_CHAR,      23, 4 },
      { TokenType::SPECIAL_SEMICOLON, 27, 1 },
      { TokenType::NEWLINE,           28, 1 },

      { TokenType::KEYWORD_CHAR,      29, 4 },
      { TokenType::IDENTIFIER,        34, 1 },
      { TokenType::OPER_ASSIGN,       36, 1 },
      { TokenType::LITERAL_CHAR,      38, 4 },
      { TokenType::SPECIAL_SEMICOLON, 42, 1 },
      { TokenType::NEWLINE,           43, 1 },

      { TokenType::KEYWORD_CHAR,      44, 4 },
      { TokenType::IDENTIFIER,        49, 1 },
      { TokenType::OPER_ASSIGN,       51, 1 },
      { TokenType::LITERAL_CHAR,      53, 4 },
      { TokenType::SPECIAL_SEMICOLON, 57, 1 },
      { TokenType::NEWLINE,           58, 1 },
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/char_literals.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      { TokenType::KEYWORD_CHAR,       0, 4 },
      { TokenType::OPER_STAR,          5, 1 },
      { TokenType::IDENTIFIER,         6, 1 },
      { TokenType::OPER_ASSIGN,        8, 1 },
      { TokenType::LITERAL_STR,       10, 2 },
      { TokenType::SPECIAL_SEMICOLON, 12, 1 },
      { TokenType::NEWLINE,           13, 1 },

      { TokenType::KEYWORD_CHAR,      14, 4 },
      { TokenType::OPER_STAR,         19, 1 },
      { TokenType::IDENTIFIER,        20, 1 },
      { TokenType::OPER_ASSIGN,       22, 1 },
      { TokenType::LITERAL_STR,       24, 4 },
      { TokenType::SPECIAL_SEMICOLON, 28, 1 },
      { TokenType::NEWLINE,           29, 1 },

      { TokenType::KEYWORD_CHAR,      30, 4 },
      { TokenType::OPER_STAR,         35, 1 },
      { TokenType::IDENTIFIER,        36, 1 },
      { TokenType::OPER_ASSIGN,       38, 1 },
      { TokenType::LITERAL_STR,       40, 5 },
      { TokenType::SPECIAL_SEMICOLON, 45, 1 },
      { TokenType::NEWLINE,           46, 1 },

      { TokenType::KEYWORD_CHAR,      47, 4 },
      { TokenType::OPER_STAR,         52, 1 },
      { TokenType::IDENTIFIER,        53, 1 },
      { TokenType::OPER_ASSIGN,       55, 1 },
      { TokenType::LITERAL_STR,       57, 6 },
      { TokenType::SPECIAL_SEMICOLON, 63, 1 },
      { TokenType::NEWLINE,           64, 1 },
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/string_literals.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
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
