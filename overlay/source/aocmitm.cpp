#include <aocmitm.hpp>

#define AOC_MITM_SRV "aocmitm"

namespace aocmitm {

    static Service g_srv;

    bool IsAvailable() {
        bool has = false;
        const auto srv_name = smServiceNameToU64(smEncodeName(AOC_MITM_SRV));
        // smHasService
        serviceDispatchInOut(smGetServiceSession(), 65100, srv_name, has);
        return has;
    }

    Result Initialize() {
        if(serviceIsActive(&g_srv)) {
            return 0;
        }
        return smGetService(&g_srv, AOC_MITM_SRV);
    }

    void Exit() {
        serviceClose(&g_srv);
    }

    bool IsApplicationInMitmList(u64 app_id) {
        bool out = false;
        serviceDispatchInOut(&g_srv, 0, app_id, out);
        return out;
    }

    void AddApplicationToMitmList(u64 app_id) {
        serviceDispatchIn(&g_srv, 1, app_id);
    }

    void RemoveApplicationFromMitmList(u64 app_id) {
        serviceDispatchIn(&g_srv, 2, app_id);
    }

}