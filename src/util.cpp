#include <cstring>
#include <cstdarg>
#include <filesystem>
#include <iostream>
#include <utility>
#include <cassert>

#include "term.hpp"
#include "util.hpp"

bool util::is_alphabetic(char const c) {
  return
    (c >= 'A' && c <= 'Z') ||
    (c >= 'a' && c <= 'z');
}

bool util::is_digit(char const c) {
  return (c >= '0' && c <= '9');
}

bool util::is_non_newline_whitespace(char const c) {
  return strchr(" \t\v\f", c);
}

void util::escape_escape_sequences(std::string &str) {
  std::pair<char, char> const sequences[] {
    { '\a', 'a' },
    { '\b', 'b' },
    { '\f', 'f' },
    { '\n', 'n' },
    { '\r', 'r' },
    { '\t', 't' },
    { '\v', 'v' },
  };

  for (size_t i = 0; i < str.length(); ++i) {
    char *const c = str.data() + i;

    for (auto const &seq : sequences) {
      if (*c == seq.first) {
        *c = seq.second;
        str.insert(i, "\\");
        ++i; // to account for inserted "\\"
        break;
      }
    }
  }
}

size_t util::find_unescaped(
  char const *const str,
  char const searchCh,
  char const escapeCh,
  size_t const offset
) {
  size_t const len = strlen(str);

  for (size_t pos = offset; pos < len; ++pos) {
    if (str[pos] != searchCh) {
      continue;
    }

    auto const escapeCount = std::count(str, str + pos - 1, escapeCh);

    bool const isEscaped = !is_even(escapeCount);
    if (!isEscaped) {
      return pos;
    }
  }

  return std::string::npos;
}

std::fstream util::open_file(char const *const path, int const flags) {
  using util::make_str;

  bool const forReading = (flags & 1) == 1;
  if (forReading) {
    if (!std::filesystem::exists(path))
      throw std::runtime_error(make_str("file '%s' not found", path));
  }

  std::fstream file(path, static_cast<std::ios_base::openmode>(flags));

  if (!file.is_open())
    throw std::runtime_error(make_str("unable to open file '%s'", path));

  if (!file.good())
    throw std::runtime_error(make_str("file '%s' in bad state", path));

  return file;
}

std::vector<char> util::extract_bin_file_contents(char const *const path) {
  std::fstream file = util::open_file(path, std::ios::binary | std::ios::in);

  auto const fileSize = std::filesystem::file_size(path);
  assert(fileSize <= std::numeric_limits<size_t>::max());

  std::vector<char> vec(fileSize);

  file.read(vec.data(), std::streamsize(fileSize));

  return vec;
}

std::string util::extract_txt_file_contents(char const *const path) {
  std::fstream file = util::open_file(path, std::ios::in);
  auto const fileSize = std::filesystem::file_size(path);

  std::string content{};
  content.reserve(fileSize);

  std::getline(file, content, '\0');

  // remove any \r characters
  content.erase(
    remove(content.begin(), content.end(), '\r'),
    content.end()
  );

  return content;
}

std::string util::make_str(char const *const fmt, ...)
{
  size_t const bufLen = 1024;
  char buffer[bufLen];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  return std::string(buffer);
}

int util::print_err(char const *fmt, ...)
{
  using namespace term;

  term::set_font_effects(FG_RED);

  va_list args;
  va_start(args, fmt);
  int const retval = vprintf(fmt, args);
  va_end(args);

  term::reset_font_effects();

  putc('\n', stdout);

  return retval;
}
