
#pragma once
#include <cstring>
#include <cstdarg>

namespace aoc {

    void LogBase(const char *fmt, std::va_list args);

    inline void LogFmt(const char *fmt, ...) {
        std::va_list args;
        va_start(args, fmt);
        LogBase(fmt, args);
        va_end(args);
    }

}

#define AOC_LOG_FMT(fmt, ...) ::aoc::LogFmt(fmt "\n", ##__VA_ARGS__)