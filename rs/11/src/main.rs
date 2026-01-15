union MyUnion {
    f: f32,
    u: u32,
}

fn main() {
    // Unions
    let mut u = MyUnion { f: 0.0 };

    unsafe {
        println!("Bits as float: {}", u.f);
        println!("Bits as integer: {:#x}", u.u);
    }

    u.u = 0x3F800000; // This is the bit pattern for 1.0 in IEEE 754

    unsafe {
        println!("After manual bit update, float is: {}", u.f);
    }

    // Dangling
    let ptr: *const i32;

    {
        let x = 1234;
        ptr = &x as *const i32;
        unsafe {
            println!("Value at ptr: {}", *ptr);
        }
    }

    unsafe {
        println!("Value at ptr: {}", *ptr);
    }

    // Deference

    let mut num = 5;

    let r1 = &raw const num;
    let r2 = &raw mut num;

    unsafe {
        println!("r1 is: {}", *r1);
        println!("r2 is: {}", *r2);
    }
}
