// various utility/helper functions

#ifndef FMTCPP_UTIL_HPP
#define FMTCPP_UTIL_HPP

#include <fstream>
#include <string>
#include <vector>

namespace util {

std::string make_str(char const *fmt, ...);

int print_err(char const *fmt, ...);

bool is_alphabetic(char);
bool is_digit(char);
bool is_non_newline_whitespace(char);

void escape_escape_sequences(std::string &);

size_t find_unescaped(
  char const *str,
  char searchCh,
  char escapeCh,
  size_t startOffset = 0
);

std::fstream open_file(char const *path, int flags);
std::vector<char> extract_bin_file_contents(char const *path);
std::string extract_txt_file_contents(char const *path);

// Returns the size of a static C-style array at compile time.
template <typename ElemTy, size_t Length>
consteval size_t lengthof(ElemTy (&)[Length]) { return Length; }

template <typename Ty>
bool is_even(Ty const num) {
  return num % 2 == 0;
}

} // namespace util

#endif // FMTCPP_UTIL_HPP
