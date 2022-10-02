#![no_std]
#![no_main]

#[macro_use]
extern crate nx;

#[macro_use]
extern crate alloc;

extern crate paste;

use nx::result::*;
use nx::util;
use nx::thread;
use nx::diag::abort;
use nx::diag::log;
use nx::ipc::server;
use nx::fs;
use core::panic;

mod aoc;
mod mitm;

const STACK_HEAP_SIZE: usize = 0x10000;
static mut STACK_HEAP: [u8; STACK_HEAP_SIZE] = [0; STACK_HEAP_SIZE];

#[no_mangle]
pub fn initialize_heap(_hbl_heap: util::PointerAndSize) -> util::PointerAndSize {
    unsafe {
        util::PointerAndSize::new(STACK_HEAP.as_mut_ptr(), STACK_HEAP_SIZE)
    }
}

const POINTER_BUF_SIZE: usize = 0;
type Manager = server::ServerManager<POINTER_BUF_SIZE>;

#[no_mangle]
pub fn main() -> Result<()> {
    thread::get_current_thread().name.set_str("aoc-mitm.Main");
    fs::initialize_fspsrv_session()?;
    fs::mount_sd_card("sdmc")?;
    mitm::scan_mitms()?;

    let mut manager = Manager::new()?;
    manager.register_mitm_service_server::<aoc::AddOnContentManager>()?;
    manager.loop_process()?;

    fs::finalize_fspsrv_session();
    Ok(())
}

#[panic_handler]
fn panic_handler(info: &panic::PanicInfo) -> ! {
    util::simple_panic_handler::<log::lm::LmLogger>(info, abort::AbortLevel::SvcBreak())
}