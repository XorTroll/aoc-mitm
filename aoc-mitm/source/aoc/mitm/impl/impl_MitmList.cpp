#include <aoc/mitm/impl/impl_MitmList.hpp>
#include <vector>
#include <algorithm>

namespace aoc::mitm::impl {

    static std::vector<ams::ncm::ApplicationId> g_mitm_app_ids;
    static ams::ncm::ApplicationId g_mitmd_app_id = ams::ncm::InvalidApplicationId;
    static ams::os::Mutex g_mitm_lock(true);

    bool IsApplicationInMitmList(ams::ncm::ApplicationId app_id) {
        std::scoped_lock lk(g_mitm_lock);
        return std::find(g_mitm_app_ids.begin(), g_mitm_app_ids.end(), app_id) != g_mitm_app_ids.end();
    }

    void AddApplicationToMitmList(ams::ncm::ApplicationId app_id) {
        std::scoped_lock lk(g_mitm_lock);
        if(!IsApplicationInMitmList(app_id)) {
            g_mitm_app_ids.push_back(app_id);
        }
    }

    void RemoveApplicationFromMitmList(ams::ncm::ApplicationId app_id) {
        std::scoped_lock lk(g_mitm_lock);
        if(IsApplicationInMitmList(app_id)) {
            g_mitm_app_ids.erase(std::remove(g_mitm_app_ids.begin(), g_mitm_app_ids.end(), app_id), g_mitm_app_ids.end());
        }
    }

    void SetMitmdApplication(ams::ncm::ApplicationId app_id) {
        std::scoped_lock lk(g_mitm_lock);
        g_mitmd_app_id = app_id;
    }

    ams::ncm::ApplicationId GetMitmdApplication() {
        std::scoped_lock lk(g_mitm_lock);
        return g_mitmd_app_id;
    }

}
