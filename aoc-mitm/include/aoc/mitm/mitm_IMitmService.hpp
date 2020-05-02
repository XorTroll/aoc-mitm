
#pragma once
#include <aoc/mitm/impl/impl_MitmList.hpp>

namespace aoc::mitm {

    class IMitmService : public ams::sf::IServiceObject {

        private:
            enum class CommandId {
                IsApplicationInMitmList = 0,
                AddApplicationToMitmList = 1,
                RemoveApplicationFromMitmList = 2,
                IsCurrentApplicationMitmd = 3,
            };

        public:
            void IsApplicationInMitmList(ams::ncm::ApplicationId app_id, ams::sf::Out<bool> out_is) {
                out_is.SetValue(impl::IsApplicationInMitmList(app_id));
            }

            void AddApplicationToMitmList(ams::ncm::ApplicationId app_id) {
                impl::AddApplicationToMitmList(app_id);
            }

            void RemoveApplicationFromMitmList(ams::ncm::ApplicationId app_id) {
                impl::RemoveApplicationFromMitmList(app_id);
            }

            void IsCurrentApplicationMitmd(ams::sf::Out<bool> out_is) {
                bool is_mitmd = false;
                u64 current_app_pid = 0;
                auto rc = pmdmntGetApplicationProcessId(&current_app_pid);
                if(R_SUCCEEDED(rc)) {
                    auto app_id = ams::ncm::InvalidApplicationId;
                    rc = pminfoGetProgramId(&app_id.value, current_app_pid);
                    if(R_SUCCEEDED(rc)) {
                        is_mitmd = impl::GetMitmdApplication() == app_id;
                    }
                }
                out_is.SetValue(is_mitmd);
            }

        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(IsApplicationInMitmList),
                MAKE_SERVICE_COMMAND_META(AddApplicationToMitmList),
                MAKE_SERVICE_COMMAND_META(RemoveApplicationFromMitmList),
                MAKE_SERVICE_COMMAND_META(IsCurrentApplicationMitmd),
            };

    };

    constexpr ams::sm::ServiceName ServiceName = ams::sm::ServiceName::Encode("aocmitm");

}