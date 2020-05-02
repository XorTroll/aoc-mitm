
#pragma once
#include <stratosphere.hpp>
#include <aoc/aoc_Log.hpp>

namespace impl {

    static inline ams::os::Mutex g_logging_lock(true);

}

#define AOC_ASSERT(rc) ({ \
    auto _rc = static_cast<::ams::Result>(rc); \
    if(R_FAILED(_rc)) { \
        fatalThrow(_rc.GetValue()); \
    } \
})
