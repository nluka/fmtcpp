#define _CRT_SECURE_NO_WARNINGS

#include <stdexcept>
#include <string_view>
#include <cstring>
#include <unordered_map>

#include "lexer.hpp"
#include "util.hpp"

lexer::Token::Token(TokenType const type, uint32_t const pos, uint32_t const len)
: m_type{type},
  m_pos{pos},
  m_len{len}
{}

lexer::TokenType lexer::Token::type() const noexcept { return m_type; }
uint32_t lexer::Token::position() const noexcept { return m_pos; }
uint32_t lexer::Token::length() const noexcept { return m_len; }

void lexer::Token::set_type(TokenType const v) { m_type = v; }
void lexer::Token::set_position(uint32_t const v) { m_pos = v; }
void lexer::Token::set_length(uint32_t const v) { m_len = v; }

bool lexer::Token::operator==(lexer::Token const &other) const noexcept {
  bool const sameType = m_type == other.m_type;
  bool const samePos = m_pos == other.m_pos;
  bool const sameLen = m_len == other.m_len;
  bool const equal = sameType && samePos && sameLen;
  return equal;
}

bool lexer::Token::operator!=(lexer::Token const &other) const noexcept {
  return !(*this == other);
}

namespace lexer {
  std::ostream &operator<<(std::ostream &os, lexer::Token const &tok) {
    os << '(' << static_cast<uint16_t>(tok.m_type) << ' ' << tok.m_pos << ' ' << tok.m_len << ')';
    return os;
  }
}

std::vector<lexer::Token> lexer::tokenize_text(char const *const text, size_t const textLen) {
  using lexer::TokenType;
  using lexer::Token;

  std::vector<Token> tokens{};

  // not very scientific, but from experimentation seems reasonable
  tokens.reserve(textLen / 3);

  // first pass, collection phase
  {
    size_t pos = 0;
    while (pos < textLen) {
      Token const tok = detail::extract_token(text, textLen, pos);
      if (tok.type() == TokenType::NIL)
        break;
      else {
        pos += tok.length();
        tokens.push_back(tok);
      }
    }
  }

  // second pass, compression phrase
  for (size_t i = 0; i < tokens.size();) {
    switch (tokens[i].type()) {
      default: {
        ++i;
        break;
      }

      case TokenType::LITERAL_CHAR:
      case TokenType::LITERAL_STR: {
        auto const prevToken = tokens.begin() + ptrdiff_t(i - 1ull);
        bool const isPrefixed =
          prevToken->type() == TokenType::IDENTIFIER &&
          prevToken->length() == 1 &&
          std::strchr("LuU", text[prevToken->position()]);

        if (isPrefixed) {
          //   L  'c'
          //   ^  ^
          //   |  |
          // i-1  i
          // ------
          // we need to replace tokens [i-1, i] as a single
          // LITERAL_CHAR

          auto &currToken = tokens[i];
          currToken.set_position(currToken.position() - 1);
          currToken.set_length(currToken.length() + 1);
          tokens.erase(prevToken);
        }

        ++i;
        break;
      }
    }
  }

  return tokens;
}

lexer::Token lexer::detail::extract_token(
  char const *const text,
  size_t const textLen,
  size_t &pos
) {
  // advance `pos` to beginning of next token:
  for (; pos < textLen && util::is_non_newline_whitespace(text[pos]); ++pos);

  char const *const firstChar = text + pos;

  lexer::detail::BroadTokenType const broadTokType =
    lexer::detail::determine_token_broad_type(*firstChar);

  size_t const tokLen = lexer::detail::determine_token_len(
    firstChar, broadTokType, textLen - pos);

  lexer::TokenType const tokType = lexer::detail::determine_token_type(
    firstChar, broadTokType, tokLen);

  return {
    tokType,
    static_cast<uint32_t>(pos),
    static_cast<uint32_t>(tokLen),
  };
}

lexer::detail::BroadTokenType lexer::detail::determine_token_broad_type(char const firstChar) {
  using lexer::detail::BroadTokenType;

  switch (firstChar) {
    case '\n': return BroadTokenType::NEWLINE;
    case '#':  return BroadTokenType::PREPRO;
    case '.':  return BroadTokenType::OPER_OR_LITERAL_OR_SPECIAL;
    case '/':  return BroadTokenType::OPER_OR_COMMENT;
    default:   break;
  }

  if (firstChar == '_' || util::is_alphabetic(firstChar)) {
    return BroadTokenType::KEYWORD_OR_IDENTIFIER;
  }
  if (firstChar == '"' || firstChar == '\'' || util::is_digit(firstChar)) {
    return BroadTokenType::LITERAL;
  }
  if (std::strchr("!%&*+-<=>^|~", firstChar)) {
    return BroadTokenType::OPERATOR;
  }
  if (std::strchr("(),:;?[\\]{}", firstChar)) {
    return BroadTokenType::SPECIAL;
  }

  return BroadTokenType::NIL;
}

static
size_t find_numeric_literal_len(
  char const *const firstChar,
  size_t const numCharsRemaining
) {
  // interpreting numeric literals in C is bloody complicated,
  // luckily we don't have to do that so we can be pretty lax.
  // here are some useful references:
  // https://web.archive.org/web/20181230041359if_/http://www.open-std.org/jtc1/sc22/wg14/www/abq/c17_updated_proposed_fdis.pdf
  // https://www.educba.com/c-literals/

  // numeric literals start must with either:
  // - a dot (.) indicating a floating-point literal
  // - a digit (0-9) indicating a decimal|hex|octal|binary|floating-point literal
  if (!util::is_digit(*firstChar) && *firstChar != '.')
    // this is not a numeric literal!
    return 0;

  size_t pos = 1;
  #define CURRCHAR *(firstChar + pos)

keep_going:
  while (
    pos < numCharsRemaining && (
      util::is_digit(CURRCHAR) ||
      util::is_alphabetic(CURRCHAR) ||
      std::strchr("'.", CURRCHAR)
    )
  ) ++pos;

  if (!std::strchr("+-", CURRCHAR))
    return pos;

  // might be scientific notation...
  char const prevChar = *(firstChar + pos - 1);
  if (std::tolower(prevChar) == 'e' && pos >= 3) {
    char const prevPrevChar = *(firstChar + pos - 2);
    if (std::tolower(prevPrevChar) != 'x') {
      // it is!
      ++pos;
      goto keep_going;
    }
  }

  // nope, it's not
  return pos;

  #undef CURRCHAR
}

size_t lexer::detail::determine_token_len(
  char const *const firstChar,
  lexer::detail::BroadTokenType const broadTokType,
  size_t const numCharsRemaining
) {
  using lexer::detail::BroadTokenType;

  if (numCharsRemaining == 0)
    return 0;

  switch (broadTokType) {
    case BroadTokenType::NEWLINE:
    case BroadTokenType::SPECIAL:
      return 1;

    case BroadTokenType::PREPRO: {
      if (numCharsRemaining == 1)
        return 1;

      {
        char const secondChar = *(firstChar + 1);
        if (secondChar == '#')
          return 2; // ##
      }

      size_t const firstNewlinePos = util::find_unescaped(firstChar, '\n', '\\');
      char const *const firstOpeningMultiLineComment = std::strstr(firstChar, "/*");
      size_t const firstOpeningMultiLineCommentPos =
        firstOpeningMultiLineComment == nullptr
          ? size_t(UINT32_MAX)
          : size_t(firstOpeningMultiLineComment - firstChar);

      if (firstOpeningMultiLineCommentPos < firstNewlinePos) {
        char const *const commentClose = std::strstr(firstChar, "*/");
        return size_t(commentClose - firstChar) + 2ull;
      } else {
        return firstNewlinePos;
      }
    }

    case BroadTokenType::OPER_OR_LITERAL_OR_SPECIAL: {
      if (numCharsRemaining == 1)
        return 1; // member selection operator

      if (
        numCharsRemaining >= 3 &&
        (*firstChar == '.') &&
        (*(firstChar + 1) == '.') &&
        (*(firstChar + 2) == '.')
      ) return 3; // ellipses

      // must be a floating-point literal then...

      char const *lastChar = firstChar + 1;

      // advance `lastChar` to first non-digit character
      while (
        static_cast<size_t>(lastChar - firstChar) <= numCharsRemaining &&
        util::is_digit(*lastChar)
      ) ++lastChar;

      if (*lastChar == 'f' || *lastChar == 'F')
        return size_t(lastChar - firstChar) + 1ull;
      else
        return size_t(lastChar - firstChar);
    }

    case BroadTokenType::OPER_OR_COMMENT: {
      if (numCharsRemaining == 1)
        return 1;

      char const secondChar = *(firstChar + 1);
      switch (secondChar) {
        case '=':
          return 2;
        case '/': {
          char const *firstUnescapedNewline = firstChar + 2; // start at 3rd character
          while (true) {
            firstUnescapedNewline = std::strchr(firstUnescapedNewline, '\n');
            if (firstUnescapedNewline == nullptr)
              // couldn't find a newline, comment extends to end of file
              return numCharsRemaining;
            else if (*(firstUnescapedNewline - 1) != '\\')
              // found an unescaped newline
              return size_t(firstUnescapedNewline - firstChar);
            else
              // found a newline, but it's escaped
              ++firstUnescapedNewline;
          }
        }
        case '*': {
          std::string_view const content(firstChar);
          size_t const endSequencePos = content.find("*/");
          if (endSequencePos == std::string_view::npos)
            return numCharsRemaining;
          else
            return endSequencePos + 2;
        }
        default:
          return 1;
      }
    }

    case BroadTokenType::KEYWORD_OR_IDENTIFIER: {
      for (size_t pos = 1; pos <= numCharsRemaining; ++pos) {
        char const c = *(firstChar + pos);
        if (!util::is_alphabetic(c) && !util::is_digit(c) && c != '_')
          return pos;
      }
      return numCharsRemaining;
    }

    case BroadTokenType::OPERATOR: {
      if (numCharsRemaining == 1 || *firstChar == '~') {
        return 1;
      }

      char const secondChar = *(firstChar + 1);

      switch (*firstChar) {
        case '+': {
          if (std::strchr("+=", secondChar))
            return 2;
          else
            return 1;
        }

        case '-': {
          if (std::strchr("-=>", secondChar))
            return 2;
          else
            return 1;
        }

        case '*':
        case '%':
        case '=':
        case '!':
        case '^':
          return secondChar == '=' ? 2 : 1;

        case '<':
        case '>': {
          if (secondChar == '=') {
            // >= <=
            return 2;
          } else if (secondChar == *firstChar) {
            // >> <<
            if (numCharsRemaining == 2)
              return 2;
            else {
              char const thirdChar = *(firstChar + 2);
              if (thirdChar == '=')
                // >>= <<=
                return 3;
              else
                return 2;
            }
          } else {
            return 1;
          }
        }

        case '&':
        case '|': {
          if (secondChar == *firstChar || secondChar == '=')
            // && &= || |=
            return 2;
          else
            return 1;
        }

        default:
          return 0;
      }
    }

    case BroadTokenType::LITERAL: {
      if (numCharsRemaining == 1) {
        return 1;
      } else if (*firstChar == '\'' || *firstChar == '"') {
        // string or character literal
        size_t const closingCharPos = util::find_unescaped(
          firstChar, *firstChar, '\\', 1
        );
        if (closingCharPos == std::string::npos)
          // string/character literal is unclosed, we don't care
          return numCharsRemaining;
        else
          return closingCharPos + 1;
      } else {
        return find_numeric_literal_len(firstChar, numCharsRemaining);
      }
    }

    default:
      return 0;
  }
}

lexer::TokenType lexer::detail::determine_token_type(
  char const *const firstChar,
  lexer::detail::BroadTokenType const broadTokType,
  size_t const len
) {
  using lexer::detail::BroadTokenType;
  using lexer::TokenType;

  static std::unordered_map<std::string, TokenType> const s_preproDirectives {
    { "include", TokenType::PREPRO_DIR_INCLUDE },
    { "define", TokenType::PREPRO_DIR_DEFINE },
    { "undef", TokenType::PREPRO_DIR_UNDEF },
    { "ifdef", TokenType::PREPRO_DIR_IFDEF },
    { "ifndef", TokenType::PREPRO_DIR_IFNDEF },
    { "if", TokenType::PREPRO_DIR_IF },
    { "elif", TokenType::PREPRO_DIR_ELIF },
    { "else", TokenType::PREPRO_DIR_ELSE },
    { "endif", TokenType::PREPRO_DIR_ENDIF },
    { "error", TokenType::PREPRO_DIR_ERROR },
    { "pragma", TokenType::PREPRO_DIR_PRAGMA },
  };
  static std::unordered_map<std::string, TokenType> const s_keywordsOperatorsSpecialSymbols {
    { "auto", TokenType::KEYWORD_AUTO },
    { "break", TokenType::KEYWORD_BREAK },
    { "case", TokenType::KEYWORD_CASE },
    { "char", TokenType::KEYWORD_CHAR },
    { "const", TokenType::KEYWORD_CONST },
    { "continue", TokenType::KEYWORD_CONTINUE },
    { "default", TokenType::KEYWORD_DEFAULT },
    { "do", TokenType::KEYWORD_DO },
    { "double", TokenType::KEYWORD_DOUBLE },
    { "else", TokenType::KEYWORD_ELSE },
    { "enum", TokenType::KEYWORD_ENUM },
    { "extern", TokenType::KEYWORD_EXTERN },
    { "float", TokenType::KEYWORD_FLOAT },
    { "for", TokenType::KEYWORD_FOR },
    { "goto", TokenType::KEYWORD_GOTO },
    { "if", TokenType::KEYWORD_IF },
    { "inline", TokenType::KEYWORD_INLINE },
    { "int", TokenType::KEYWORD_INT },
    { "long", TokenType::KEYWORD_LONG },
    { "register", TokenType::KEYWORD_REGISTER },
    { "restrict", TokenType::KEYWORD_RESTRICT },
    { "return", TokenType::KEYWORD_RETURN },
    { "short", TokenType::KEYWORD_SHORT },
    { "signed", TokenType::KEYWORD_SIGNED },
    { "sizeof", TokenType::KEYWORD_SIZEOF },
    { "static", TokenType::KEYWORD_STATIC },
    { "struct", TokenType::KEYWORD_STRUCT },
    { "switch", TokenType::KEYWORD_SWITCH },
    { "typedef", TokenType::KEYWORD_TYPEDEF },
    { "union", TokenType::KEYWORD_UNION },
    { "unsigned", TokenType::KEYWORD_UNSIGNED },
    { "void", TokenType::KEYWORD_VOID },
    { "volatile", TokenType::KEYWORD_VOLATILE },
    { "while", TokenType::KEYWORD_WHILE },
    { "_Alignas", TokenType::KEYWORD_ALIGNAS },
    { "_Alignof", TokenType::KEYWORD_ALIGNOF },
    { "_Atomic", TokenType::KEYWORD_ATOMIC },
    { "_Bool", TokenType::KEYWORD_BOOL },
    { "_Complex", TokenType::KEYWORD_COMPLEX },
    { "_Generic", TokenType::KEYWORD_GENERIC },
    { "_Imaginary", TokenType::KEYWORD_IMAGINARY },
    { "_Noreturn", TokenType::KEYWORD_NORETURN },
    { "_Static_assert", TokenType::KEYWORD_STATICASSERT },
    { "_Thread_local", TokenType::KEYWORD_THREADLOCAL },
    { "+", TokenType::OPER_PLUS },
    { "++", TokenType::OPER_PLUSPLUS },
    { "-", TokenType::OPER_MINUS },
    { "--", TokenType::OPER_MINUSMINUS },
    { "%", TokenType::OPER_MOD },
    { "=", TokenType::OPER_ASSIGN },
    { "+=", TokenType::OPER_ASSIGN_ADD },
    { "-=", TokenType::OPER_ASSIGN_SUB },
    { "*=", TokenType::OPER_ASSIGN_MULT },
    { "%=", TokenType::OPER_ASSIGN_MOD },
    { "==", TokenType::OPER_REL_EQ },
    { "!=", TokenType::OPER_REL_NOTEQ },
    { "<", TokenType::OPER_REL_LESSTHAN },
    { "<=", TokenType::OPER_REL_LESSTHANEQ },
    { ">", TokenType::OPER_REL_GREATERTHAN },
    { ">=", TokenType::OPER_REL_GREATERTHANEQ },
    { "&&", TokenType::OPER_LOGIC_AND },
    { "||", TokenType::OPER_LOGIC_OR },
    { "!", TokenType::OPER_LOGIC_NOT },
    { "~", TokenType::OPER_BITWISE_NOT },
    { "&=", TokenType::OPER_ASSIGN_BITAND },
    { "|", TokenType::OPER_BITWISE_OR },
    { "|=", TokenType::OPER_ASSIGN_BITOR },
    { "^", TokenType::OPER_BITWISE_XOR },
    { "^=", TokenType::OPER_ASSIGN_BITXOR },
    { "<<", TokenType::OPER_BITWISE_SHIFTLEFT },
    { "<<=", TokenType::OPER_ASSIGN_BITSHIFTLEFT },
    { ">>", TokenType::OPER_BITWISE_SHIFTRIGHT },
    { ">>=", TokenType::OPER_ASSIGN_BITSHIFTRIGHT },
    { "->", TokenType::OPER_ARROW },
    { "&", TokenType::OPER_AMPERSAND },
    { "*", TokenType::OPER_STAR },
    { "(", TokenType::SPECIAL_PAREN_OPEN },
    { ")", TokenType::SPECIAL_PAREN_CLOSE },
    { "{", TokenType::SPECIAL_BRACE_OPEN },
    { "}", TokenType::SPECIAL_BRACE_CLOSE },
    { "[", TokenType::SPECIAL_BRACKET_OPEN },
    { "]", TokenType::SPECIAL_BRACKET_CLOSE },
    { "?", TokenType::SPECIAL_QUESTION },
    { ":", TokenType::SPECIAL_COLON },
    { "...", TokenType::SPECIAL_ELLIPSES },
    { ",", TokenType::SPECIAL_COMMA },
    { ";", TokenType::SPECIAL_SEMICOLON },
    { "\\", TokenType::SPECIAL_LINE_CONT },
  };

  switch (broadTokType) {
    default:
    case BroadTokenType::NIL:
      return TokenType::NIL;

    case BroadTokenType::NEWLINE:
      return TokenType::NEWLINE;

    case BroadTokenType::OPER_OR_LITERAL_OR_SPECIAL: {
      if (len == 1)
        return TokenType::OPER_DOT;
      if (len == 3 && std::strcmp(firstChar, "...") == 0)
        return TokenType::SPECIAL_ELLIPSES;
      return TokenType::LITERAL_NUM;
    }

    case BroadTokenType::OPER_OR_COMMENT: {
      if (len == 1)
        return TokenType::OPER_DIV;

      char const secondChar = *(firstChar + 1);
      switch (secondChar) {
        case '=':
          return TokenType::OPER_ASSIGN_DIV;
        case '/':
          return TokenType::COMMENT_SINGLELINE;
        case '*':
          return TokenType::COMMENT_MULTILINE;
        default:
          return TokenType::NIL;
      }
    }

    case BroadTokenType::LITERAL: {
      switch (*firstChar) {
        case '"': return TokenType::LITERAL_STR;
        case '\'': return TokenType::LITERAL_CHAR;
        default: return TokenType::LITERAL_NUM;
      }
    }

    case BroadTokenType::PREPRO: {
      if (len == 2)
        return TokenType::PREPRO_OPER_CONCAT;

      // directives can have whitespace between the # and the letters:
      // #   define
      //  ^^^
      //  we must account for this

      char const *firstAlphabeticChar = firstChar + 1;
      while (!util::is_alphabetic(*firstAlphabeticChar))
        ++firstAlphabeticChar;

      // #   define
      // ^   ^
      // |   |
      // |   firstAlphabeticChar
      // firstChar

      size_t directiveLen = 1;
      {
        char const *p = firstAlphabeticChar + 1;
        while (util::is_alphabetic(*p++))
          ++directiveLen;
      }

      std::string const directive( // content should be "include", "define", etc.
        firstAlphabeticChar,
        directiveLen
      );

      auto const type = s_preproDirectives.find(directive);
      if (type == s_preproDirectives.end())
        return TokenType::NIL;
      else
        return type->second;
    }

    case BroadTokenType::KEYWORD_OR_IDENTIFIER:
    case BroadTokenType::OPERATOR:
    case BroadTokenType::SPECIAL: {
      std::string const token(firstChar, len);
      auto const type = s_keywordsOperatorsSpecialSymbols.find(token);
      if (type == s_keywordsOperatorsSpecialSymbols.end())
        return TokenType::IDENTIFIER;
      else
        return type->second;
    }
  }
}
