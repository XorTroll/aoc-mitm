#include <aoc/aoc_Log.hpp>
#include <stratosphere.hpp>
#include <cstdio>

namespace aoc {

    static ams::os::Mutex g_log_lock(true);
    static bool has_cleared_logs = false;
    static constexpr const char LogFile[] = "sdmc:/aoc-mitm.log";

    inline static void ClearLogs() {
        std::scoped_lock lk(g_log_lock);
        remove(LogFile);
    }

    void LogBase(const char *fmt, std::va_list args) {
        std::scoped_lock lk(g_log_lock);
        if(!has_cleared_logs) {
            ClearLogs();
            has_cleared_logs = true;
        }
        auto file = fopen(LogFile, "a+");
        if(file) {
            vfprintf(file, fmt, args);
            fclose(file);
        }
    }

}