<!---
  Name          : Chapter_2.md
  Project       : Computer Systems: A Programmers Perspective 3e
  Description   : Solutions to chapter 2 homework problems
  Creation Date : 27 July 2020
  Author        : amenasec
  Link          : https://github.com/amenasec
--->


# Chapter 2 Homework Problems

### 2.55 ⬥
Compile and run the sample code that uses `show_bytes` (file `show_bytes.c`) on different machines to which you have access. Determine the byte orderings used by these machines.

---

Ubuntu 64 bit:
````shell
$ uname -mrs
Linux 5.4.0-42-generic x86_64

$ ./show-bytes 12345
calling test_show_bytes
 39 30 00 00
 00 e4 40 46
 e8 24 4d 8d fe 7f 00 00
````

macOS 64 bit:
````shell
$ uname -mrs
Darwin 19.6.0 x86_64

$ ./show-bytes 12345
calling test_show_bytes
 39 30 00 00
 00 e4 40 46
 d8 d9 c3 e5 fe 7f 00 00

````

The `show_bytes` program was run on a 64 bit Ubuntu (Linux) and a 64 bit macOS machine. As can be observed in the output from the program calls, both machines are little-endian, since the bytes representing the integer argument `12345`, `0x3039` in hex, is stored as `39 30 00 00` by the machines.

---


### 2.56 ⬥
Try running the code for `show_bytes` for different sample values.

---

````shell
$ ./show-bytes 1
calling test_show_bytes
 01 00 00 00
 00 00 80 3f
 d8 f9 47 e0 fe 7f 00 00

$ ./show-bytes 0x12345678
calling test_show_bytes
 78 56 34 12
 b4 a2 91 4d
 d8 19 77 ef fe 7f 00 00

$ ./show-bytes 0x78563412
calling test_show_bytes
 12 34 56 78
 68 ac f0 4e
 d8 29 c2 e0 fe 7f 00 00
 ````

These different sample values were run on the little-endian 64 bit macOS machine.

---


### 2.57 ⬥
Write procedures `show_short`, `show_long`, and `show_double` that print the byte representations of C objects of types `short`, `long`, and `double`, respectively. Try these out on several machines.

---

````C
void show_short(short x) {
  show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long(long x) {
  show_bytes((byte_pointer) &x, sizeof(long));
}

void show_double(double x) {
  show_bytes((byte_pointer) &x, sizeof(double));
}
````

---


### 2.58 ⬥⬥
Write a procedure `is_little_endian` that will return `1` when compiled and run on a little-endian machine, and will return `0` when compiled and run on a big-endian machine. This program should run on any machine of its word size.

---

````C
int is_little_endian() {
  unsigned int value = 1;
  char * pointer = (char*) &value;
  return (int) * pointer;
}
````


The procedure `is_little_endian` works as follows. Let us first consider the behavior of this procedure on a little-endian machine. Note that, per the C standard, the data type `int` is only guaranteed 2 bytes, although it is typical to be represented with 4 bytes. Assuming that only 2 bytes are guaranteed, although the behavior will be similar with 4 bytes, the `unsigned int value` variable will be represented as `01 00` in memory. Next, the `char *pointer` pointer will be set to the first memory address of the `unsigned int value` variable, which holds the value `01`. It is clear that the procedure will then return `1` as required.

Now let us consider the behavior of this procedure on a big-endian machine. Again, assuming that only the guaranteed 2 bytes representation is used, `unsigned int value` variable will be represented as `00 01` in memory. As before, the `char *pointer` pointer will be set to the first memory address of the `unsigned int value` variable, which now holds the value `00`. Hence, the procedure will return `0` as required.

---


### 2.59 ⬥⬥
Write a C expression that will yield a word consisting of the least significant byte of `x` and the remaining bytes of `y`. For operands `x=0x89ABCDEF` and `y=0x76543210`, this would give `0x765432EF`.

---

```` C
(x & 0xFF) | (y & ~0xFF)
````

Let us assume the word size is 4 bytes, although this expression will work with any word size. Now let us consider the behavior of this expression.

The expression `(x & 0xFF)` calculates the bitwise AND on the two operands `x` and `0xFF`. This has the result of only keeping the least significant byte of `x`, while the remaining bytes are all set to `0x00`. For example, if `x=0x89ABCDEF`, the expression `(x & 0xFF)` is equivalent to the expression `(0x89ABCDEF & 0x000000FF)`, which gives `0x000000EF`.

The expression `(y & ~0xFF)` calculates the bitwise AND on the two operands `y` and `~0xFF`. The expression `~0xFF` gives the bitwise NOT of `0xFF` which sets all bit to `1` except the least significant byte. Under our assumption of a 4 byte word size, this would yield `0xFFFFFF00`. Then it is clear that the result of the bitwise AND operation `(y & ~0xFF)` keeps all bytes of `y` except the least significant byte, which is set to `0x00`. For example, if `y=0x76543210`, the expression `(y & ~0xFF)` is equivalent to the expression `(0x76543210 & 0xFFFFFF00)`, which gives `0x76543200`.

Therfore, the full bitwise OR operation `(x & 0xFF) | (y & ~0xFF)` will yield the word consisting of the least significant byte of `x` and the remaining bytes of `y`. Continuing with the example values, we will have `0x000000EF | 0x76543200`, which gives `0x765432EF`.

---


### 2.60 ⬥⬥
Suppose we number the bytes in a ω-bit word from `0` (least significant) to `ω/8-1` (most significant). Write code for the following C function, which will return an unsigned value in which byte `i` of argument `x` has been replaced by byte `b`:
````C
unsigned replace_byte (unsigned x, int i, unsigned char b);
````
Here are some examples showing how the function should word:
````C
replace_byte(0x12345678, 2, 0xAB) --> 0x12AB3456;
replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB;
````

---

````C
unsigned replace_byte(unsigned x, int i, unsigned char b) {
  // If index is negative, return x
  if(i < 0) {
    return x;
  }

  // If index is greater than the size of unsigned, return x
  if(i > sizeof(unsigned)) {
    return x;
  }

  unsigned mask = ((unsigned) 0xFF) << (i << 3);
  unsigned i_byte = ((unsigned) b) << (i << 3);
  return (x & ~mask) | i_byte;
}
````

The procedure `replace_byte` above works as follows. First, we check that argument `i` is within the range of the size of an `unsigned` variable. If `i` is not a valid index, we return `x` unchanged. Otherwise, we create a mask for byte `i`. Note that `(i << 3)` yields the bit index from the byte index `i`, since a left shift of `3` is equivalent to multiplying by `8` and there are `8` bits in a byte. Hence, the expression `((unsigned) 0xFF) << (i << 3)` positions the `0xFF` value to the `i`th byte position. For example, if `i = 2`, then the expression `((unsigned) 0xFF) << (i << 3)` yields `0x00FF0000`. After creating the mask, we position the value of character `b` to the `i`th byte in a similar manner. For example, if `b=0xAB`, then the expression `((unsigned) b) << (i << 3)` yields `0x00AB0000`. Lastly we return the expression `(x & ~mask) | i_byte`. In this expression, we first perform the bitwise AND between `x` and `~mask` which effectively zeros out the `i`th byte of `x`. Then we perform the bitwise OR between `x & ~mask` and `i_byte`, which returns an unsigned value in which byte `i` of argument `x` is replaced by byte `b`, as required. For example if `x = 0x12345678`, `i=2`, and `b=0xAB`, then it is clear to see that `(x & ~mask) | i_byte  ≡  (0x12345678 & ~0x00FF0000) | 0x00AB0000  ≡  (0x12345678 & 0xFF00FFFF) | 0x00AB0000  ≡ 0x12005678 | 0x00AB0000  ≡  0x12AB5678`.

---


### 2.61 ⬥⬥
Write C expressions that evaluate to `1` when the following conditions are true and to `0` when they are false. Assume `x` is of type `int`.

A. Any bit of `x` equals 1. </br>
B. Any bit of `x` equals 0. </br>
C. Any bit in the least significant byte of `x` equals `1`. </br>
D. Any bit in the most significant byte of `x` equals `0`. </br>

Your code should follow the bit-level integer coding rules (page 128), with the additional restriction that you may not use equality (`==`) or inequality (`!=`) tests.

---

**A.** Any bit of `x` equals 1.
````C
!!x
````
If any bit in `x` equals `1`, then `!x` will evaluate to `0` and `!!x` will evaluate to `1`. If no bit in `x` equals `1`, that is, every bit in `x` equals `0`, then `!x` will evaluate to `1` and `!!x` will evaluate to `0`.

**B.** Any bit of `x` equals 0.
````C
!!~x
````
If any bit in `x` equals `0`, then at least one bit in `~x` will be `1`. Therefore, `!~x` will evaluate to `0` and `!!~x` will evaluate to `1`. If no bit in `x` equals `0`, that is, every bit in `x` equals `1`, then every bit in `~x` will equal `0`. Therefore, `!~x` will evaluate `1` and `!!~x` will evaluate `0`.

**C.** Any bit in the least significant byte of `x` equals `1`.
````C
!!(x & 0xFF)
````
If any bit in the least significant byte of `x` equals `1`, then at least one bit `(x & 0xFF)` will equal 1. Therefore, `!(x & 0xFF)` will evaluate to `0` and `!!(x & 0xFF)` will evaluate to `1`. If no bit in the least significant byte of `x` equals `1`, that is, every bit in the least significant byte of `x` equals `0`, then no bit in `(x & 0xFF)` will equal 1. Therefore, `!(x & 0xFF)` will evaluate to `1` and `!!(x & 0xFF)` will evaluate to `0`.

**D.** Any bit in the most significant byte of `x` equals `0`.
````C
!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)
````
As described in the `Bit-Level Integer Coding Rules` section immediately preceeding this problem in the text, the expression `((x >> ((sizeof(int) - 1) << 3)) & 0xFF)` will extract the most significant byte of `x` and position it in the least significant byte, with all other bytes being `0x00`. Hence, the expression `!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will yield the same result except the bits in the least significant byte will all be flipped, due to `~x`.
If any bit in the most significant byte of `x` equals `0`, then at least one bit in `((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will equal `1` by our reasoning above. Therefore, `!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `0` and `!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `1`. If no bit in the most significant byte of `x` equals `0`, that is, every bit in the most significant byte of `x` equals `1`, then every bit in `((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will equal `0`. Therefore, `!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `1` and `!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `0`.

---


### 2.62 ⬥⬥⬥
Write a function `int_shifts_are_arithmetic()` that yields `1` when run on a machine that uses arithmetic right shifts for data type `int` and yields `0` otherwise. Your code should work on a machine with any word size. Test your code on several machines.

---

````C
int int_shifts_are_arithmetic() {
  int test = -1;
  return !(test ^ (test >> 1))
}
````

Our function `int_shifts_are_arithmetic()` first initializes the variable `test` with the value `-1`. Therefore, all bits in `test` will be set to `1`. If a machine uses arithmetic right shifts for data type `int`, then the expression `(test >> 1)` will also be represented will all bits set to `1`. Therfore the bitwise XOR expression `(test ^ (test >> 1))` will result in all bits set to `0` and `!(test ^ (test >> 1))` will evaluate to `1`. If a machine uses a logical right shift for data type `int`, then the expression `(test >> 1)` will result in all bits set to `1` except the most significant bit will be set to `0`. Therfore the bitwise XOR expression `(test ^ (test >> 1))` will result in all bits set to `0` except the most significant bit will be set to `1` and `!(test ^ (test >> 1))` will evaluate to `0`.

---


### 2.63 ⬥⬥⬥
Fill in code for the following C functions. Function `srl` performs a logical right shift using an arithmetic right shift (given by value xsra), followed by other operations not including right shifts or division. Function `sra` performs an arithmetic right shift using logical right shift (given by value `xrsl`), followed by other operations not including right shifts or division. You may use the computation `8*sizeof(int)` to determine `ω`, the number of bits in data type `int`. The shift amount `k` can range from `0` to `ω - 1`.

---

````C
unsigned srl(unsigned x, int k) {
  // Perform shift arithmetically
  unsigned xsra = (int) x >> k;

  // Get number of bits for data type int
  int w = 8 * sizeof(int);

  // Create mask for k most significant bits
  int mask = (int) -1 << (w - k);

  return xsra & ~mask;
}

int sra(int x, int k) {
  // Perform shift logically
  int xsrl = (unsigned) x >> k;

  // Get number of bits for data type int
  int w = 8 * sizeof(int);

  // Create mask for k most significant bits
  int mask = (int) -1 << (w - k);

  // Create mask for the most significant bit
  int top_mask = (unsigned) 1 << (w - 1);

  // Clear mask if most significant bit of x is 0
  mask &= !(x & top_mask) - 1;

  return xsrl | mask;
}
````

The function `srl` performs a logical right shift using arithmetic right shifts as follows. First, we perform an arithmetic right shift with the statement `unsigned xsra = (int) x >> k`. Then the expression `int mask = (int) -1 << (w - k)` will yield a mask with only the `k` most significant bits equal to `1`. Finally, we perform the bitwise NOT operation on the mask so that only the `k` most significant bits in the mask are `0`. Therefore, the expression `xsra & ~mask` turns the `k` most significant bits in `xrsa` to `0`, as would be the case in a logical right shift.

The function `sra` performs an arithmetic right shift using logical right shifts as follows. First, we perform an arithmetic right shift with the statement `unsigned xsrl = (int) x >> k`. Then the expression `int mask = (int) -1 << (w - k)` will yield a mask with only the `k` most significant bits equal to `1`. We must create another mask for just the most significant bit with the expression `int top_mask = (unsigned) 1 << (w - 1)`. Now we must test if the most significant bit of `x` is a `1` or `0`. If the most significant bit of `x` is a `1`, then the expression `!(x & top_mask) - 1` will evaluate to `-1`. Therefore, `mask` remains unchanged by the full expression `mask &= !(x & top_mask) - 1`, since `-1` is represented by all bits equaling `1`. Then the expression `xsrl | mask` sets the `k` most significant bits of `xsrl` to `1` as it would be in an arithmetic shift. If the most significant bit of `x` is a `0`, then the expression `!(x & top_mask) - 1` will evaluate to `0`. Therefore, all bits in `mask` are cleared to `0` by the full expression `mask &= !(x & top_mask) - 1`. Then the expression `xsrl | mask` will just yield `xsrl` as would be calculated in an arithmetic shift.

---


### 2.64 ⬥
Write code to implement the following function:
````C
/* Return 1 when any odd bit of x equals 1; 0 otherwise.
  Assume w=32 */
int any_odd_one(unsigned x) {

}
````
Your function should follow the bit-level integer coding rules (page 128), except that you may assume that data type `int` has `ω = 32` bits.

---

````C
int any_odd_one(unsigned x) {
  return !!(x & 0xAAAAAAAA);
}
````

The function `any_odd_one` works as follows. Let us assume that `x` has at least one odd bit that equals `1`. Then the expression `(x & 0xAAAAAAAA)` will result in at least one bit being equal to `1`, since `0xA = 0b1010`. Note that the least significant bit is considered the `0`th bit so all the `1` bits in `0b1010` are odd bits. Then the expression `!(x & 0xAAAAAAAA)` will evaluate to `0`, since we have argued that at least one bit will be `1` in `(x & 0xAAAAAAAA)`. Therefore, `!!(x & 0xAAAAAAAA)` will evaluate to `1` as required. Now let us assume that `x` has no odd bits equal to `1`. Then the expression `(x & 0xAAAAAAAA)` will result in no bits equal to `1`. Hence, the expression `!(x & 0xAAAAAAAA)` will evaluate to `1` and `!!(x & 0xAAAAAAAA)` will evaluate to `0`.

----


### 2.65 ⬥⬥⬥⬥
Write code to implement the following function:
````C
/* Return 1 when x contains an odd number of 1s; 0 otherwise.
   Assume w=32 */
int odd_ones(unsigned x);
````
Your function should follow the bit-level integer coding rules (page 128), except that you may assume that data type `int` has `ω = 32` bits.

Your code should contain a total of at most `12` arithmetic, bitwise, and logical operations.

---

````C
int odd_ones(unsigned x) {
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return x & 0x01;
}
````

The function `odd_ones` behaves as follows. Note that we are assuming a word size of `32` bits, as given to us in the problem. It is also important to note, that since `x` is of type `unsigned`, any right shifts on `x` will be logical. The first expression `x ^= x >> 16` logically right shifts `16` bits and then performs the bitwise XOR operation with the original `x`. This has the effect of bitwise XORing the `ω/2` most significant bits in `x` with the `ω/2` least significant bits in `x`. It is clear to see, since the right shifts are logical, that the `ω2` most significant bits after these two operations will equal `0`. Now let us consider the `ω/2` least significant bits after these two operations. If two bits that were XORed together both equaled `1`, the resulting bit would be `0`. If one bit were `1` and the other `0`, the resulting bit would be `1`, and if both bits equaled `0`, the resulting bit would be `0`. In this manner, we maintain the parity of the number of `1` bits of the original `x` in the `ω/2` least significant bits of the new `x`, since we only zero out pairs of `1` bits. This process of halving continues until we have performed the bitwise XOR operation between all original bits in `x`. Since we know the parity of the number of `1` bits has remain unchanged in the remaining bit, if the least significant bit equals `1`, then we know that the original `x` contains an odd number of `1` bits. If the least significant bit equals `0`, then we know that the original `x` contains an even number of `1` bits. Finally, we must use the bitwise AND operation in the expression `x & 0x01` to return the least significant bit.

---


### 2.66 ⬥⬥⬥
Write code to implement the  following function:
````C
/*
 * Generate mask indicating leftmost 1 in x. Assume w = 32.
 * For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
 * If x = 0, then return 0.
 */
int leftmost_one(unsigned x);
````
Your function should follow the bit-level integer coding rules (page 128), except that you may assume that data type `int` has `ω = 32` bits.

Your code should contain a total of at most `15` arithmetic, bitwise, and logical operations.

*Hint*: First transform `x` into a bit vector of the form `[0...01...1]`.

---

````C
int leftmost_one(unsigned x) {
  x |= x >> 16;
  x |= x >> 8;
  x |= x >> 4;
  x |= x >> 2;
  x |= x >> 1;
  return (x >> 1) + (x && 1);
}
````

The function `leftmost_one` behaves as follows. Note that we are assuming a word size of `32` bits, as given to us in the problem. It is also important to note, that since `x` is of type `unsigned`, any right shifts on `x` will be logical. The first expression `x |= x >> 16` logically right shifts `x` by `16` bits and then performs the bitwise OR operation with the original `x`. This has the effect of copying the `ω/2` most significant bits in `x` that equal `1` into the `ω/2` least significant bits. Note that if no bit in the `ω/2` most significant bits equal `1`, then `x` remains unchanged. By continuing this process by halving the number of right shifts each time, we can ensure that every bit less significant than the leftmost `1` bit is set to `1`. Now let us consider the return expression `(x >> 1) + (x && 1)`. If `x = 0`, then it is clear that both `(x >> 1)` and `(x && 1)` evaluate to `0`, so we return `0` as required. If `x` is not `0`, we know we have transformed `x` into the bit vector `[0...01...1]`, where all and only the bits less significant and including the leftmost `1` in `x` equal `1`, by our reasoning above. Then it is clear that `(x && 1)` evaluate to `1`. The expression `(x >> 1)` will shift all bits to the right, but then adding `1` will cause all bits to carry over into the bit which held the leftmost `1` in the original `x`. For example, if `x = 0b1111`, `(x >> 1)` will evaluate to `x = 0b0111`, but then `(x >> 1) + 1` evaluates to `0b1000`.

----


### 2.67 ⬥⬥
You are given the task of writing a procedure `int_size_is_32()` that yields `1` when run on a machine for which an int is 32 bits, and yields `0` otherwise. You are not allowed to use the `sizeof` operator. Here is the first attempt:
````C
/* The following code does not run properly on some machines */
int bad_int_size_is_32() {
  // Set most significant bit (msb) of 32-bit machine
  int set_msb = 1 << 31;
  // Shift past msb of 32-bit word *
  int beyond_msb = 1 << 32;

  // set_msb is nonzero when word size >= 32
  // beyond_msb is zero when word size <= 32
  return set_msb && !beyond_msb;
}
````
When compiled and run on a 32-bit SUN SPARC, however, this procedure returns `0`. The following compiler message gives us an indication of the problem:
````
warning: left shift count >= width of type
````
A. In what way does our code fail to comply with the C standard? </br>
B. Modify the code to run properly on any machine for which data type `int` is at least 32 bits. </br>
C. Modify the code to run properly on any machine for which data type int is at least 16 bits.

---

**A.** In what way does our code fail to comply with the C standard?

As explained in the `Aside` section on page 59 of the text, the behavior of shifting by some value `k ≥ ω` is not defined in the C standard. Therefore, we cannot assume that the expression `1 << 32` is zero when `ω ≤ 32`.

**B.** Modify the code to run properly on any machine for which data type `int` is at least 32 bits.

````C
int bad_int_size_is_32() {
  int set_msb = 1 << 31;
  int beyond_msb = set_msb << 1;

  return set_msb && !beyond_msb;
}
````
The behavior of this code will run properly on any machine for which data type `int` is at least 32 bits because there are no left shifts greater than or equal to 32. Hence, the C standard defines the intended behavior.

**C.** Modify the code to run properly on any machine for which data type `int` is at least 16 bits.
````C
int bad_int_size_is_32() {
  int set_msb = 1 << 15 << 15 < 1;
  int beyond_msb = set_msb << 1;

  return set_msb && !beyond_msb;
}
````
The behavior of this code will run properly on any machine for which data type `int` is at least 16 bits because there are no left shifts greater than or equal to 16. Hence, the C standard defines the intended behavior.

----


### 2.68 ⬥⬥
Write code for a function with the following prototype:
````C
/*
 * Mask with least significant n bits set to 1
 * Examples: n -= 6 --> 0x3F, n = 17 --> 0x1FFFF
 * Assume 1 <= n <= w
 */
int lower_one_mask(int n);
````
Your function should follow the bit-level integer coding rules (page 128). Be careful of the case `n = ω`.

---

````C
int lower_one_mask(int n) {
  int w = sizeof(int) << 3;
  return (unsigned) -1 >> (w - n);
}
````

The function `lower_one_mask` behaves as follows. We first get the word size with the expression `sizeof(int) << 3`. Note that the `sizeof` function returns the number of bytes, so we must left shift by `3` to get the number of bits. Then we can simply return the expression `(unsigned) -1 >> (w - n)`. Since we are casting this expression to be `unsigned`, the value `-1` will be represented by `w` `1` bits and right shifts will be logical. Hence, right shifting by `(w - n)` will yield our desired mask. It is important to note that since we have the assumption that `1 ≤ n ≤ ω`, the we know that `0 ≤ (w - n) < ω`. Therefore, the behavior will be guaranteed by the C standard.

---


### 2.69 ⬥⬥⬥
Write code for a function with the following prototype.
````C
// Do rotating left shift. Assume 0 <= n < w
// Examples when x = 0x12345678 and w = 32:
//   n=4 -> 0x23456781, n=20 -> 0x67812345
unsigned rotate_left(unsigned x, int n);
````
Your function should follow the bit-level integer coding rules (page 128). Be careful of the case `n = 0`.

---

````C
unsigned rotate_left(unsigned x, int n) {
  int w = sizeof(int) << 3;

  unsigned upper = x << n;
  unsigned lower = x >> (w - n - 1) >> 1;

  return upper | lower;
}
````

The function `rotate_left` works as follows. Note that we are assuming that `0 ≤ n < ω`. We first get the word size with the expression `sizeof(int) << 3`. Note that the `sizeof` function returns the number of bytes, so we must left shift by `3` to get the number of bits. Next, we left shift `x` by `n` to set the `n` most significant bits with the expression `x << n`. Then we set the `ω - n` least significant bits with the expression `x >> (w - n - 1) >> 1`. It is important to note that we cannot simply use the expression `x >> (w - n)` in the case that `n = 0`, since the behavior for a shift of `ω` is not defined by the C standard. The expression `x >> (w - n - 1) >> 1` guarantees that we will never shift by a value greater than or equal to `ω`. Lastly, we perform the bitwise OR operation between the most significant bits and the least significant bits.

----


### 2.70 ⬥⬥
Write code for the function with the following prototype:
````C
/*
 * Return 1 when x can be represented as an n-bit, 2's complement
 * number; 0 otherwise
 * Assume 1 <= n <= w
 */
int fits_bits(int x, int n);
````
Your function should follow the bit-level integer coding rules (page 128).

---

````C
int fits_bits(int x, int n) {
  int w = sizeof(int) << 3;

  int bit_test = x << (w - n) >> (w - n);

  return x == bit_test;
}
````

The function `fit_bits` works as follows. Note that we are assuming that `1 ≤ n ≤ ω`. We first get the word size with the expression `sizeof(int) << 3`. Note that the `sizeof` function returns the number of bytes, so we must left shift by `3` to get the number of bits. Then we perform `ω - n` left shifts on `x` followed by `ω - n` right shifts. Two important things to note here is that the right shifts will be arithmetic shifts and that `0 ≤ ω - n < ω`, from our assumption that `1 ≤ n ≤ w`, so we know the behavior defined by the C standard. Lastly, if the original `x` and the left then right shifted version of `x` are equal, we return `1`. Otherwise, we return `0`. Now let us argue that this is correct.

If `x ≥ 0`, then `x` can be represented as an `n`-bit, 2's complement number if and only if the `ω - (n - 1)` most significant bits of `x` equal `0`. It is clear to see that the `ω - n` most significant bits will not fit in an `n` bit number. However, the most significant bit in a 2's complement number will make the number negative, so we must include this bit to equal `0`, since we have assumed `x ≥ 0`. If one of the `ω - n` most significant bits of `x` equals `1`, then it will be lost due to overflow from the left shifts. If the `ω - (n - 1)` most significant bit is `1`, then the arithmetic right shifts will cause sign extension. Therefore, `x` will not equal `bit_test` in our return expression. Hence, `fit_bits` works when `x ≥ 0`.

If `x < 0`, then `x` can be represented as an `n`-bit, 2's complement number if and only if the `ω - (n - 1)` most significant bits of `x` equal `1`. If the `ω - (n - 1)` most significant bits of `x` equal `1`, then it is clear that the `ω - n` most significant bits will overflow from the left shifts, but sign extension from the right shifts will replace them. Therefore `x` will equal `bit_test` in our return expression. Now let us consider the three cases where there is a `0` bit in the `ω - (n - 1)` most significant bits. If the most significant bit equals `0`, then `x ≥ 0`, which we have assumed to be false. If the `ω - (n - 1)` most significant bit equals `0` then sign extension will cause `x` to not equal `bit_test` in our return expression, which is what we want because this number would not be representable in only `n` bits. Similarly, if one of the other `ω - (n - 1)` most significant bits equals `0`, then it will be lost due to sign extension. But this number would not be representable in only `n` bits either, so this is correct.

---


### 2.71 ⬥
You just started working for a company that is implementing a set of procedures to operate on a data structure where `4` signed bytes are packed into a 32-bit `unsigned`. Bytes within the word are numbered from `0` (least significant) to `3` (most significant). You have been assigned the task of implementing a function for a machine using two's-complement arithmetic and arithmetic right shifts with the following prototype:
````C
/* Declaration of data type where 4 bytes are packed
   into an unsigned */
typedef unsigned packed_t;

/* Extract byte from word. Return as signed integer */
int xbyte(packed_t word, int bytenum);
````
That is, the function will extract the designated byte and sign extend it to be a 32-bit `int`.

Your predecessor (who was fired for incompetence) wrote the following code:
````C
/* Failed attempt at xbyte */
int xbyte(packed_t word, int bytenum) {
  return (word >> (bytenum << 3)) & 0xFF;
}
````

A. What is wrong with this code? </br>
B. Give a correct implementation of the function that uses only left and right shifts, along with one subtraction.

---
**A.** What is wrong with this code?

The failed attempt at `xbyte` fails to properly handle negative numbers, since the bitwise AND operation will set all bits except the least significant byte to `0`. That is, sign extension will not occur.

**B.** Give a correct implementation of the function that uses only left and right shifts, along with one subtraction.

````C
typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum) {
  int byte = word << ((3 - bytenum) << 3);

  return byte >> 24;
}
````

The function `xbytes` works as follows. Note that we are given the size of type `int` and `unsigned` are both 32 bits. We first left shift `word` so that the byte to be extracted is in the most significant byte position with the expression `word << ((3 - bytenum) << 3)`. Then we right shift `byte` to put the most significant byte, which is the byte to be extracted, into the least significant byte position. Since `byte` is of type `int`, the right shifts will be performed arithmetically and therefore sign extension will occur as desired.

---


### 2.72 ⬥⬥
You are given the task of writing a function that will copy an integer `val` into a buffer `buf`, but it should do so only if enough space is available in the buffer.

Here is the code you write:
````C
/* Copy integer into buffer if space is available */
/* WARNING: The following code is buggy */
void copy_int(int val, void *buf, int maxbytes) {
  if(maxbytes-sizeof(val) >= 0) {
      memcpy(buf, (void*)&val, sizeof(val));
  }
}
````
This code makes use of the library function `memcpy`. Although its use is a bit artificial here, where we simply want to copy an `int`, it illustrates an approach commonly used to copy larger data structures.

You carefully test the code and discover that it always copies the value to the buffer, even when `maxbytes` is too small.

A. Explain why the conditional test in the code always succeeds. *Hint*: The `sizeof` operator returns a value of type `size_t`.

B. Show how you can rewrite the conditional test to make it work properly.

---

**A.** Explain why the conditional test in the code always succeeds. *Hint*: The `sizeof` operator returns a value of type `size_t`.

The conditional test in the code always succeeds due to improperly handling the inequality operation between different types. The `sizeof` function returns a value of type `size_t`, which is unsigned. Therefore, the operation `maxbytes-sizeof(val)` will be evaluated as unsigned and hence will be greater than or equal to `0`. Thus, the condition `maxbytes-sizeof(val) >= 0` will always be true.

**B.** Show how you can rewrite the conditional test to make it work properly.

````C
void copy_int(int val, void *buf, int maxbytes) {
  if(maxbytes >= (int) sizeof(val)) {
    memcpy(buf, (void*)&val, sizeof(val));
  }
}
````

If the rewrite the conditional to treat the expression `sizeof(val)` as type `int`, by casting, then our code will work properly.

---


### 2.73 ⬥⬥
Write code for a function with the following prototype:
````C
/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y);
````
Instead of overflowing the way normal 2's complement addition does, saturating addition returns `TMax` when there would be positive overflow, and `TMin` when there would be negative overflow. Saturating arithmetic is commonly used in programs that perform digital signals processing.

Your function should follow the bit-level integer coding rules (page 128).

---

````C
int saturating_add(int x, int y) {
  int sum = x + y;
  int mask = INT_MIN;

  int overflow_flag = !(x & mask) && !(y & mask) && (sum & mask);
  int underflow_flag = (x & mask) && (y & mask) && !(sum & mask);

  (overflow_flag && (sum = INT_MAX) || underflow_flag && (sum = INT_MIN));

  return sum;
}
````

The function `saturating_add` works as follows. First, we add `x` and `y`. Next we create a mask with just the most significant bit set to `1` by using `INT_MIN`. Next we check if a positive overflow has occurred or if a negative underflow has occurred. A positive overflow occurs when `x > 0` and `y > 0`, but `sum < 0`. Performing the bitwise AND operation with the mask we just created will determine if a value is negative. Therefore, the expression `!(x & mask) && !(y & mask) && (sum & mask)` will only evaluate to `1` if `x > 0`, `y > 0`, and `sum < 0`. A negative underflow occurs when `x < 0` and `y < 0`, but `sum > 0`. Therefore, the expression `(x & mask) && (y & mask) && !(sum & mask)` will only evaluate to `1` if `x < 0`, `y < 0`, and `sum > 0`. Next, we exploit the short circuit evaluation of C to achieve control flow similar to an `If-Else` clause. The expression `overflow_flag && (sum = INT_MAX)` will result in `sum = INT_MAX` if `overflow_flag` equals `1`. If `overflow_flag` does equals `1`, then the second expression of the logical OR clause will not execute. Otherwise, the expression `underflow_flag && (sum = INT_MIN)` will result in `sum = INT_MIN` if `underflow_flag` equals `1`. If neither `overflow_flag` or `underflow_flag` equal `1`, then `sum` will remain unchanged, since no overflows have occurred.

---


### 2.74 ⬥⬥
Write a function with the following prototype:
````C
/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y);
````
This function should return `1` if the computation `x-y` does not overflow.

---

````C
int tsub_ok(int x, int y) {
  int dif = x - y;

  int overflow_flag = (x > 0) && (y < 0) && (dif < 0);
  int underflow_flag = (x < 0) && (y > 0) && (dif > 0);

  return !overflow_flag && !underflow_flag;
}
````

The function `tsub_ok` works as follows. First, we calculate the difference `x - y`. Next, we check if a positive overflow has occurred or if a negative underflow has occurred. A positive overflow occurs when `x > 0` and `y < 0`, but `dif < 0`. These inequalities are all checked in the expression `(x > 0) && (y < 0) && (dif < 0)`. A negative underflow occurs when `x < 0` and `y > 0`, but `dif > 0`. These inequalities are all checked in the expression `(x < 0) && (y > 0) && (dif > 0)`. Finally, if either positive overflow or negative underflow has occurred, the expression `!overflow_flag && !underflow_flag` will evaluate to `1`, and `0` otherwise.

----
