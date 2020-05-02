
#pragma once
#include <aoc/aoc_Types.hpp>

namespace aoc::mitm::impl {

    bool IsApplicationInMitmList(ams::ncm::ApplicationId app_id);
    void AddApplicationToMitmList(ams::ncm::ApplicationId app_id);
    void RemoveApplicationFromMitmList(ams::ncm::ApplicationId app_id);

    inline bool ShouldMitmApplication(ams::ncm::ApplicationId app_id) {
        auto mitm = IsApplicationInMitmList(app_id);
        if(mitm) {
            RemoveApplicationFromMitmList(app_id);
        }
        return mitm;
    }

    void SetMitmdApplication(ams::ncm::ApplicationId app_id);
    ams::ncm::ApplicationId GetMitmdApplication();

    inline void ResetMitmdApplication() {
        SetMitmdApplication(ams::ncm::InvalidApplicationId);
    }

}