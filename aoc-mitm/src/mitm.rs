use nx::result::*;
use nx::sync;
use nx::fs;
use alloc::vec::Vec;
use alloc::string::String;

static mut MITM_LIST: sync::Locked<Vec<u64>> = sync::Locked::new(false, Vec::new());
static MITM_LIST_FILE: &'static str = "sdmc:/aoc-mitm/titles.list";

pub fn scan_mitms() -> Result<()> {
    if let Ok(mut title_list) = fs::open_file(String::from(MITM_LIST_FILE), fs::FileOpenOption::Read()) {
        let data_len = title_list.get_size()?;
        let mut data: Vec<u8> = vec![0; data_len];
        title_list.read(data.as_mut_ptr(), data_len)?;
        if let Ok(data_str) = String::from_utf8(data) {

            for line in data_str.split('\n') {
                if let Ok(parsed_app_id) = u64::from_str_radix(line.trim_start_matches("0x"), 16) {
                    unsafe {
                        MITM_LIST.get().push(parsed_app_id);
                    }
                }
            }
        }
    }

    Ok(())
}

pub fn has_mitm(app_id: u64) -> bool {
    unsafe {
        MITM_LIST.get().contains(&app_id)
    }
}