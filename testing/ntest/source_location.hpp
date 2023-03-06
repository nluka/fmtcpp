// source_location standard header (core)

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef _SOURCE_LOCATION_
#define _SOURCE_LOCATION_
#ifndef __cpp_consteval
#pragma message("The contents of <source_location> are available only with C++20 consteval support.")
#else // ^^^ !defined(__cpp_consteval) / defined(__cpp_consteval) vvv

#include <cstdint>

namespace std {

struct source_location {
    static consteval source_location current(const uint_least32_t _Line_ = __builtin_LINE(),
        const uint_least32_t _Column_ = __builtin_LINE() , const char* const _File_ = __builtin_FILE(),
        const char* const _Function_ = __builtin_FUNCTION()) noexcept {
        source_location _Result;
        _Result._Line     = _Line_;
        _Result._Column   = _Column_;
        _Result._File     = _File_;
        _Result._Function = _Function_;
        return _Result;
    }

    constexpr source_location() noexcept = default;

    constexpr uint_least32_t line() const noexcept {
        return _Line;
    }
    constexpr uint_least32_t column() const noexcept {
        return _Column;
    }
    constexpr const char* file_name() const noexcept {
        return _File;
    }
     constexpr const char* function_name() const noexcept {
        return _Function;
    }

private:
    uint_least32_t _Line{};
    uint_least32_t _Column{};
    const char* _File     = "";
    const char* _Function = "";
};

}

#endif // !defined(__cpp_consteval)
#endif // _SOURCE_LOCATION_