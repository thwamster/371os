#![no_std]
#![no_main]
#![allow(unconditional_recursion)]
#![cfg(not(test))]

mod img;
mod vga;

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {
	vga::map_image();
	loop {}
}

#[panic_handler]
fn panic(info : &core::panic::PanicInfo) -> ! { panic(info) }
