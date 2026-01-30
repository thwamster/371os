use malloc::*;

fn main() {
	let p0 : usize;
	let p1 : usize;
	let x : i32;
	let y : i32;
	let z : i32;
	let w : i32;

	dbg![p0 = malloc(16).unwrap()];
	dbg![p1 = malloc(32).unwrap()];
	dbg![x = 0x44332211];
	dbg![y = 0x12345678];
	dbg![&p0, &p1];
	dbg![&x, &y];
	dbg![set(x, p0)];
	dbg![set(y, p1)];
	dbg![z = get(p0)];
	dbg![w = get(p1)];
	dbg![&z, &w];
	dbg![assert_eq![x, z]];
	dbg![assert_eq![y, w]];
	dbg![assert![malloc(2048).is_none()]];
	dbg![malloc(32).unwrap()];
	dbg![assert![malloc(64).is_none()]];
}
