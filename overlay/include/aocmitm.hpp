
#pragma once
#include <switch.h>

namespace aocmitm {

    bool IsAvailable();

    Result Initialize();
    void Exit();

    bool IsApplicationInMitmList(u64 app_id);
    void AddApplicationToMitmList(u64 app_id);
    void RemoveApplicationFromMitmList(u64 app_id);

}