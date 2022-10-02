use nx::result::*;
use nx::ipc::sf;
use nx::ipc::server;
use nx::service;
use nx::service::pm;
use nx::service::pm::IInformationInterface;
use nx::service::sm;
use nx::version;

use crate::mitm;

ipc_sf_define_interface_trait! {
    trait IAddOnContentManager {
        count_add_on_content_by_application_id [0, version::VersionInterval::all()]: (app_id: u64) => (count: u32);
        list_add_on_content_by_application_id [1, version::VersionInterval::all()]: (offset: u32, count: u32, app_id: u64, out_list: sf::OutMapAliasBuffer<u32>) => (count: u32);
        count_add_on_content [2, version::VersionInterval::all()]: (process_id: sf::ProcessId) => (count: u32);
        list_add_on_content [3, version::VersionInterval::all()]: (process_id: sf::ProcessId, offset: u32, count: u32, out_list: sf::OutMapAliasBuffer<u32>) => (count: u32);
        get_add_on_content_base_id_by_application_id [4, version::VersionInterval::all()]: (app_id: u64) => (aoc_base_id: u64);
        get_add_on_content_base_id [5, version::VersionInterval::all()]: (process_id: sf::ProcessId) => (aoc_base_id: u64);
        prepare_add_on_content_by_application_id [6, version::VersionInterval::all()]: (app_id: u64, dlc_index: u32) => ();
        prepare_add_on_content [7, version::VersionInterval::all()]: (process_id: sf::ProcessId, dlc_index: u32) => ();
    }
}

pub struct AddOnContentManager {
    dummy_session: sf::Session
}

impl AddOnContentManager {
    fn list_add_on_content_common(&self, out_list: sf::OutMapAliasBuffer<u32>, offset: u32, _count: u32) -> u32 {
        let out_array = out_list.get_mut_slice();
        let actual_count = core::cmp::min(out_array.len() as u32, MAX_AOC_CONTENT_COUNT);
        for i in offset..actual_count {
            out_array[i as usize] = i + 1;
        }
        actual_count
    }

    fn get_program_id(&self, process_id: sf::ProcessId) -> Result<u64> {
        let pminfo = service::new_service_object::<pm::InformationInterface>()?;
        pminfo.get().get_program_id(process_id.process_id)
    }

    fn get_add_on_content_base_id_common(&self, app_id: u64) -> u64 {
        app_id + 0x1000
    }
}

impl sf::IObject for AddOnContentManager {
    ipc_sf_object_impl_default_command_metadata!();

    fn get_session(&mut self) -> &mut sf::Session {
        &mut self.dummy_session
    }
}

// TODO: Is this high enough?
const MAX_AOC_CONTENT_COUNT: u32 = 128;

impl IAddOnContentManager for AddOnContentManager {
    fn count_add_on_content_by_application_id(&mut self, _app_id: u64) -> Result<u32> {
        Ok(MAX_AOC_CONTENT_COUNT)
    }

    fn list_add_on_content_by_application_id(&mut self, offset: u32, count: u32, _app_id: u64, out_list: sf::OutMapAliasBuffer<u32>) -> Result<u32> {
        Ok(self.list_add_on_content_common(out_list, offset, count))
    }

    fn count_add_on_content(&mut self, _process_id: sf::ProcessId) -> Result<u32> {
        Ok(MAX_AOC_CONTENT_COUNT)
    }

    fn list_add_on_content(&mut self, _process_id: sf::ProcessId, offset: u32, count: u32, out_list: sf::OutMapAliasBuffer<u32>) -> Result<u32> {
        Ok(self.list_add_on_content_common(out_list, offset, count))
    }

    fn get_add_on_content_base_id_by_application_id(&mut self, app_id: u64) -> Result<u64> {
        Ok(self.get_add_on_content_base_id_common(app_id))
    }

    fn get_add_on_content_base_id(&mut self, process_id: sf::ProcessId) -> Result<u64> {
        let app_id = self.get_program_id(process_id)?;
        Ok(self.get_add_on_content_base_id_common(app_id))
    }

    fn prepare_add_on_content_by_application_id(&mut self, _app_id: u64, _dlc_index: u32) -> Result<()> {
        Ok(())
    }

    fn prepare_add_on_content(&mut self, _process_id: sf::ProcessId, _dlc_index: u32) -> Result<()> {
        Ok(())
    }
}

impl server::ISessionObject for AddOnContentManager {}

impl server::IMitmServerObject for AddOnContentManager {
    fn new(_info: sm::mitm::MitmProcessInfo) -> Self {
        Self { dummy_session: sf::Session::new() }
    }
}

impl server::IMitmService for AddOnContentManager {
    fn get_name() -> sm::ServiceName {
        sm::ServiceName::new("aoc:u")
    }

    fn should_mitm(info: sm::mitm::MitmProcessInfo) -> bool {
        mitm::has_mitm(info.program_id)
    }
}