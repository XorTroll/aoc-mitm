
#pragma once
#include <aoc/mitm/impl/impl_MitmList.hpp>

namespace aoc::srv {

    class IAddOnContentManager : public ams::sf::IMitmServiceObject {

        private:
            enum class CommandId {
                CountAddOnContentByApplicationId = 0,
                ListAddOnContentByApplicationId = 1,
                CountAddOnContent = 2,
                ListAddOnContent = 3,
                GetAddOnContentBaseIdByApplicationId = 4,
                GetAddOnContentBaseId = 5,
                PrepareAddOnContentByApplicationId = 6,
                PrepareAddOnContent = 7,
            };

        public:
            // Note: this value is used assuming that no game will have >128 DLCs
            static constexpr s32 DlcContentCount = 128;

        private:
            inline void CountAddOnContentBase(ams::sf::Out<u32> out_count) {
                out_count.SetValue(DlcContentCount);
            }

            inline void ListAddOnContentBase(const ams::sf::OutBuffer &out_buf, ams::sf::Out<s32> out_count, s32 offset, s32 count) {
                const auto buf_count = static_cast<s32>(out_buf.GetSize() / sizeof(s32));
                const auto size = std::min({ buf_count, DlcContentCount, count });
                auto dlc_idx_buf = reinterpret_cast<s32*>(out_buf.GetPointer());
                for(auto i = offset; i < size; i++) {
                    // DLC ids consist on DLC base ID + an index (1, 2, 3...) so we will populate the array this way
                    dlc_idx_buf[i] = i + 1;
                }
                out_count.SetValue(size);
            }

            inline ams::ncm::ApplicationId GetApplicationIdFromProcessId(const ams::sf::ClientProcessId &client_pid) {
                auto app_id = ams::ncm::InvalidApplicationId;
                pminfoGetProgramId(&app_id.value, static_cast<u64>(client_pid.process_id));
                return app_id;
            }

            inline void GetAddOnContentBaseIdBase(ams::sf::Out<ams::ncm::ApplicationId> out_dlc_base_id, ams::ncm::ApplicationId app_id) {
                out_dlc_base_id.SetValue({ app_id.value + 0x1000 });
            }

        public:
            static bool ShouldMitm(const ams::sm::MitmProcessInfo &client_info) {
                const ams::ncm::ApplicationId app_id = { client_info.program_id.value };
                return mitm::impl::ShouldMitmApplication(app_id);
            }

            IAddOnContentManager(std::shared_ptr<::Service> &&s, const ams::sm::MitmProcessInfo &c) : IMitmServiceObject(std::forward<std::shared_ptr<::Service>>(s), c) {
                AOC_LOG_FMT("Initialized aoc:u with ID 0x%016lX", c.program_id.value);
                mitm::impl::SetMitmdApplication({ c.program_id.value });
            }

            ~IAddOnContentManager() {
                AOC_LOG_FMT("Finalized aoc:u");
                mitm::impl::ResetMitmdApplication();
            }

        public:
            void CountAddOnContentByApplicationId(ams::sf::Out<u32> out_count, ams::ncm::ApplicationId app_id) {
                AOC_LOG_FMT("CountAddOnContentByApplicationId -> ID: 0x%016lX", app_id.value);
                this->CountAddOnContentBase(out_count);
            }

            void ListAddOnContentByApplicationId(const ams::sf::OutBuffer &out_buf, ams::sf::Out<s32> out_count, s32 offset, s32 count, ams::ncm::ApplicationId app_id) {
                AOC_LOG_FMT("ListAddOnContentByApplicationId -> ID: 0x%016lX, Offset: %d, Count: %d", app_id.value, offset, count);
                this->ListAddOnContentBase(out_buf, out_count, offset, count);
            }

            void CountAddOnContent(const ams::sf::ClientProcessId &client_pid, ams::sf::Out<u32> out_count) {
                const auto app_id = this->GetApplicationIdFromProcessId(client_pid);
                AOC_LOG_FMT("CountAddOnContent -> ID: 0x%016lX", app_id.value);
                this->CountAddOnContentBase(out_count);
            }

            void ListAddOnContent(const ams::sf::ClientProcessId &client_pid, const ams::sf::OutBuffer &out_buf, ams::sf::Out<s32> out_count, s32 offset, s32 count) {
                const auto app_id = this->GetApplicationIdFromProcessId(client_pid);
                AOC_LOG_FMT("ListAddOnContent -> ID: 0x%016lX, Offset: %d, Count: %d", app_id.value, offset, count);
                this->ListAddOnContentBase(out_buf, out_count, offset, count);
            }

            void GetAddOnContentBaseIdByApplicationId(ams::sf::Out<ams::ncm::ApplicationId> out_dlc_base_id, ams::ncm::ApplicationId app_id) {
                AOC_LOG_FMT("GetAddOnContentBaseIdByApplicationId -> ID: 0x%016lX", app_id.value);
                this->GetAddOnContentBaseIdBase(out_dlc_base_id, app_id);
            }

            void GetAddOnContentBaseId(const ams::sf::ClientProcessId &client_pid, ams::sf::Out<ams::ncm::ApplicationId> out_dlc_base_id) {
                const auto app_id = this->GetApplicationIdFromProcessId(client_pid);
                AOC_LOG_FMT("GetAddOnContentBaseId -> ID: 0x%016lX", app_id.value);
                this->GetAddOnContentBaseIdBase(out_dlc_base_id, app_id);
            }

            void PrepareAddOnContentByApplicationId(ams::ncm::ApplicationId app_id, u32 dlc_idx) {
                AOC_LOG_FMT("PrepareAddOnContentByApplicationId -> stubbed, ID: 0x%016lX, DLC index: %d", app_id.value, dlc_idx);
            }
            
            void PrepareAddOnContent(const ams::sf::ClientProcessId &client_pid, u32 dlc_idx) {
                const auto app_id = this->GetApplicationIdFromProcessId(client_pid);
                AOC_LOG_FMT("PrepareAddOnContent -> stubbed, ID: 0x%016lX, DLC index: %d", app_id.value, dlc_idx);
            }

        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(CountAddOnContentByApplicationId),
                MAKE_SERVICE_COMMAND_META(ListAddOnContentByApplicationId),
                MAKE_SERVICE_COMMAND_META(CountAddOnContent),
                MAKE_SERVICE_COMMAND_META(ListAddOnContent),
                MAKE_SERVICE_COMMAND_META(GetAddOnContentBaseIdByApplicationId),
                MAKE_SERVICE_COMMAND_META(GetAddOnContentBaseId),
                MAKE_SERVICE_COMMAND_META(PrepareAddOnContentByApplicationId),
                MAKE_SERVICE_COMMAND_META(PrepareAddOnContent),
            };

    };

    constexpr ams::sm::ServiceName ServiceName = ams::sm::ServiceName::Encode("aoc:u");

}