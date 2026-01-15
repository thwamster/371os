use split_at::*;

fn main() {
    let mut n: [i32; 10] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    println!("{:?}", &n);

    let (a, b) = split_at_mut(&mut n, 5_usize);
    println!("{:?}\n{:?}", &a, &b);
}
