#[macro_use]
mod smack;
use smack::*;

// @flag --no-memory-splitting
// @expect error

fn call_with_one<F>(mut some_closure: F) -> ()
  where F : FnMut(i32) -> () {

  some_closure(1);
}

fn main() {
  let mut num = 5;
  {
    let mut add_num = |x: i32| num += x;

    add_num(5);
    call_with_one(&mut add_num);
  }
  assert_eq!(10, num); // Should be 11
}