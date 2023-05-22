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
  using namespace term;
  namespace fs = std::filesystem;

  std::printf("std::filesystem::current_path() = %s\n", fs::current_path().string().c_str());

  {
    auto const res = ntest::init();
    size_t const total = res.num_files_removed + res.num_files_failed_to_remove;

    if (total > 0)
      printf(FG_MAGENTA, "ntest: ");
    if (res.num_files_removed)
      printf(FG_MAGENTA, "%zu residual files removed ", res.num_files_removed);
    if (res.num_files_failed_to_remove > 0)
      printf(FG_YELLOW, "(%zu residual files failed to be removed)", res.num_files_failed_to_remove);
    if (total > 0)
      std::printf("\n");
  }

  using lexer::TokenType;
  using lexer::Token;

  {
    std::vector<Token> const expected {
      Token(TokenType::PREPRO_DIR_INCLUDE,  0, 18 ),
      Token(TokenType::NEWLINE,             18, 1 ),

      Token(TokenType::COMMENT_SINGLELINE,  19, 22),
      Token(TokenType::NEWLINE,             41, 1 ),

      Token(TokenType::PREPRO_DIR_DEFINE,   42, 16),
      Token(TokenType::NEWLINE,             58, 1 ),

      Token(TokenType::PREPRO_DIR_DEFINE,   59, 27),
      Token(TokenType::NEWLINE,             86, 1 ),
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/prepro.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      Token(TokenType::KEYWORD_INT,         0, 3),
      Token(TokenType::IDENTIFIER,          4, 4),
      Token(TokenType::SPECIAL_PAREN_OPEN,  8, 1),
      Token(TokenType::KEYWORD_INT,         9, 3),
      Token(TokenType::IDENTIFIER,          13, 4),
      Token(TokenType::SPECIAL_COMMA,       17, 1),
      Token(TokenType::KEYWORD_CHAR,        19, 4),
      Token(TokenType::KEYWORD_CONST,       24, 5),
      Token(TokenType::OPER_STAR,           30, 1),
      Token(TokenType::OPER_STAR,           31, 1),
      Token(TokenType::IDENTIFIER,          32, 4),
      Token(TokenType::SPECIAL_PAREN_CLOSE, 36, 1),
      Token(TokenType::SPECIAL_BRACE_OPEN,  38, 1),
      Token(TokenType::NEWLINE,             39, 1),

      Token(TokenType::KEYWORD_RETURN,      42, 6),
      Token(TokenType::LITERAL_NUM,         49, 1),
      Token(TokenType::SPECIAL_SEMICOLON,   50, 1),
      Token(TokenType::NEWLINE,             51, 1),

      Token(TokenType::SPECIAL_BRACE_CLOSE, 52, 1),
      Token(TokenType::NEWLINE,             53, 1),
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/main.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      Token(TokenType::KEYWORD_CHAR,       0, 4),
      Token(TokenType::IDENTIFIER,         5, 1),
      Token(TokenType::OPER_ASSIGN,        7, 1),
      Token(TokenType::LITERAL_CHAR,       9, 3),
      Token(TokenType::SPECIAL_SEMICOLON, 12, 1),
      Token(TokenType::NEWLINE,           13, 1),

      Token(TokenType::KEYWORD_CHAR,      14, 4),
      Token(TokenType::IDENTIFIER,        19, 1),
      Token(TokenType::OPER_ASSIGN,       21, 1),
      Token(TokenType::LITERAL_CHAR,      23, 4),
      Token(TokenType::SPECIAL_SEMICOLON, 27, 1),
      Token(TokenType::NEWLINE,           28, 1),

      Token(TokenType::KEYWORD_CHAR,      29, 4),
      Token(TokenType::IDENTIFIER,        34, 1),
      Token(TokenType::OPER_ASSIGN,       36, 1),
      Token(TokenType::LITERAL_CHAR,      38, 4),
      Token(TokenType::SPECIAL_SEMICOLON, 42, 1),
      Token(TokenType::NEWLINE,           43, 1),

      Token(TokenType::KEYWORD_CHAR,      44, 4),
      Token(TokenType::IDENTIFIER,        49, 1),
      Token(TokenType::OPER_ASSIGN,       51, 1),
      Token(TokenType::LITERAL_CHAR,      53, 4),
      Token(TokenType::SPECIAL_SEMICOLON, 57, 1),
      Token(TokenType::NEWLINE,           58, 1),
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/char_literals.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }
  {
    std::vector<lexer::Token> const expected {
      Token(TokenType::KEYWORD_CHAR,       0, 4),
      Token(TokenType::OPER_STAR,          5, 1),
      Token(TokenType::IDENTIFIER,         6, 1),
      Token(TokenType::OPER_ASSIGN,        8, 1),
      Token(TokenType::LITERAL_STR,       10, 2),
      Token(TokenType::SPECIAL_SEMICOLON, 12, 1),
      Token(TokenType::NEWLINE,           13, 1),

      Token(TokenType::KEYWORD_CHAR,      14, 4),
      Token(TokenType::OPER_STAR,         19, 1),
      Token(TokenType::IDENTIFIER,        20, 1),
      Token(TokenType::OPER_ASSIGN,       22, 1),
      Token(TokenType::LITERAL_STR,       24, 4),
      Token(TokenType::SPECIAL_SEMICOLON, 28, 1),
      Token(TokenType::NEWLINE,           29, 1),

      Token(TokenType::KEYWORD_CHAR,      30, 4),
      Token(TokenType::OPER_STAR,         35, 1),
      Token(TokenType::IDENTIFIER,        36, 1),
      Token(TokenType::OPER_ASSIGN,       38, 1),
      Token(TokenType::LITERAL_STR,       40, 5),
      Token(TokenType::SPECIAL_SEMICOLON, 45, 1),
      Token(TokenType::NEWLINE,           46, 1),

      Token(TokenType::KEYWORD_CHAR,      47, 4),
      Token(TokenType::OPER_STAR,         52, 1),
      Token(TokenType::IDENTIFIER,        53, 1),
      Token(TokenType::OPER_ASSIGN,       55, 1),
      Token(TokenType::LITERAL_STR,       57, 6),
      Token(TokenType::SPECIAL_SEMICOLON, 63, 1),
      Token(TokenType::NEWLINE,           64, 1),
    };
    std::string const text = util::extract_txt_file_contents("test_files/tiny/string_literals.c");
    std::vector<lexer::Token> const actual = lexer::tokenize_text(text.c_str(), text.length());
    ntest::assert_stdvec(expected, actual);
  }

  // report output
  {
    auto const res = ntest::generate_report("fmtcpp", [](ntest::assertion const &a, bool const passed) {
      if (!passed)
        util::print_err("failed: %s:%zu", a.loc.file_name(), a.loc.line());
    });

    if ((res.num_fails + res.num_passes) == 0) {
      printf(FG_BRIGHT_YELLOW, "No tests defined");
    } else {
      if (res.num_fails > 0) {
        printf(FG_BRIGHT_RED,   "%zu failed", res.num_fails);
        std::printf(" | ");
        printf(FG_BRIGHT_GREEN, "%zu passed", res.num_passes);
      } else
        printf(FG_BRIGHT_GREEN, "All %zu tests passed", res.num_passes);
    }
    std::printf("\n\n");
  }

  return 0;
}
