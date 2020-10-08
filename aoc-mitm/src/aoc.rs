use nx::result::*;
use nx::mem;
use nx::diag::log;
use nx::ipc::sf;
use nx::ipc::server;
use nx::ipc::sf::lm;
use nx::ipc::sf::lm::ILogger;
use nx::ipc::sf::lm::ILogService;
use nx::service;
use nx::service::pm;
use nx::service::pm::IInformationInterface;

use crate::mitm;

pub trait IAddOnContentManager {
    ipc_interface_define_command!(count_add_on_content_by_application_id: (app_id: u64) => (count: u32));
    ipc_interface_define_command!(list_add_on_content_by_application_id: (offset: u32, count: u32, app_id: u64, out_list: sf::OutMapAliasBuffer) => (count: u32));
    ipc_interface_define_command!(count_add_on_content: (process_id: sf::ProcessId) => (count: u32));
    ipc_interface_define_command!(list_add_on_content: (process_id: sf::ProcessId, offset: u32, count: u32, out_list: sf::OutMapAliasBuffer) => (count: u32));
    ipc_interface_define_command!(get_add_on_content_base_id_by_application_id: (app_id: u64) => (aoc_base_id: u64));
    ipc_interface_define_command!(get_add_on_content_base_id: (process_id: sf::ProcessId) => (aoc_base_id: u64));
    ipc_interface_define_command!(prepare_add_on_content_by_application_id: (app_id: u64, dlc_index: u32) => ());
    ipc_interface_define_command!(prepare_add_on_content: (process_id: sf::ProcessId, dlc_index: u32) => ());
}

pub struct AddOnContentManager {
    session: sf::Session
}

impl AddOnContentManager {
    fn list_add_on_content_common(&self, out_list: sf::OutMapAliasBuffer, offset: u32, _count: u32) -> u32 {
        let out_array: &mut [u32] = out_list.get_mut_slice();
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
    fn get_session(&mut self) -> &mut sf::Session {
        &mut self.session
    }

    fn get_command_table(&self) -> sf::CommandMetadataTable {
        vec! [
            ipc_interface_make_command_meta!(count_add_on_content_by_application_id: 0),
            ipc_interface_make_command_meta!(list_add_on_content_by_application_id: 1),
            ipc_interface_make_command_meta!(count_add_on_content: 2),
            ipc_interface_make_command_meta!(list_add_on_content: 3),
            ipc_interface_make_command_meta!(get_add_on_content_base_id_by_application_id: 4),
            ipc_interface_make_command_meta!(get_add_on_content_base_id: 5),
            ipc_interface_make_command_meta!(prepare_add_on_content_by_application_id: 6),
            ipc_interface_make_command_meta!(prepare_add_on_content: 7)
        ]
    }
}

// TODO: Is this high enough?
const MAX_AOC_CONTENT_COUNT: u32 = 128;

impl IAddOnContentManager for AddOnContentManager {
    fn count_add_on_content_by_application_id(&mut self, app_id: u64) -> Result<u32> {
        Ok(MAX_AOC_CONTENT_COUNT)
    }

    fn list_add_on_content_by_application_id(&mut self, offset: u32, count: u32, app_id: u64, out_list: sf::OutMapAliasBuffer) -> Result<u32> {
        Ok(self.list_add_on_content_common(out_list, offset, count))
    }

    fn count_add_on_content(&mut self, process_id: sf::ProcessId) -> Result<u32> {
        Ok(MAX_AOC_CONTENT_COUNT)
    }

    fn list_add_on_content(&mut self, process_id: sf::ProcessId, offset: u32, count: u32, out_list: sf::OutMapAliasBuffer) -> Result<u32> {
        Ok(self.list_add_on_content_common(out_list, offset, count))
    }

    fn get_add_on_content_base_id_by_application_id(&mut self, app_id: u64) -> Result<u64> {
        Ok(self.get_add_on_content_base_id_common(app_id))
    }

    fn get_add_on_content_base_id(&mut self, process_id: sf::ProcessId) -> Result<u64> {
        let app_id = self.get_program_id(process_id)?;
        Ok(self.get_add_on_content_base_id_common(app_id))
    }

    fn prepare_add_on_content_by_application_id(&mut self, app_id: u64, dlc_index: u32) -> Result<()> {
        Ok(())
    }

    fn prepare_add_on_content(&mut self, process_id: sf::ProcessId, dlc_index: u32) -> Result<()> {
        Ok(())
    }
}

impl server::IMitmServerObject for AddOnContentManager {
    fn new(info: service::sm::MitmProcessInfo) -> Self {
        Self { session: sf::Session::new() }
    }
}

impl server::IMitmService for AddOnContentManager {
    fn get_name() -> &'static str {
        nul!("aoc:u")
    }

    fn should_mitm(info: service::sm::MitmProcessInfo) -> bool {
        mitm::has_mitm(info.program_id)
    }
}