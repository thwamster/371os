#![no_std]
#![no_main]
#![allow(unconditional_recursion)]
#![cfg(not(test))]

mod vga;

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {
	vga::print_str("Hello, World!");
	loop {}
}

#[panic_handler]
fn panic(info : &core::panic::PanicInfo) -> ! { panic(info) }
