use crate::image::IMAGE;

static mut INDEX_X : usize = 0;
static mut INDEX_Y : usize = 0;

const MMIO : *mut [[u16; WIDTH]; HEIGHT] = 0xB8000 as *mut [[u16; WIDTH]; HEIGHT];
const HEIGHT : usize = 25;
const WIDTH : usize = 80;
const FOREGROUND : u16 = 0b1 << 8;
const BACKGROUND : u16 = 0b1 << 12;
const BRIGHT : u16 = 0b1 << 11;
const BLINK : u16 = 0b1 << 15;

#[repr(u16)]
enum Color {
	Black = 0x0,
	Blue = 0x1,
	Green = 0x2,
	Cyan = 0x3,
	Red = 0x4,
	Magenta = 0x5,
	Brown = 0x6,
	Gray = 0x7,
	DarkGray = 0x8,
	LightBlue = 0x9,
	LightGreen = 0xA,
	LightCyan = 0xB,
	LightRed = 0xC,
	Pink = 0xD,
	Yellow = 0xE,
	White = 0xF
}

pub fn print_str(string : &str) {
	let characters : &[u8] = string.as_bytes();

	for i in 0..characters.len() {
		print_char(characters[i] as u16, Color::White, Color::Black, false);
	}
}

fn print_char(character : u16, foreground : Color, background : Color, blink : bool) {
	unsafe {
		(*MMIO)[INDEX_X][INDEX_Y] = character
			+ FOREGROUND * foreground as u16
			+ BACKGROUND * background as u16
			+ BLINK * blink as u16;

		INDEX_X += 1;

		if INDEX_X > WIDTH {
			INDEX_X = 0;
			INDEX_Y += 1;
		}
	}
}

pub fn map_colors() {
	for x in 0..HEIGHT {
		for y in 0..WIDTH {
			map_pixel(y, x, (x / 5) as u16);
		}
	}
}

pub fn map_image() {
	for x in 0..HEIGHT {
		for y in 0..WIDTH {
			map_pixel(y, x, IMAGE[x * WIDTH + y]);
		}
	}
}

fn map_pixel(y : usize, x : usize, color : u16) {
	unsafe {
		(*MMIO)[x][y] = color * BACKGROUND;
	}
}
