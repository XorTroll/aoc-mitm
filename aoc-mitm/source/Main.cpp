#include <aoc/srv/srv_IAddOnContentManager.hpp>
#include <aoc/mitm/mitm_IMitmService.hpp>

extern "C" {

    extern u32 __start__;
    u32 __nx_applet_type = AppletType_None;
    u32 __nx_fs_num_sessions = 1;

    #define INNER_HEAP_SIZE 0x10000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char   nx_inner_heap[INNER_HEAP_SIZE];
    void __libnx_init_time(void);

    void __libnx_initheap(void) {
        void *addr = nx_inner_heap;
        size_t size = nx_inner_heap_size;
        extern char *fake_heap_start;
        extern char *fake_heap_end;
        fake_heap_start = (char*)addr;
        fake_heap_end = (char*)addr + size;
    }

    void __appInit(void) {
        ams::sm::DoWithSession([&]() {
            AOC_ASSERT(fsInitialize());
            AOC_ASSERT(fsdevMountSdmc());
            AOC_ASSERT(pminfoInitialize());
            AOC_ASSERT(pmdmntInitialize());
        });
    }

    void __appExit(void) {
        pmdmntExit();
        pminfoExit();
        fsdevUnmountAll();
        fsExit();
    }

}

namespace ams {

    ncm::ProgramId CurrentProgramId = { 0x0100000000000523ul };

    namespace result {

        bool CallFatalOnResultAssertion = true;

    }

}

namespace {

    constexpr size_t ServerNum = 2;
    constexpr size_t AocMitmMaxSessions = 0x10;
    
    ams::sf::hipc::ServerManager<ServerNum> g_aoc_manager;

}

int main() {
    // Register aoc:u
    AOC_ASSERT(g_aoc_manager.RegisterMitmServer<aoc::srv::IAddOnContentManager>(aoc::srv::ServiceName));

    // Register aocmitm
    AOC_ASSERT(g_aoc_manager.RegisterServer<aoc::mitm::IMitmService>(aoc::mitm::ServiceName, AocMitmMaxSessions));
 
    g_aoc_manager.LoopProcess();
 
    return 0;
}