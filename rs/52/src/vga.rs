static mut INDEX : (usize, usize) = (0, 0);

const MMIO : *mut [[u16; WIDTH]; HEIGHT] = 0xB8000 as *mut [[u16; WIDTH]; HEIGHT];
const HEIGHT : usize = 25;
const WIDTH : usize = 80;
const FOREGROUND : u16 = 0b1 << 8;
const BACKGROUND : u16 = 0b1 << 12;
const BRIGHT : u16 = 0b1 << 11;
const BLINK : u16 = 0b1 << 15;

#[repr(u16)]
enum Color {
	BLACK = 0x0,
	BLUE = 0x1,
	GREEN = 0x2,
	CYAN = 0x3,
	RED = 0x4,
	MAGENTA = 0x5,
	BROWN = 0x6,
	GRAY = 0x7,
	DARK_GRAY = 0x8,
	LIGHT_BLUE = 0x9,
	LIGHT_GREEN = 0xA,
	LIGHT_CYAN = 0xB,
	LIGHT_RED = 0xC,
	PINK = 0xD,
	YELLOW = 0xE,
	WHITE = 0xF
}

macro_rules! print {
    () => { ... };
    ($($arg:tt)*) => { ... };
}

pub fn print_str(string : &str) {
	let characters : &[u8] = string.as_bytes();

	for i in 0..characters.len() {
		print_char(characters[i] as u16, Color::WHITE, Color::BLACK, false);
	}
}

fn print_char(character : u16, foreground : Color, background : Color, blink : bool) {
	unsafe {
		(*MMIO)[INDEX.0][INDEX.1] = character
			+ FOREGROUND * foreground as u16
			+ BACKGROUND * background as u16
			+ BLINK * blink as u16;
		INDEX.1 += 1;

		if INDEX.1 > WIDTH {
			INDEX.1 = 0;
			INDEX.0 += 1;
		}
	}
}
