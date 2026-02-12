#![no_std]
#![no_main]
#![allow(unconditional_recursion)]
#![cfg(not(test))]

const HEIGHT : usize = 25;
const WIDTH : usize = 80;
const BACKGROUND : u16 = 0x10;
const BRIGHT : u16 = 0x800;
const BLINK : u16 = 0x8000;

#[repr(u16)]
enum Color {
	BLACK = 0x000,
	BLUE = 0x100,
	GREEN = 0x200,
	CYAN = 0x300,
	RED = 0x400,
	MAGENTA = 0x500,
	BROWN = 0x600,
	GRAY = 0x700
}

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {
	hello();

	loop {}
}

fn hello() {
	let buffer : *mut [[u16; WIDTH]; HEIGHT] = 0xB8000 as *mut [[u16; WIDTH]; HEIGHT];
	let s : &[u8; 12] = b"Hello World!";
	let c : [u16; 12] = [
		Color::RED as u16 + BRIGHT,
		Color::RED as u16,
		Color::BROWN as u16 + BRIGHT,
		Color::GREEN as u16 + BRIGHT,
		Color::GREEN as u16,
		Color::GRAY as u16 + BRIGHT,
		Color::CYAN as u16 + BRIGHT,
		Color::CYAN as u16,
		Color::BLUE as u16 + BRIGHT,
		Color::BLUE as u16,
		Color::MAGENTA as u16,
		Color::MAGENTA as u16 + BRIGHT
	];

	for i in 0..s.len() {
		unsafe {
			(*buffer)[0][i] = s[i] as u16 + c[i] //  + (BACKGROUND * Color::BLACK as u16);
		}
	}
}

#[panic_handler]
fn panic(info : &core::panic::PanicInfo) -> ! { panic(info) }
