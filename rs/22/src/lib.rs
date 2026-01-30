const SIZE : usize = 0x80;
static mut BUS : [u8; SIZE] = [0u8; SIZE];

fn init() {
	assert_eq![SIZE & (SIZE - 1), 0];

	for i in 0..(SIZE >> 6) {
		unsafe {
			BUS[i] = 0xFF;
		}
	}

	return;
}

pub fn malloc(s : usize) -> Option<usize> {
	assert![s > 0];

	unsafe {
		if BUS[0] == 0 {
			init();
		}
	}

	let mut c : usize = 0;
	let mut m : usize = 0;

	for i in 0..(SIZE) {
		if c >= s {
			break;
		}

		unsafe {
			if BUS[i / 8] & (1 << i % 8) == 0 {
				c += 1;
				continue;
			}
		}

		c = 0;
		m = i + 1;
	}

	if c >= s {
		for i in m..(m + c) {
			unsafe {
				BUS[i / 8] |= 1 << i % 8;
			}
		}

		return Some(m);
	}

	None
}

pub fn set(n : i32, s : usize) {
	let b : [u8; 4] = n.to_ne_bytes();

	for i in 0..4 {
		unsafe {
			BUS[i + s] = b[i];
		}
	}
}

pub fn get(s : usize) -> i32 { unsafe { i32::from_ne_bytes(BUS[s..s + 4].try_into().unwrap()) } }
