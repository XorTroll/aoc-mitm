
#define TESLA_INIT_IMPL
#include <tesla.hpp>
#include <aocmitm.hpp>
#include <vector>

struct ApplicationData {
    u64 app_id;
    char name[0x200];
};

namespace {

    bool g_aocmitm_initialized = false;
    std::vector<ApplicationData> g_applications;

    void LoadApplications() {
        NsApplicationRecord rec = {};
        s32 offset = 0;
        s32 tmp_count = 0;
        while(true) {
            auto rc = nsListApplicationRecord(&rec, 1, offset, &tmp_count);
            if(R_FAILED(rc) || (tmp_count < 1)) {
                break;
            }
            ApplicationData app = {};
            app.app_id = rec.application_id;
            NsApplicationControlData nacp = {};
            nsGetApplicationControlData(NsApplicationControlSource_Storage, app.app_id, &nacp, sizeof(nacp), nullptr);
            NacpLanguageEntry *lang = nullptr;
            nacpGetLanguageEntry(&nacp.nacp, &lang);
            if(lang != nullptr) {
                strcpy(app.name, lang->name);
                g_applications.push_back(app);
            }
            offset++;
            rec = {};
        }
    }

}

class AocMitmGui : public tsl::Gui {

    public:
        AocMitmGui() {}

        virtual tsl::elm::Element *createUI() override {
            auto frame = new tsl::elm::OverlayFrame("aoc-mitm manager", "dev");
            auto list = new tsl::elm::List();
            u32 idx = 0;
            for(auto &app: g_applications) {
                auto item = new tsl::elm::ListItem(app.name);
                item->setClickListener([item, idx](u64 keys) {
                    auto &app = g_applications[idx];
                    if(keys & KEY_A) {
                        if(aocmitm::IsApplicationInMitmList(app.app_id)) {
                            aocmitm::RemoveApplicationFromMitmList(app.app_id);
                            item->setText(app.name);
                        }
                        else {
                            aocmitm::AddApplicationToMitmList(app.app_id);
                            item->setText(std::string(" * ") + app.name);
                        }
                        return true;
                    }
                    return false;
                });
                list->addItem(item);
                idx++;
            }
            frame->setContent(list);
            return frame;
        }

};

class AocMitmOverlay : public tsl::Overlay {

    public:
        virtual void initServices() override {
            tsl::hlp::doWithSmSession([&]() {
                if(R_SUCCEEDED(nsInitialize())) {
                    if(aocmitm::IsAvailable()) {
                        if(R_SUCCEEDED(aocmitm::Initialize())) {
                            g_aocmitm_initialized = true;
                        }
                    }
                }
            });
            if(g_aocmitm_initialized) {
                LoadApplications();
            }
        }

        virtual void exitServices() override {
            aocmitm::Exit();
            nsExit();
        }

        virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
            return initially<AocMitmGui>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
        }

};

int main(int argc, char **argv) {
    return tsl::loop<AocMitmOverlay>(argc, argv);
}