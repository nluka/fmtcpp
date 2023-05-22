#include <iostream>
#include <cassert>

#include "fmtcpp.hpp"
#include "util.hpp"

static std::ostream *s_os = nullptr;

CXChildVisitResult print_ast_node(
  CXCursor cursor,
  [[maybe_unused]] CXCursor parent,
  [[maybe_unused]] CXClientData client_data
) {
  CXSourceLocation location = clang_getCursorLocation(cursor);
  CXFile file;
  unsigned int line, column, offset;
  clang_getSpellingLocation(location, &file, &line, &column, &offset);
  CXCursorKind cursor_kind = clang_getCursorKind(cursor);
  CXString cursor_kind_spelling = clang_getCursorKindSpelling(cursor_kind);
  CXString cursor_spelling = clang_getCursorSpelling(cursor);
  CXString file_name = clang_getFileName(file);

  assert(s_os != nullptr);

  {
    char const *const file_path = clang_getCString(file_name);
    if (file_path != nullptr)
      *s_os << clang_getCString(file_name) << ':' << line << ',' << column << "   ";
  }

  *s_os << clang_getCString(cursor_kind_spelling) << "   " << clang_getCString(cursor_spelling) << '\n';

  clang_disposeString(cursor_kind_spelling);
  clang_disposeString(cursor_spelling);
  clang_disposeString(file_name);

  // Recursively visit the children of the current node
  clang_visitChildren(cursor, print_ast_node, nullptr);

  return CXChildVisit_Continue;
}

void fmtcpp::print_nodes(std::string const &cpp_source_code, std::ostream &os) {

  s_os = &os;

  CXIndex index = clang_createIndex(0, 0);

  CXUnsavedFile unsaved_file {
    "unsaved.cpp",
    cpp_source_code.c_str(),
    cpp_source_code.length()
  };

  char const *arguments[] {
    "-std=c++11",
    "-fparse-all-comments",
  };

  CXTranslationUnit transl_unit;

  CXErrorCode const ec = clang_parseTranslationUnit2(
    index,
    "unsaved.cpp",
    arguments,
    util::lengthof(arguments),
    &unsaved_file, 1,
    CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_PrecompiledPreamble,
    &transl_unit
  );

  *s_os << "clang_parseTranslationUnit2 CXErrorCode = " << ec << '\n';

  CXCursor root_cursor = clang_getTranslationUnitCursor(transl_unit);
  clang_visitChildren(root_cursor, print_ast_node, nullptr);

  clang_disposeTranslationUnit(transl_unit);
  clang_disposeIndex(index);
}

std::string fmtcpp::format_source_code([[maybe_unused]] std::string const &cpp_source_code) {
  return "";
}
