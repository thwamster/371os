fn main() {    
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
}
