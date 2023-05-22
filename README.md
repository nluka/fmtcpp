# fmtcpp

## Findings
- Writing a custom C++ parser is a tremendous amount of work
  - It might make sense to use a library for this, such as [CppParser](https://github.com/satya-das/cppparser)
- It might be possible to extract symbols from a #include file using libclang, which makes the necessary symbol table work easier
  - Would be very helpful if source location info is included, to determine the "nature" of a macro (e.g. type alias, code block) in edge cases where simply knowing that an identifier is a macro is not enough info to format with confidence
