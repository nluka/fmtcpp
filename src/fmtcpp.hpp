#ifndef FMTCPP_PARSER_HPP
#define FMTCPP_PARSER_HPP

#include <string>
#include <clang-c/Index.h>

namespace fmtcpp {

void print_nodes(std::string const &cpp_source_code, std::ostream &os);

std::string format_source_code(std::string const &cpp_source_code);

} // namespace fmtcpp

#endif // FMTCPP_PARSER_HPP
