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

The `show_bytes` program was run on a 64 bit Ubuntu (Linux) and a 64 bit macOS machine. As can be observed in the output from the program calls, both machines are little-endian, since the bytes representing the integer argument `12345`, `0x3039` in hex, is stored as `39 30 00 00` by the machines. That is, the least significant byte comes first.

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

These different sample values were run on a little-endian 64 bit macOS machine.

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
  unsigned test = 1;
  char* pointer = (char*) &test;
  return (int) *pointer;
}
````

By definition, a little-endian machine will store an object in memory ordered from least significant byte to most and a big-endian machine will store an object in memory ordered from most significant byte to least. Therefore, we check the ordering of the bytes of a known value to determine the endianness of the machine our code is compiled and run on.

In our function `is_little_endian`, we first declare an `unsigned` variable, `test`, and initialize it to `1`. This variable's byte representation will have the least significant byte equal `0x01` and all other bytes equal `0x00`. Since the C standard guarantees `2` bytes for type `unsigned`, we are guaranteed that the least significant byte equals `0x01` and the most significant byte equals `0x00`. Next, we create a pointer to the first byte in the byte representation of `test` with `char* pointer = (char*) &test`. Finally, we dereference the pointer and return its value casted as an `int`.

On a little-endian machine, we know that an object will be stored in memory ordered from least significant byte to most. As we have argued above, the least significant byte of the `test` variable will equal `0x01`. The `pointer` pointer will reference the first byte of the `test` variable. Hence, when this procedure is run on a little-endian machine, it will return `1`, as required.

Conversely, on a big-endian machine, we know that an object will be stored in memory ordered from most significant byte to least. We have also argued that the most significant byte in `test` will equal `0x00`. Hence, when we return `(int) *pointer` on a big-endian machine, we will return `0`.

---


### 2.59 ⬥⬥
Write a C expression that will yield a word consisting of the least significant byte of `x` and the remaining bytes of `y`. For operands `x=0x89ABCDEF` and `y=0x76543210`, this would give `0x765432EF`.

---

````C
(x & 0xFF) | (y & ~0xFF)
````

Let us assume the word size is 4 bytes, although this expression will work with any word size. Now let us consider the behavior of this expression.

The expression `(x & 0xFF)` calculates the bitwise AND operation on the two operands `x` and `0xFF`. This has the result of only keeping the least significant byte of `x`, while the remaining bytes are all set to `0x00`. For example, if `x=0x89ABCDEF`, the expression `(x & 0xFF)` is equivalent to the expression `(0x89ABCDEF & 0x000000FF)`, which gives `0x000000EF`.

The expression `(y & ~0xFF)` calculates the bitwise AND on the two operands `y` and `~0xFF`. The expression `~0xFF` yields the bitwise NOT of `0xFF` which sets all bytes to `0xFF` except the least significant byte, which is set to `0x00`. Under our assumption of a 4 byte word size, this would yield `0xFFFFFF00`. Then it is clear that the result of the bitwise AND operation `(y & ~0xFF)` keeps all bytes of `y` except the least significant byte, which is set to `0x00`. For example, if `y=0x76543210`, the expression `(y & ~0xFF)` is equivalent to the expression `(0x76543210 & 0xFFFFFF00)`, which gives `0x76543200`.

Therefore, the full expression `(x & 0xFF) | (y & ~0xFF)` will yield the word consisting of the least significant byte of `x` and the remaining bytes of `y` by calculating the bitwise OR operation between the subexpressions `(x & 0xFF)` and `(y & ~0xFF)`. Continuing with the example values used above, the full expression is equivalent to `0x000000EF | 0x76543200`, which gives `0x765432EF`.

---


### 2.60 ⬥⬥
Suppose we number the bytes in a ω-bit word from `0` (least significant) to `ω/8-1` (most significant). Write code for the following C function, which will return an unsigned value in which byte `i` of argument `x` has been replaced by byte `b`:

````C
unsigned replace_byte(unsigned x, int i, unsigned char b);
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

The procedure `replace_byte` given above works as follows. First, we check that argument `i` is within the range of the size of an `unsigned` variable. If `i` is not a valid index, we return `x` unchanged. Otherwise, we create a mask for byte `i`. Note that `(i << 3)` yields the bit index from the byte index `i`, since a left shift of `3` is equivalent to multiplying by `8` and there are `8` bits in a byte. Hence, the expression `((unsigned) 0xFF) << (i << 3)` positions the `0xFF` value to the `i`th byte position. For example, if `i = 2`, then the expression `((unsigned) 0xFF) << (i << 3)` yields `0x00FF0000`. After creating the mask, we position the byte of character `b` to the `i`th byte in a similar manner. For example, if `b=0xAB`, then the expression `((unsigned) b) << (i << 3)` yields `0x00AB0000`. Lastly we return the expression `(x & ~mask) | i_byte`. In this expression, we first perform the bitwise AND between `x` and `~mask` which effectively zeros out the `i`th byte of `x`. Then we perform the bitwise OR between `x & ~mask` and `i_byte`, which returns an unsigned value in which byte `i` of argument `x` is replaced by byte `b`, as required. For example if `x = 0x12345678`, `i=2`, and `b=0xAB`, then it is clear to see that `(x & ~mask) | i_byte  ≡  (0x12345678 & ~0x00FF0000) | 0x00AB0000  ≡  (0x12345678 & 0xFF00FFFF) | 0x00AB0000  ≡ 0x12005678 | 0x00AB0000  ≡  0x12AB5678`.

---


### Bit-Level Integer Coding Rules
In several of the following problems, we will artificially restrict what programming constructs you can use to help you gain a better understanding of the bit-level, logic, and arithmetic operations of C. In answering these problems, your code must follow these rules.

* Assumptions
  * Integers are represented in two's complement form.
  * Right shifts of signed data are performed arithmetically.
  * Data type `int` is `ω` bits long. For some of the problems, you will be given a specific value of `ω`, but otherwise your could should work as long as `ω` is a multiple of `8`. You can use the expression `sizeof(int)<<3` to compute `ω`.
* Forbidden
  * Conditionals (`if` or `?:`), loops, switch statements, function calls, and macro invocations.
  * Division, modulus, and multiplication.
  * Relative comparison operators (`<`, `>`, `<=`, and `>=`).
* Allowed operations
  * All bit-level and logic operations.
  * Left and right shifts, but only with shift amounts between `0` and `ω - 1`.
  * Addition and subtraction.
  * Equality (`==`) and inequality (`!=`) tests. (Some of the problems do not allow these.)
  * Integer constants `INT_MIN` and `INT_MAX`.
  * Casting between data types `int` and `unsigned`, either explicitly or implicitly.

Even with these rules, you should try to make your code readable by choosing descriptive names and using comments to describe the logic behind your solutions. As an example, the following code extracts the most significant byte from integer argument `x`:

````C
/* Get most  significant byte from x */
int get_msb(int x) {
  /* Shift by w-8 */
  int shift_val = (sizeof(int)-1)<<3;
  /* Arithmetic shift */
  int xright = x >> shift_val;
  /* Zero all but LSB */
  return xright & 0xFF;
}
````

---


### 2.61 ⬥⬥
Write C expressions that evaluate to `1` when the following conditions are true and to `0` when they are false. Assume `x` is of type `int`.

**A.** Any bit of `x` equals 1.

**B.** Any bit of `x` equals 0.

**C.** Any bit in the least significant byte of `x` equals `1`.

**D.** Any bit in the most significant byte of `x` equals `0`.

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
!!((~x >> ((sizeof(int) - 1) << 3)) & 0xFF)
````

As described in the Bit-Level Integer Coding Rules, the expression `((x >> ((sizeof(int) - 1) << 3)) & 0xFF)` will extract the most significant byte of `x` and position it in the least significant byte, with all other bytes being `0x00`. Hence, the expression `(~x >> ((sizeof(int) -1 ) << 3)) & 0xFF` will yield the same result except the bits in the least significant byte will all be flipped, due to `~x`.
If any bit in the most significant byte of `x` equals `0`, then at least one bit in `(~x >> ((sizeof(int) -1 ) << 3)) & 0xFF` will equal `1` by our reasoning above. Therefore, `!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `0` and `!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `1`. If no bit in the most significant byte of `x` equals `0`, that is, every bit in the most significant byte of `x` equals `1`, then every bit in `(~x >> ((sizeof(int) -1 ) << 3)) & 0xFF` will equal `0`. Therefore, `!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `1` and `!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will evaluate to `0`.

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

Our function `int_shifts_are_arithmetic()` first initializes the variable `test` with the value `-1`. All bits in `test` will be set to `1`, due to the assumption in the Bit-Level Coding Rules that integers are represented in two's complement form. If a machine uses arithmetic right shifts for data type `int`, then the expression `(test >> 1)` will also be represented with all bits set to `1`. Therefore the bitwise XOR expression `(test ^ (test >> 1))` will result in all bits set to `0` and `!(test ^ (test >> 1))` will evaluate to `1`. If a machine uses a logical right shift for data type `int`, then the expression `(test >> 1)` will result in all bits set to `1` except the most significant bit will be set to `0`. Therefore the bitwise XOR expression `(test ^ (test >> 1))` will result in all bits set to `0` except the most significant bit will be set to `1` and `!(test ^ (test >> 1))` will evaluate to `0`.

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
  int mask = -1 << (w - k);

  return xsra & ~mask;
}

int sra(int x, int k) {
  // Perform shift logically
  int xsrl = (unsigned) x >> k;

  // Get number of bits for data type int
  int w = 8 * sizeof(int);

  // Create mask for k most significant bits
  int mask = -1 << (w - k);

  // Create mask for the most significant bit
  int msb_mask = (unsigned) 1 << (w - 1);

  // Clear mask if most significant bit of x is 0
  mask &= !(x & msb_mask) - 1;

  return xsrl | mask;
}
````

The function `srl` performs a logical right shift using arithmetic right shifts as follows. First, we perform an arithmetic right shift with the statement `unsigned xsra = (int) x >> k`. Then, the expression `int mask = -1 << (w - k)` will yield a mask with only the `k` most significant bits equal to `1`. Finally, we perform the bitwise NOT operation on the mask so that only the `k` most significant bits in the mask are `0`. Therefore, the expression `xsra & ~mask` turns the `k` most significant bits in `xrsa` to `0`, as would be the case in a logical right shift.

The function `sra` performs an arithmetic right shift using logical right shifts as follows. First, we perform an arithmetic right shift with the statement `unsigned xsrl = (int) x >> k`. As before, the expression `int mask = -1 << (w - k)` will yield a mask with only the `k` most significant bits equal to `1`. We must create another mask for just the most significant bit with the expression `int msb_mask = (unsigned) 1 << (w - 1)`. If the most significant bit of `x` is a `1`, then the expression `!(x & msb_mask) - 1` will evaluate to `-1`. Therefore, `mask` remains unchanged by the full expression `mask &= !(x & msb_mask) - 1`, since `-1` is represented by all bits equaling `1` in two's complement form. Then the expression `xsrl | mask` sets the `k` most significant bits of `xsrl` to `1` as it would be in an arithmetic shift. If the most significant bit of `x` is a `0`, then the expression `!(x & msb_mask) - 1` will evaluate to `0`. Therefore, all bits in `mask` are cleared to `0` by the full expression `mask &= !(x & top_mask) - 1`. Then the expression `xsrl | mask` will just yield `xsrl` as would be calculated in an arithmetic shift.

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

With the given assumption that data type `int` has `ω = 32` bits, the function `any_odd_one` works as follows. Let us assume that `x` has at least one odd bit that equals `1`. Then the expression `(x & 0xAAAAAAAA)` will result in at least one bit being equal to `1`, since `0xA = 0b1010`. Note that the least significant bit is considered the `0`th bit so all the `1` bits in `0b1010` are odd bits. Then the expression `!(x & 0xAAAAAAAA)` will evaluate to `0`, since we have argued that at least one bit will be `1` in `(x & 0xAAAAAAAA)`. Therefore, `!!(x & 0xAAAAAAAA)` will evaluate to `1` as required. Now let us assume that `x` has no odd bits equal to `1`. Then the expression `(x & 0xAAAAAAAA)` will result in no bits equal to `1`. Hence, the expression `!(x & 0xAAAAAAAA)` will evaluate to `1` and `!!(x & 0xAAAAAAAA)` will evaluate to `0`.

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

Consider the bitwise XOR operation. We know from the definition of the operator that `0 ^ 0 = 0`, `1 ^ 0 = 1`, `0 ^ 1 = 1`, and `1 ^ 1 = 0`. Therefore, it is clear to see that if the operands contain an odd number of `1`'s, then the operation yields a `1`, and if the operands contain an even number of `1`'s, then the operation yields a `0`. Furthermore, observe that the parity of `1`'s in the operands is preserved in the result. That is, if the operands contain an odd number of `1`'s, such as `1 ^ 0`, then the result also contains an odd number of `1`'s, as in `1 ^ 0 = 1`. Likewise, if the operands contain an even number of `1`'s, such as `1 ^ 1`, then the result also contains an even number of `1`'s, as in `1 ^ 1 = 0`.

Now let us discuss our `odd_ones` function. Note that we are given the assumption that data type `int` has `ω = 32` bits. The function works by performing the bitwise XOR operation between all `32` bits in `x`. As argued above, the result of these operations will have the same parity as the number of `1`'s in the original `x`. Hence, if the result is `1`, then we know `x` contains an odd number of `1`'s. Therefore, the bitwise AND operation between the result and `0x01` will return `1`. However, if the result is `0`, then we know `x` contains an even number of `1`'s and the bitwise AND operation between the result and `0x01` will return `0`.

We are able to calculate the bitwise XOR operation between all bits in `x` by XORing the `ω/2 = 16` most significant bits with the `16` least significant bits with the expression `x ^= x >> 16`. After this operation, only the `16` least significant bits of `x` represent the result we are interested in. We  then repeat this process with the `16/2 = 8` most significant bits of the significant result and the `8` least significant bits. This halving and XORing process continues until all bits have been XORed together. Then we simply return the least significant bit, which has the same parity of `1`'s as in the original `x`.

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

Consider a bit vector of the form `[0...01...1]`, where every bit less significant, and including, the leftmost `1` bit in `x` equals `x`. That is, only the bits more significant than the leftmost `1` in `x` equal `0`. A logical right shift of `1` on the bit vector will have the form `[0...001...1]`, where every and only bit less significant than the leftmost `1` in `x` equals `1`. However, an addition of `1` to the result of the right shift will yield `[0...010...1]`, where only the left most bit in `x` equals `1`. It is important to consider the case when `x = 0`, however, in which case the right shift will yield the bit vector `[0...0]` and thus, the addition will yield `[0...01]`. Hence, we only want to add `1` to the right shift result when `x` is not `0`. The expression `(x >> 1) + (x && 1)` handles both the right shift and addition properly, assuming `x` is in the correct bit vector form described above.

Now we must modify `x` to achieve the bit vector form `[0...01...1]` as needed. The expression `x |= x >> 16` will keep the leftmost `1` bit in place and, if applicable, will set the bit `16` binary places less significant equal to `1`. Hence, the leftmost bit and every `16`th bit after the leftmost bit up to the least significant bit is guaranteed to equal `1`. The expression `x |= x >> 8` behaves similarly, but results in the leftmost bit and every `8`th bit after the leftmost bit up to the least significant bit guaranteed to equal `1`. This process continues with the expressions `x |= x >> 4`, `x |= x >> 2`, and `x |= x >> 1`, which guarantees that every bit less significant and including the leftmost `1` bit equal `1` as required.

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

**A.** In what way does our code fail to comply with the C standard?

**B.** Modify the code to run properly on any machine for which data type `int` is at least 32 bits.

**C.** Modify the code to run properly on any machine for which data type int is at least 16 bits.

---

**A.** In what way does our code fail to comply with the C standard?

As explained in the Aside section on page 59 of the text, the behavior of shifting by some value `k ≥ ω` is not defined by the C standard. Therefore, we cannot assume that the expression `1 << 32` is zero when `ω ≤ 32`.

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

We know that in two's complement form, `-1` is represented with all bits set to `1`. Therefore, if `ω` is the number of bits used for data type `int` and `unsigned`, then the expression `(unsigned) -1 >> (w - n)` will create a mask with the `n` least significant bits sets to `1`. Since we are casting the right shift expression to be of type `unsigned`, the right shifts will be logical. Note that if we used the expression `(unsigned) 1 << n` instead, the case when `n = ω` would have undefined behavior, as shifting by some value `k ≥ ω` is not defined by the C standard. Our expression could have the same undefined behavior in the case when `n = 0`, however we are given the assumption that `1 ≤ n ≤ w`, so `0 ≤ w - n ≤ w - 1`. Hence, our function produces well defined behavior given the input assumption.

---


### 2.69 ⬥⬥⬥
Write code for a function with the following prototype:
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

To generate the rotating left shift, we can calculate the normal non-rotating left shift result and the rotated bits. Then a simply bitwise OR operation between the two will yield our desired result. The expression `x << n` calculates the non-rotating bits of the left shift. The expression `x >> (w - n - 1) >> 1` left shifts `x` `w - n` times to yield the rotated bits. Note that we cannot use the expression `x >> (w - n)` in case `n = 0`, as this would have undefined behavior, as shifting by some value `k ≥ ω` is not defined by the C standard. Our expression always has defined behavior due to the input assumption that `0 ≤ n < w`, and therefore `0 ≤ w - n - 1 ≤ w - 1`.

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

  int bit_test = (x << (w - n)) >> (w - n);

  return x == bit_test;
}
````

The function `fit_bits` works as follows. First, we perform `ω - n` left shifts on `x` followed by `ω - n` right shifts. Two important things to note here is that the right shifts will be arithmetic shifts and that `0 ≤ ω - n < ω`, from our assumption that `1 ≤ n ≤ w`, so we know the behavior defined by the C standard. Lastly, if the original `x` and the expression `(x << (w - n)) >> (w - n)` are equal, we return `1`. Otherwise, we return `0`. Now let us argue that this is correct.

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

**A.** What is wrong with this code?

**B.** Give a correct implementation of the function that uses only left and right shifts, along with one subtraction.

---

**A.** What is wrong with this code?

The failed attempt at `xbyte` fails to properly handle negative numbers, since the bitwise AND operation will set all bits except the least significant byte to `0`. Proper sign extension for a negative number would have all bits not in the least significant byte equal `1`.

**B.** Give a correct implementation of the function that uses only left and right shifts, along with one subtraction.

````C
typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum) {
  int byte = word << ((3 - bytenum) << 3);

  return byte >> 24;
}
````

To properly handle negative numbers, we need to ensure that proper sign extension takes place. In order to do this, we must first left shift the byte to be extracted to the most significant byte position and then right shift to the least significant byte position. By placing the byte in the most significant byte position, we ensure our right shifts will have correct sign extension.

Since there are `4` bytes in the type `packed_t` data type numbered  from `0` to `3`, the expression `((3 - bytenum) << 3)` gives the number of bits to  left shift in order for the byte to be extracted to occupy the most  significant byte place. Finally, since we know the byte to be extracted is in the most significant byte place, we can right shift by `24` to move it  into the least  significant byte place with proper sign extension.

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

**A.** Explain why the conditional test in the code always succeeds. *Hint*: The `sizeof` operator returns a value of type `size_t`.

**B.** Show how you can rewrite the conditional test to make it work properly.

---

**A.** Explain why the conditional test in the code always succeeds. *Hint*: The `sizeof` operator returns a value of type `size_t`.

The conditional test in the code always succeeds due to improperly handling the inequality operation between different types. The `sizeof` function returns a value of type `size_t`, which is unsigned. Therefore, the operation `maxbytes-sizeof(val)` will be evaluated as unsigned and hence will always be greater than or equal to `0`. Thus, the condition `maxbytes-sizeof(val) >= 0` will always be true.

**B.** Show how you can rewrite the conditional test to make it work properly.

````C
void copy_int(int val, void *buf, int maxbytes) {
  if(maxbytes >= (int) sizeof(val)) {
    memcpy(buf, (void*)&val, sizeof(val));
  }
}
````

If we cast the expression `sizeof(val)` as type `int`, then our conditional will work as expected.

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

Overflow from addition can occur in two cases. If a positive value is added to a positive value and overflow occurs, the resulting sum will be negative. If a negative value is added to a negative value and overflow occurs, also known as underflow in this case, the resulting sum will be positive.

Since we are forbidden to use comparative operators in the bit-level integer coding rules, we can test if a two's complement `int` is negative by checking if the most significant bit equals `1`. Note that `INT_MIN` will only have the most significant bit set to `1`, which is exactly the mask we are looking for. Hence, the expressions `(x & mask)` and `!(x & mask)` will return `1` when `x` is negative and not negative, respectively.

The expression `!(x & mask) && !(y & mask) && (sum & mask)` will yield `1` if `x` is positive, `y` is positive, and `sum` is negative, indicating a positive overflow has occurred. The expression `(x & mask) && (y & mask) && !(sum & mask)` will yield `1` if `x` is negative, `y` is negative, and `sum` is positive, indicating a negative underflow has occurred.

Lastly, we can exploit the short circuit evaluation of C to achieve control flow similar to an `If-Else` clause. The expression `overflow_flag && (sum = INT_MAX)` will result in `sum = INT_MAX` if `overflow_flag` equals `1`. If `overflow_flag` does equals `1`, then the second expression of the logical OR clause will not execute. Otherwise, the expression `underflow_flag && (sum = INT_MIN)` will result in `sum = INT_MIN` if `underflow_flag` equals `1`. If neither `overflow_flag` or `underflow_flag` equal `1`, then `sum` will remain unchanged, since no overflows have occurred.

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

Overflow from subtraction can occur in two cases. If a positive value is  subtracted from a negative value and overflow occurs, the resulting difference will be a positive value. After calculating the difference `dif = x - y`, we can test if a positive overflow happened with the expression `(x > 0) && (y < 0) && (dif < 0)`. If a negative value is subtracted from a positive value and overflow occurs, the resulting difference will be a negative value. We can test if a negative overflow occurred, also known as an underflow, with the expression `(x < 0) && (y > 0) && (dif > 0)`. If neither of these checks returns true, then no overflow has occured.

----


### 2.75 ⬥⬥⬥
Suppose we want to compute the complete `2ω`-bit representation of `x • y`, where both `x` and `y` are unsigned, on a machine for which data type `unsigned` is `ω` bits. The low-order `ω` bits of the product can be computed with the expression `x*y`, so we only require a procedure with prototype:
````C
unsigned unsigned_high_prod(unsigned x, unsigned y);
````
that computes the high-order `ω` bits of `x • y` for unsigned variables.

We have access to a library function with prototype:
````C
int signed_high_prod(int x, int y);
````
that computes the high-order `ω` bits of `x • y` for the case where `x` and `y` are in two's-complement form. Write code calling this procedure to implement the function for unsigned arguments. Justify the correctness of your solution.

*Hint*: Look at the relationship between the signed product `x • y` and the unsigned product `x' • y'` in the derivation of Equation 2.18.

---

````C
unsigned unsigned_high_prod(unsigned x, unsigned y) {
  int w = sizeof(unsigned) << 3;

  int x_sign = x >> (w - 1);
  int y_sign = y >> (w - 1);

  int high_signed_prod = signed_high_prod(x, y);

  return high_signed_prod + (x_sign * y) + (y_sign * x);
}
````

Equation 2.18 on page 98 of the text shows the bit-level equivalence of unsigned and two's complement multiplication. Namely, for unsigned values `x'` and `y'`, and signed values `x` and `y`, we have that `(x' • y') = (x + x_{ω-1}2^ω) • (y + y_{ω-1}2^ω) = (x • y) + (x_{ω-1}y + y_{ω-1}x)•2^ω + (x_{ω-1}y_{ω-1})•2^{2ω}`. It is easy to see that this is correct, since the most significant bit in two's complement form  and unsigned form have values `-2^{ω-1}` and `2^{ω-1}`, respectively. Hence, when we consider a two's complement number `x` in unsigned, we must add `x_{ω-1}2^ω`, since `2^{ω-1} - -2^{ω-1} = 2•2^{ω-1} = 2^ω`.

In the derivation of Equation 2.18, we perform our calculations `mod 2^ω`, since we only have `ω` bit for the result. In this scenario, we must perform our calculations `mod 2^{2ω}`, since we are computing the `2ω`-bit representation of `x • y`. Therefore, we have the equation `(x' • y') mod 2^{2ω} = (x • y) + (x_{ω-1}y + y_{ω-1}x)•2^ω mod 2^{2ω}`.

Knowing this relationship, our function is easy to understand. First, we get the most significant bits of `x` and `y` with the expressions `x >> (w - 1)` and `y >> (w - 1)`, respectively. Then, using the `signed_high_prod` library function we have access to, we can obtain the `ω` most significant bits of the complete two's complement `2ω`-bit representation of `x • y`. Finally , we just need to add `(x_{ω-1}y + y_{ω-1}x)•2^ω` to the `signed_high_prod` result. However, since the least significant bit in our result will actually correspond to the `ω`th bit in the complete `2ω`-bit representation we must divide `(x_{ω-1}y + y_{ω-1}x)•2^ω` by `2^ω`. Thus, the C expression `high_signed_prod + (x_sign * y) + (y_sign * x)` properly returns the high-order `ω` bits of `x • y` for unsigned variables.

---

### 2.76 ⬥
The library function `calloc` has the following declaration:
````C
void *calloc(size_t nmemb, size_t size);
````
According to the library documentation, "The `calloc` function allocates memory for an array of `nmeb` elements of `size` bytes each. The memory is set to zero. If `nmeb` or `size` is zero, then `calloc` returns `NULL`."

Write an implementation of `calloc` that performs the allocation by a call to `malloc` and sets the memory to zero via `memset`. Your code should not have any vulnerabilities due to arithmetic overflow, and it should work correctly regardless of the number of bits used to represent data of type `size_t`.

As a reference, functions `malloc` and `memset` have the following declarations:
````C
void *malloc(size_t size);
void *memset(void *s, int c, size_t n);
````

---

````C
void *calloc(size_t nmemb, size_t size) {
  if(nmemb == 0 || size == 0) {
    return NULL;
  }

  size_t buf_size = nmemb * size;

  // Check for overflow
  if(buf_size/nmemb != size) {
    return NULL;
  }

  void * buf = malloc(buf_size);
  if(buf != NULL) {
    memset(buf, 0, buff_size);
  }

  return buf;
}
````

As required, if either `nmemb` or `size` are zero, our function returns `NULL`. Next we calculate the size of our memory buffer with the expression `nmemb * size`. To ensure that no overflow has occurred in the previous multiplication, we can check that `buf_size/nmemb == size`. If `buf_size/nmemb` does not equal `size`, then overflow has occurred and we return `NULL`. Otherwise, we allocate memory with `malloc(buf_size)`, zero out the memory with `memset(buf, 0, buff_size)`, and return `buf`.

---


### 2.77 ⬥⬥
Suppose we are given the task of generating code to multiply integer variable `x` by various different factors `K`. To be efficient, we want to use only the operations `+`, `-`, and `<<`. For the following values of `K`, write C expressions to perform the multiplication using at most three operations per expression.

**A.** `K = 17`

**B.** `K = -7`

**C.** `K = 60`

**D.** `K = -112`

---

**A.** `K = 17`
````C
x + (x << 4)
````

**B.** `K = -7`
````C
x - (x << 3)
````

**C.** `K = 60`
````C
(x << 6) - (x << 2)
````

**D.** `K = -112`
````C
(x << 4) - (x << 7)
````

---


### 2.78 ⬥⬥
Write code for a function with the following prototype:
````C
/* Divide bypower of 2. Assume 0 <= k <= w-1 */
int divide_power2(int x, int k);
````
The function should compute `x/2^k` with correct rounding, and it should follow the bit-level integer coding rules (page 128).

---

````C
int divide_power2(int x, int k) {
  int neg_flag = x & INT_MIN;
  neg_flag && (x = (x + (1 << k) - 1));

  return x >> k;
}
````

As described in the text, integer division always rounds towards zero. Therefore, if `x >= 0`, then the expression `x >> k` will compute `x/2^k` with correct rounding. The principle of two's complement division by a power of 2, rounding down, is stated and derived on page 105 of the text. Conversely, if `x < 0`, then the expression `(x + (1 << k) - 1) >> k` will compute `x/2^k` with correct rounding. The principle of two's complement division by a power of 2, rounding up, is stated and derived on page 106 of the text.

With this in mind, the function `divide_power2` behaves as follows. First, we set `neg_flag` to flag whether `x` is negative. Note that `INT_MIN` is represented by a single `1` bit in the most significant bit position in two's complement. Hence, the bitwise AND operation `x & INT_MIN` will return `INT_MIN` if `x < 0` and `0` otherwise. Next, we exploit the short circuit evaluation of conditionals in C, since we are not allowed to use `if` statements. If `neg_flag = 1`, then the expression `x = (x + (1 << k) >> 1))` will be executed which sets `x` according to the principle of two's complement division by a power of 2, rounding down, described above. If `neg_flag = 0`, then the expression `x = (x + (1 << k) >> 1))` will be skipped. Lastly, we return the expression `x >> k`, where `x` has been modified to result in correct rounding.

---


### 2.79 ⬥⬥
Write code for a function `mul3div4` that, for integer argument `x`, computes `3*x/4` but follows the bit-level integer coding rules (page 128). Your code should replicate the fact that the computation `3*x` can cause overflow.

---

````C
int mul3div4(int x) {
  int prod = (x << 1) + x;

  // Same logic as homework problem 2.78
  int neg_flag = prod & INT_MIN;
  neg_flag && (prod = (prod + 3));
  int quot = prod >> 2;

  return quot;
}
````

The function `mul3div4` makes use of the same logic `divide_power2` from homework problem 2.78 uses to divide by a power of `2`. First, we calculate the product `x * 3` with the expression `(x << 1) + x`. Then, using the logic from the function `divide_power2` hardcoded with `k = 2`, we can calculate the quotient of dividing `x * 3` by `4`. Since the expression multiplication calculations occur first, this code replicates the fact that the computation `3*x` can cause overflow, as desired.

---


### 2.80 ⬥⬥
Write code for a function `threefourths` that, for integer argument `x`, computes the value of `(3/4)x` rounded toward zero. It should not overflow. Your function should follow the bit-level integer coding rules (page 128).

---

````C
int threefourths(int x) {
  int neg_flag = x & INT_MIN;

  // Split x into 2 least significant bits and w-2 most significant bits
  int least = x & 0x3;
  int most = x & ~0x3;

  // For least significant bits, perform multiplication, then division
  least = (least << 1) + least;
  int bias = (1 << 2) - 1;
  (neg_flag && (least += bias));
  least = least >> 2;

  // For most significant bits, perform division, then multiplication
  most = most >> 2;
  most = (most << 1) + most;

  return most + least;
}
````

Since our function `threefourths` should not overflow, we must first divide `x` by `4` and then multiply by `3`. This ensures that no overflow will take place. However, we must be careful that the rounding from the division operation does not affect the rounding over both operations. For example, consider the following code that makes use of the function `divide_power2` from homework problem 2.78 above.

````C
int wrong_threefourths(int x) {
  int quo = divide_power2(x, 2);
  return (quo << 1) + quo;
}

````

In `wrong_threefourths`, we divide first and then multiply, so no overflow occurs. However, the rounding from the operation `x >> 2` can cause incorrect results over the entire calculation. Consider the case when `x = 10`. It is clear that the result, rounded toward zero, should be `7`. However, `10 >> 2 = 2` and therefore `(2 << 1) + 2 = 6`. Note that this rounding issue does not occur when we multiply by `3` first and then divide by `4`.

When dividing by `4`, the rounding issue arises from losing the two least significant bits from right shifting. When multiplying by `3`, our overflow issue arises from the most significant bits. Therefore, to overcome both of the problems, we can split `x` into most and least significant bits. Then we can perform the division first on the most significant bits and we can perform the multiplication first on the least significant bits. Finally, we add both results back together. By doing this we are guarantee that overflow does not occur and no rounding issues occur.

The function `threefourths` works as follows. First, we set `neg_flag` to flag whether `x` is negative. Note that `INT_MIN` is represented by a single `1` bit in the most significant bit position in two's complement. Hence, the bitwise AND operation `x & INT_MIN` will return `INT_MIN` if `x < 0` and `0` otherwise. Next, we split `x` into the two least significant bits with the expression `x & 0x3` and the `ω - 2` most significant bits with the expression `x & ~0x3`. Next, we multiply the least significant bits by `3` with the expression `(least << 1) + least` and then we divide by `4` with the expression `least >> 2`. If `x < 0`, then we must add the bias `(1 << k) - 1`, in this case `(1 << 2) - 1 = 3` to these bits before the division operation. See the principle for two's complement division by a power of 2, round up, on page 1-6 of the text for this derivation. For the most significant bits, we divide by `4` with the expression `most >> 2` and then multiply by `3` with the expression `(most << 1) + most`. Note that we do not have to add a bias before we divide for the most significant bits if `x < 0` because it has already been added to the least significant bits. Lastly, we add the least and most significant bits back together with the expression `most + least` and return.

---


### 2.81 ⬥⬥
Write C expressions to generate the bit patterns that follow, where `a^k` represents `k` repetitions of symbol `a`. Assume a `ω`-bit data type. Your code may contain references to parameters `j` and `k`, representing the values of `j` and `k`, but not a parameter representing `ω`.

**A.** 1^{w-k}0^k

**B.** 0^{w-k-j}1^k0^j

---

**A.** 1^{w-k}0^k
````C
-1 << k
````

**B.** 0^{w-k-j}1^k0^j
````C
~(-1 << k) << j
````

---


### 2.82 ⬥⬥
We are running programs where values of type `int` are 32 bits. They are represented in two's complement, and they are right shifted arithmetically. Values of type `unsigned` are also 32 bits.

We generate arbitrary values `x` and `y`, and convert them to unsigned values as follows:
````C
/* Create some arbitrary values */
int x = random();
int y = random();
/* Convert to unsigned */
unsigned ux = (unsigned) x;
unsigned uy = (unsigned) y;
````

For each of the following C expressions, you are to indicate wherever or not the expression *always* yields `1`. If it always yields `1`, describe the underlying mathematical principles. Otherwise, give an example of arguments that make it yield `0`.

**A.** `(x<y) == (-x>-y)`

**B.** `((x+y)<<4) + y-x == 17*y+15*x`

**C.** `~x+~y+1 == ~(x+y)`

**D.** `(ux-uy) == -(unsigned)(y-x)`

**E.** `((x >> 2) << 2) <= x`

---

**A.** `(x<y) == (-x>-y)`

This expression will not always yield `1`. Consider the case where `x = TMin` and `y = 0`. Clearly `x = TMin < 0 = y`. However, `-x = -TMin = TMin < 0 = -0 = y`. Hence, the expression yields `0`.

**B.** `((x+y)<<4) + y-x == 17*y+15*x`

This expression will always yield `1`. Observe that `((x+y)<<4) + y-x = 16(x+y) + y-x = 17*y+15*x`.

**C.** `~x+~y+1 == ~(x+y)`

This expression will always yield `1`. Observe that `~x+~y+1 = (~x+1) + (~y+1) - 1 = -x - y - 1 = -(x+y) - 1 = ~(x+y)+1 - 1 = ~(x+y)`.

**D.** `(ux-uy) == -(unsigned)(y-x)`

This expression will always yield `1`. Observe that `(ux-uy) = ((unsigned) x - (unsigned) y) = (unsigned)(x-y) = -(unsigned)(y-x)`.


**E.** `((x >> 2) << 2) <= x`

This expression will always yield `1`. Observe that right shifting by `2` and then left shifting by `2` will essentially zero out the two least significant bits. If these two bits equaled `0` originally, then `((x >>2) << 2) = x`. However, if either or both of these bits equaled `1`, then `((x >> 2) << 2) < x`, since we assume type `int` is 32 bits and therefore the most significant bit, responsible for making an `int` negative would remain unchanged, and losing either of the two least significant bits would cause value to be lost.

---


### 2.83 ⬥⬥
Consider numbers having a binary representation consisting of an infinite string of the form `0,y,y,y,y,y,y...`, where `y` is a k-bit sequence. For example, the binary representation of `1/3` is `0.01010101...` (`y=01`), while the representation of `1/5` is `0.001100110011...` (`y=0011`).

**A.** Let `Y = B2U_k(y)`, that is, the number having binary representation `y`. Give a formula in terms of `Y` and `k` for the value represented by the infinite string. *Hint*: Consider the effect of shifting the binary point `k` positions to the right.

**B.** What is the numeric value of the string for the following values of `y`? </br>
(a) `101` </br>
(b) `0110` </br>
(c) `010011`

---

**A.** Let `Y = B2U_k(y)`, that is, the number having binary representation `y`. Give a formula in terms of `Y` and `k` for the value represented by the infinite string. *Hint*: Consider the effect of shifting the binary point `k` positions to the right.

````C
0.yyyyyy = Y / (2^k - 1)
````

Observe that `0.yyyyyy... << k = y.yyyyyy.... = Y + 0.yyyyyy`. Then we have that `0.yyyyyy... << k = 0.yyyyyy * 2^k = Y + 0.yyyyyy`. Hence, we get the formula `0.yyyyyy = Y / (2^k - 1)`.

**B.** What is the numeric value of the string for the following values of `y`? </br>
(a) `101`

It is clear that `Y = 0b101` and `k = 3`. Then, using the formula derived in part a above, we get that `0b101 / (2^3-1) = 5/7`. Thus, `y = 5/7`.

(b) `0110`

It is clear that `Y = 0b0110` and `k = 4`. Then, using the formula derived in part a above, we get that `0b0110 / (2^4-1) = 6/15 = 2/5`. Thus, `y = 2/5`.

(c) `010011`

It is clear that `Y = 0b010011` and `k = 6`. Then, using our formula derived in part a above, we get that `0b010011 / (2^6-1) = 19/63`. Thus, `y = 19/63`.

---


### 2.84 ⬥⬥
Fill in the return value for the following procedure, which tests whether its first argument is less than or equal to its second. Assume the function `f2u` returns an unsigned 32-bit number having the same bit representation as its floating-point argument. You can assume that neither argument is `NaN`. The two flavors of zero, `+0` and `-0` are considered equal.

````C
int float_le(float x, float y) {
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);

  // Get the sign bits
  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;

  // Give an expression using only ux, uy, sx, and sy
  return             ;
}
````

---

````C
int float_le(float x, float y) {
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);

  // Get the sign bits
  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;

  // Give an expression using only ux, uy, sx, and sy
  return (ux << 1 == 0 && uy << 1 == 0) ||    // If ux = uy = +/-0
         (sx && !sy) ||                       // If x<0 and y>0
         (sx && sy && ux >= uy) ||            // If x<0, y<0, and x<=y
         (!sx && !sy && ux <= uy);            // If x>0, y>0, and x<=y
}
````

There are four cases where `x ≤ y`. From the problem, we know that two flavors of zero, `+0` and `-0` are considered equal. The expression `ux << 1 == 0 && uy << 1 == 0` will evaluate to `1` if and only if `x = +/-0` and `x = +/-0`. The next case is when `x < 0` and `y > 0`. For this case, we can simply check that the sign of `x` equals `1` and the sign of `y` equals `0` with the expression `sx && !sy`. The third case occurs when `x < 0`, `y < 0`, and `x ≤ y`. We can check that both `x < 0` and `y < 0` using the sign bits, but then we must check that `ux ≥ uy`, since treating these numbers as unsigned will flip the inequality, using the expression `sx && sy && ux >= uy`. The last case is when `x > 0`, `y > 0`, and `x ≤ y`. Similarly, we can check that both `x > 0` and `y > 0` using the sign bits and then check that `x ≤ y` with the expression `!sx && !sy && ux <= uy`.

---


### 2.85 ⬥
Given a floating-point format with a `k`-bit exponent and an `n`-bit fraction, write formulas for the exponent `E`, the significand `M`, the fraction `ƒ`, and the value `V` for  the quantities that follow. In addition, describe the bit representation.

**A.** The number 7.0

**B.** The largest odd integer that can be represented exactly

**C.** The reciprocal of the smallest positive normalized values

---

**A.** The number 7.0

`E = 2`
`M = 0b1.11`
`ƒ = 0b0.11`
`V = 7.0`

In positional notation, it is easy to see that `7.0  = 0b111.0`. Then it is clear that the significand `M = 0b1.11` and the fraction `ƒ = 0b0.11`. It follows that the exponent need be `E = 2` and therefore `exp = 2 + bias = 2 + 2^{k-1}-1 = 1 + 2^{k-1}`. Thus, our bit pattern will look like `0 10...01 110...`.

**B.** The largest odd integer that can be represented exactly

`E = n`
`M = 0b1.1...`
`ƒ = 0b0.1...`
`V = 2^{n+2} - 1`

The largest odd integer we can represent exactly is represented by `n+1` `1` bits which are to the left of the binary point. Therefore, we must have that the significand `M = 0b1.1...1` with `n+1` `1` bits and the fraction `ƒ = 0b0.1...1` with `n` `1` bits. It is clear to see that `E = n` and therefore `exp = n + bias = n + 2^{k-1} - 1 = n-1 + 2^{k-1}`. Thus, our bit pattern will look like `0 {n-1 + 2^{k-1}} 1...1`.

**C.** The reciprocal of the smallest positive normalized value

`E = 2^{k-1} - 2`
`M = 0b1.0`
`ƒ = 0b0.0`
`V = 2^{2^{k-1} - 2}`

Note that the smallest positive normalized value is represented by the bit pattern `0 0...01 0...`. This value is equal to `2^{1-bias} = 2^{1 - 2^{k-1} + 1} = 2^{2 - 2^{k-1}}`. Then, the reciprocal of the smallest positive normalized value, `2^{2 - 2^{k-1}}`, will be equal to `2^{2^{k-1} - 2}`. If `M = 0b1.0` and `ƒ = 0b0.0`, then it is easy to see that `E = bias - 1 = 2^{k-1} - 1 - 1 = 2^{k-1} - 2`. This gives us `exp = 2^{k-1} - 2 + bias = 2^{k-1} - 2 + 2^{k-1}-1 = 2(2^{k-1}) - 3 = 2^k - 3`. Thus, our bit pattern will look like  `0 1...101 0...0`.

---


### 2.86 ⬥
Intel-compatible processors also support an "extended-precision"floating-point format with an 80-bit word divided into a sign bit, `k = 15` exponent bits, a  single *integer* bit, and `n = 63` fraction bits. The integer bit is an explicit copy of the implied bit in the IEEE floating-point representation. That is, it equals `1` for normalized values and `0` for denormalized values. Fill in the following table giving the approximate values of some "interesting" numbers in this format:

| Description                    | Value | Decimal |
| :----------                    | :---- | :------ |
| Smallest positive denormalized |       |         |
| Smallest positive normalized   |       |         |
| Largest normalized             |       |         |

This format  can be used in C programs compiled for Intel-compatible machines by declaring the data to be of type `long double`. However, it forces the compiler to generate code based on the legacy 8087 floating-point instructions. The resulting program will most likely run much slower than would be the case for data type `float` or `double`.

---

| Description                    | Value               | Decimal |
| :----------------------------- | :------------------ | :-------------------- |
| Smallest positive denormalized | `0 0....0 0 0...01` | 2^{-16445}            |
| Smallest positive normalized   | `0 0...01 1 0....0` | 2^{-16382}            |
| Largest normalized             | `0 1...10 1 1....1` | 2^{16384} - 2^{16320} |

Let us consider the smallest positive denormalized value in extended-precision floating point format. It is clear that the sign bit must equal `0`, because the want a positive value. Also, since this value if denormalized, all `k` exponent bits must equal `0` and the integer bit must equal `0`. If we keep all `63` fractions bits equal to `0`, then the value will equal `0`, which is not positive. Therefore, we must set the least significant fraction bit to `1`. Hence, the  bit pattern will look like `0 0....0 0 0...01`, which will have the value `(−1)^s * m * 2^{1 - bias} = 1 * 2^{-63} * 2^{−16382} = 2^{−16445}`.

The smallest positive normalized value in extended-precision floating point format must have the sign bit equal to `0`. Since this value is normalized, the integer bit must equal `1` and at least one exponent bit must equal `0` and at least one must equal `1`. Since we are looking for the smallest value, we just set the least significant exponent bit equal to `1`. Furthermore, we set all fraction bits equal to `0`. Hence, the bit pattern will look like `0 0...01 1 0....0`, which has the value `(−1)^s * m * 2^{e−bias} = 1 * 1 * 2^{1−16383} = 2^{-16382}`.

Now let us derive the largest normalized value in extended-precision floating point format. The sign bit is set to `0` because we are looking for the largest value. Since this value is normalized, the integer bit must equal `1` and at least one exponent bit must equal `0` and at least one must equal `1`. Since we are looking for the largest value, we must set all bits equal to `1`, except the least significant bit, which  we set to `0`. Additionally, we set all the fraction bits equal to `1`. Hence, the bit pattern will look like `0 1...10 1 1....1`, which will have the value `(−1)^s * m * 2^{e−bias} = 1 * (1 + (1-2^{-63})) * 2^{(2^{15} - 2)−16383} = (2-2^{-63}) * 2^{16383} = 2^{16384} - 2^{16320}`.

---


### 2.87 ⬥
The 2008 version of the IEEE floating-point standard, named IEEE 754-2008, includes a 16-bit "half-precision" floating-point format. It was originally devised  by computer graphics companies for storing data in which a higher dynamic range is required than can be achieved with 16-bit integers. This format has 2 sign bit, 5 exponent bits (`k =  5`), and `10` fraction bits (`n = 10`). The exponent bias is `2^{5-1} - 1 = 15`.

Fill in the table that follows for each of the numbers given, with the following instructions for each column:

*Hex*: The four hexadecimal digits describing the encoded form.

*M*: The value of the significand. This should be a number of the form `x` or `x/y`, where `x` is an integer and `y`is an integral power of `2`. Examples include `0`, `67/64`, and `1/256`.

*E*: The integer value of the exponent.

*V*: The numeric value represented. Use the notation `x` or `x * 2^z`, where `x` and `z` are integers.

*D*: The (possibly approximate) numerical value, as is printed using the `%f` formatting specification of `printf`.

As an example, to represent the number `7/8`, we would have `s = 0`, `M = 7/4`,  and `E =  -1`. Our number would therefore have an exponent field of `0b01110`  (decimal value `15 - 1 = 14`) and a significand field of `0b1100000000`, giving a hex representation of `0x3B00`. The  numerical value is `0.875`.

You need not fill in entries marks `-`.

| Description                             | Hex | M  | E  | V  | D  |
| :-------------------------------------- | :-- | :- | :- | :- | :- |
| `-0`                                    |     |    |    |    |    |
| Smallest value `>  2`                   |     |    |    |    |    |
| 512                                     |     |    |    |    |    |
| Largest denormalized                    |     |    |    |    |    |
| `-∞`                                    |     |    |    |    |    |
| Number with hex representation `0x3BB0` |     |    |    |    |    |

---

| Description                             | Hex      | M           | E     | V             | D                   |
| :-------------------------------------- | :--      | :---------- | :---- | :-------------| :------------------ |
| `-0`                                    | `0x8000` | `0`         | `-14` | `-0`          | `-0.0`              |
| Smallest value `>  2`                   | `0x4001` | `1025/1024` | `1`   | `1025/512`    | `2.001953125`       |
| 512                                     | `0x6000` | `1`         | `9`   | `512`         | `512.0`             |
| Largest denormalized                    | `0x03FF` | `1023/1024` | `-14` | `1023/2^{24}` | `0.000060975551605` |
| `-∞`                                    | `0xFC00` | -           | -   | `-∞`          | `-∞`                |
| Number with hex representation `0x3BB0` | `0x3BB0` | `123/64`    | `-1`  | `123/128`     | `0.9609375`         |  

---


### 2.88 ⬥⬥
Consider the following two 9-bit floating-point representations based on the IEEE floating-point format.

**1.**  Format A
* There is `1` sign bit.
* There are  `k = 5` exponent bits. The exponent bias is `15`.
* There are `n = 3` fraction bits.

**1.**  Format B
* There is `1` sign bit.
* There are  `k = 4` exponent bits. The exponent bias is `7`.
* There are `n = 4` fraction bits.

In the following table, you are given some bit patterns in format A, and your task is to convert them to the closest value in format B. If rounding is necessary, you should *round toward* `+∞`. In addition, give the values of numbers given by the format A and format B bit patterns. Give these as whole numbers (e.g., `17`) or as fractions (e.g., `17/64` or `17/2^6`).

| Format A Bits | Format A Value | Format B Bits | Format B Value |
| :------------ | :------------- | :------------ | :------------- |
| `1 01111 001` | `-9/8`         | `1 0111 0010` | `-9/8`         |
| `0 10110 011` | ``             | ``            | ``             |
| `1 00111 010` | ``             | ``            | ``             |
| `0 00000 111` | ``             | ``            | ``             |
| `1 11100 000` | ``             | ``            | ``             |
| `0 10111 100` | ``             | ``            | ``             |

---

| Format A Bits | Format A Value | Format B Bits | Format B Value |
| :------------ | :------------- | :------------ | :------------- |
| `1 01111 001` | `-9/8`         | `1 0111 0010` | `-9/8`         |
| `0 10110 011` | `176`          | `0 1110 0110` | `176`          |
| `1 00111 010` | `-5/1024`      | `1 0000 0111` | `-5/1024`      |
| `0 00000 111` | `7/131072`     | `0 0000 0001` | `1/1024`       |
| `1 11100 000` | `-8192`        | `1 1110 1111` | `-248`         |
| `0 10111 100` | `384`          | `0 1111 0000` | `+∞`           |


Let us consider the pattern `0 10110 011` in Format A. Observe that `S = 0`, `M = 0b1.011`, and `E = e - bias = 22 - 15 = 7`. Therefore, we have that the value equals `(-1)^S * M * 2^E = 1 * 0b1.011 * 2^7 = 176`. In Format B, if `S = 0`, `M = 0b1.0110`, and `E = e - bias = 14 - 7`, then we derive the same value`(-1)^S   * M * 2^E = 1 * 0b1.0110 * 2^7 = 176`.

In the bit pattern `1 00111 010`, we can see that `S = 1`, `M = 0b1.010`, and `E = e - bias = 7 - 15 = -8`. Hence, the value of this number is `(-1)^S * M * 2^E = -1 * 0b1.010 * 2^{-8} = -5/1024`. In Format B, if `S = 0`, `M = 0b0.0101`, and `E  = 1 - bias = 1 - 7 = -6`, we again  derive the same value `(-1)^S * M * 2^E  = -1 * 0b0.0101 * 2^{-6} = -5/1024`.

Now let us consider the bit pattern `0 00000 111`. It is clear that `S = 0`, `M = 0b0.111`, and `E = 1 - bias = -14`, and therefore the value is `(-1)^S * M * 2^E = 1 * 0.111 * 2^{-14} = 7/131702`. Notice that in Format B, the lowest exponent we can get is `1 - bias = 1 - 7 = -6`, for a denormalized value, which is not enough to accurately represent `7/131702`. Since we must round towards `+∞`, the closest value we can represent in Format B has `S = 0`, `M = 0b0001`, and `E  = 1 - bias = -6`. That it, the bit pattern `0 0000 0001`, which has value `(-1)^S * M * 2^E = 1 * 0b0001 *  2^{-6} = 1 /1024`.

The bit pattern `1 11100 000` has the characteristics `S = 1`, `M = 0b1.000`, and `E = e - bias = 28 - 15 = 13`, such that it has decimal value `(-1)^S * M * 2^E = -1 * 1 * 2^13 = -8192`. Once again, with only `4` exponent bits in Format B, the largest exponent we can get is `E = e - bias = 14 - 7 = 7`, from a normalized value. However, since the target value is negative and we round towards `+∞`, we can use the bit pattern such that `S = 1`, `M = 0b1.1111`, and `E = e - bias = 14 - 7 = 7`. This Format B representation has the value `(-1)^S * M * 2^E = -1 * 0b1.1111 * 2^{7} = -248`.

Lastly, let us consider the bit pattern `0 10111 100`. Observe that `S = 0`, `M =  0b1.100`, and `E = e - bias = 23 - 15 = 8`, which gives us a value of `(-1)^S * M * 2^E = 1 * 0b1.100 * 2^8 = 384`. From the previous bit pattern, disregarding the negative sign bit, the largest integer we were able to represent in  Format B was `248`. Since, `384 > 248` and we round up, then the closest rounded up value we can represent in Format B is `+∞`. Thus, the Format B bit pattern  will have all exponent bits equal `1`, fraction bits all equal `0`, and the sign bit equal `0`.

---

### 2.89 ⬥⬥
We are  running programs on a machine where values of type `int` have a 32-bit two's complement representation. Values of type `float` use the 32-bit IEEE format, and values of type `double` use the 64-bit IEEE format.

We generate arbitrary integer values `x`, `y`, and `z`, and convert them to values of type `double` as follows:
````C
/* Create some arbitrary values*/
int x = random();
int y = random();
int z = random();
/* Convert to double  */
double dx = (double) x;
double dy = (double) y;
double dz = (double) z;
````

For each of the following C  expressions, you are to indicate whether or not the expression *always* yields  `1`.If it always yields `1`, describe the underlying mathematical principles. Otherwise, give an example of arguments that make it yield `0`. Note that you cannot use an IA32 machine running  `GCC` to test your answers, since it would use the 80-bit extended-precision representation for both `float` and `double`.

**A.** `(float) x == (float) dx`

**B.** `dx - dy == (double) (x-y)`

**C.** `(dx + dy) + dz == dx + (dy + dz)`

**D.** `(dx * dy) * dz  == dx * (dy  * dz)`

**E.** `dx / dx == dz / dz`

---

**A.** `(float) x == (float) dx`

This expression will always yield `1`. Type `double` has a significant precision of `53` bits. Since type `int` only uses `32` bits, any integer can be represented as a `double`. Then, when both numbers get casted to type `float`, they will be represented as the same number. Thus, the expression will always evaluate to `1`.

**B.** `dx - dy == (double) (x-y)`

Consider the case when `y = INT_MIN` and `x = 0`. The expression `x - y = 0 - INT_MIN = INT_MIN`, but since type `double` has more significant precision, `dx - dy` will be equal to the absolute value of `INT_MIN`, `|INT_MIN|`. Hence, this expression does not always evaluate to `1`.

**C.** `(dx + dy) + dz == dx + (dy + dz)`

This expression will always evaluate to evaluate to `1`. Type `double` has a significant precision of `53` bits. Since type `int` only uses `32` bits, any combination of three `double` cast integers can be represented as type `double` with precision. Thus, the expression `(dx + dy) + dz == dx + (dy + dz)` will always yield `1`.

**D.** `(dx * dy) * dz == dx * (dy * dz)`

It is possible for this expression to evaluate to `0`. If `x = 1692873611`, `y = -753094038`, and `z = -768663586`, then the expression `(dx * dy) * dz = 979963843234205960952086528.000000`, but `dx * (dy * dz) = 979963843234206098391040000.000000` due to precision limitations in `double` multiplication arithmetic. Type `double` has a significant precision of `53` bits. An `int` uses `32` bits for storage, so every `int` can be represented with precision in a `double`. However, a `double` cannot always store the result of an operation, as shown in our example. Hence, it is clear that this expression does not always evaluate to `1`.

**E.** `dx / dx == dz / dz`

This expression does not always yield `1`. If `x  = 0`, then `dx / dx` will return `NaN`. However, if `z` does not equal `0`, then `dz / dz`  will return `1`. Thus, this expression can yield `0`.

---


### 2.90 ⬥
You have been assigned the task of writing a C function to compute a floating-point representation of `2^x`. You decide that the best way to do this is to directly construct the IEEE single-precision representation of the result. When `x` is too small, your routine will return `0.0`. When `x` is too large, it will return `+∞`. Fill in the blank portions of the code that follows to compute the correct result. Assume the function `u2f` returns a floating point value having an identical bit representation as its unsigned argument.


````C
float fpwr2(int x) {
    /* Result exponent and fraction*/
    unsigned exp, frac;
    unsigned u;

    if (x < ________) {
        /* Too small. Return 0.0*/
        exp = ________;
        frac = ________;
    } else if (x < ________) {
        /* Denormalized result*/
        exp = ________;
        frac = ________;
    } else if(x < ________){
        /* Normalized result*/
        exp = ________;
        frac = ________;
    } else {
        /* Too big. Return +oo*/
        exp = ________;
        frac = ________;
    }

    /* Pack exp and frac into 32 bits*/
    u = exp << 23 | frac;
    /* Return as float*/
    return u2f(u);
}
````

---

````C
float fpwr2(int x) {
  /* Result exponent and fraction*/
  unsigned exp, frac;
  unsigned u;

  int bias = pow(2,7) - 1;

  if (x < ((1-bias) - 23)) {
    /*  Too small. Return 0.0*/
    exp = 0;
    frac = 0;
  } else if (x < (1 - bias)) {
    /* Denormalized result*/
    exp = 0;
    frac = 1 << (unsigned) (x - ((1-bias) - 23));
  } else if(x < (bias + 1)){
    /* Normalized result*/
    exp = bias + x;
    frac = 0;
  } else {
    /* Too big. Return +oo*/
    exp = 0xFF;
    frac = 0;
  }

  /* Pack exp and frac into 32 bits*/
  u = exp << 23 | frac;
  /* Return as float*/
  return u2f(u);
}
````

Let us consider each of the cases and describe how these values were derived. First, note that the `bias` is calculated with the expression `pow(2,7) - 1` since `k =  8` bits are used for the exponent in single-precision floating-point representation. The first case, as stated in the comment, is for powers of `2` that are too small. The smallest exponent `E` we can get out of a floating-point number is in denormalized numbers. Therefore, the minimum is `E = 1 - bias`, which happens to be `E = 1 - bias = 1 - 127 = -126` for single-precision floating-points. However, using the fraction field, we can get an additional `2^{-23}` factor with only the least significant fraction field bit set to equal `1`, because `n = 23`. Hence, if `x < ((1-bias) - 23)` we can not represent `2^x` in a single-precision floating-point, since `2^{((1-bias) - 23)}` is the smallest power of `2` we can store. It is clear to see that to return `0.0`, we set `exp = 0` and `frac = 0`;

The next case is for denormalized values. Note that we only use denormalized values if the number is too small to be stored as a normalized. Therefore, our conditional should check if `x` is less than the smallest exponent a normalized value could represent. A normalized exponent takes the form `E = e - bias`, so the smallest value is `e - bias = 1 - bias`, since at least one bit must equal `1`. The exponent bits `exp` must equal `0` because this value is denormalized. For denormalized values `E = 1 - bias` and, as described above, we can use the `n` bits of the fraction field. Therefore, the fraction field should be set to `1 << (unsigned) (x - ((1-bias) - 23))`.

For normalized values, the exponent value is defined by `E = e - bias`. The value `e` can take on a maximum value of `(2^k - 1) - 1 = 2^8 - 2` for normalized values. Note that if `e = 2^k - 1`, then the exponent field is all ones and is considered a special case. Therefore, the largest exponent a normalized value can take on is `2^8 - 2 - (2^7 - 1) = 2^7 - 1 = bias`. Hence, `x` must be less than `bias +  1`, to be considered for the normalized case. It is easy to see that the fraction field is set to `0` and `exp = bias + x`, since `E = x = exp - bias` for normalized values.

Lastly, if the exponent is too big, we return `+∞`. This is the special case where all exponent bits are set to ones and all fraction bits are set to zero.

---


### 2.91 ⬥
Around 250 B.C., the Greek mathematician Archimedes proved that `223/71 < π  < 22/7`. Had he had access to a computer and the standard library `<math.h>`, he would have been able to determine that the single-precision floating-point approximation of `π` has the hexadecimal representation `0x40490FDB`. Of course, all of these are just approximations, since `π` is not rational.

**A.** What is the fractional binary number denoted by this floating-point value?

**B.** What is the fractional binary representation of `22/7`? *Hint*: See Problem 2.83.

**C.** At what bit position (relative to the binary point) do these two approximations to `π` diverge?

---

**A.** What is the fractional binary number denoted by this floating-point value?

It is easy to convert the hex value into binary as `0x40490FDB = 0b01000000010010010000111111011011`. Note that the exponent field `10000000` denotes a normalized value, and the exponent `E = e - bias = 2^7 - (2^7 - 1) = 1`. Therefore, the fraction binary number denoted by this floating-point value is `0b11.0010010000111111011011`.

**B.** What is the fractional binary representation of `22/7`? *Hint*: See Problem 2.83.

Note that `22/7 = 3 + 1/7`. Then, using the context of Problem 2.83 above, if `Y = 0b001` and `k = 3`, then the formula derived in Problem 2.83 will give us `0b001 / (2^3-1) = 1/7`. Thus, the fractional binary representation of `22/7` is `0b11.001001001...`.

**C.** At what bit position (relative to the binary point) do these two approximations to `π` diverge?

Comparing the fractional binary number representation of the hex value `0x40490FDB`, `0b11.0010010000111111011011`, and the fractional binary representation of `22/7`, `0b11.001001001...`, it is easy to see that the approximations diverge at the ninth bit.

---

### Bit-Level Floating-Point Coding rules

In the following problems, you will write code to implement floating-point functions, operating directly on bit-level representations of floating-point numbers. Your code should exactly replicate the conventions for IEEE floating-point operations, including using round-to-even mode when rounding is required.

To this end, we define data type `float_bits` to be equivalent to `unsigned`:
````C
/* Access bit-level representation floating-point number */
typedef unsigned float_bits;
````

Rather than using data type `float` in your code, you will use float_bits. You may use both `int` and `unsigned` data types, including unsigned and integer constants and operations. You may not use any unions, structs, or arrays. Most significantly, you may not use any floating-point data types, operations, or constants Instead, your code should perform the bit manipulations that implement the specified floating-point operations.

The following function illustrates the use of these coding rules. For argument `ƒ`, it returns `±0` if `ƒ` is denormalized (preserving the sign of `ƒ`), and returns `ƒ` otherwise.

````C
/* If f is denorm, return 0. Otherwise, return f */
float_bits float_denorm_zero(float_bits f) {
  /* Decompose bit representation into parts */
  unsigned sign = f>>31;
  unsigned exp = f>>23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  if(exp == 0) {
    /* Denormalized. Set fraction to 0 */
    frac = 0;
  }
  /* Reassemble bits */
  return (sign << 31)  | (exp << 23) | frac;
}
````

---


### 2.92 ⬥⬥
Following the bit-level floating-point coding rules, implement the function with the following prototype:

````C
/* Compute -f. If f is Nan, then return f. */
float_bits float_negate(float_bits f);
````

For floating-point number `ƒ`, this function computes `-ƒ`. If `ƒ` is `NaN`, your function should simply return `ƒ`.

Test your function by evaluating it for all `2^{32}` values of argument `f` and comparing results to what would be obtained using your machine's floating-point operations.

---

````C
float_bits float_negate(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = (f >> 23) & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  // Return f if f is NaN
  if((exp == 0xFF) && (frac != 0)) {
    return f;
  }

  return (~sign << 31) | (exp << 23) | frac;
}

void test_float_negate() {
  unsigned u;
  float f, f_neg, f_fb_neg;
  float_bits fb_neg;

  u = 0;
  do {
    f = *(float*) &u;
    f_neg = -f;

    fb_neg = float_negate(u);
    f_fb_neg = *(float*) &fb_neg;

    if(isnan(f)) {
      assert(fb_neg == u);
    } else {
      assert(f_fb_neg == f_neg);
    }

    u++;
  } while(u != 0);
}
````

For floating-point number `ƒ`, to compute `-ƒ`, we simply need to flip the sign bit. However, we must first check if `ƒ` is `NaN` and if so, return `ƒ` instead. In a single-precision floating-point value, the most significant bit corresponds to the sign bit, the next `8` significant bits correspond to the exponent field, and the remaining `23` bits correspond to the fraction field. Therefore, we can extract the sign bit with the expression `f >> 31`, the exponent field with `(f >> 23) & 0xFF`, and the fraction field with `f & 0x7FFFFF`. Now, to check if `ƒ` is `NaN`, we must check if the exponent field is all ones, `0xFF`, and the fraction field is all zeros. If it is, we return `ƒ`. Otherwise, we can flip the sign bit with `~sign` and return the expression `(~sign << 31) | (exp << 23) frac`.

---


### 2.93 ⬥⬥
Following the bit-level floating-point coding rules, implement the function with the following prototype:

````C
/* Compute |f|. If f is Nan, then return f. */
float_bits float_absval(float_bits f);
````

For floating-point number `ƒ`, this function computes `|ƒ|`. If `ƒ` is `NaN`, your function should simply return `ƒ`.

Test your function by evaluating it for all `2^{32}` values of argument `f` and comparing results to what would be obtained using your machine's floating-point operations.

---

````C
/* Compute |f|. If f is Nan, then return f. */
float_bits float_absval(float_bits f) {
  unsigned exp = (f >> 23) & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  // Return f if f is NaN
  if((exp == 0xFF) && (frac != 0)) {
    return f;
  }

  return (0 << 31) | (exp << 23) | frac;
}

void test_float_absval() {
  unsigned u;
  float f, f_abs, f_fb_abs;
  float_bits fb_abs;

  u = 0;
  do {
    f = *(float*) &u;
    f_abs = fabsf(f);

    fb_abs = float_absval(u);
    f_fb_abs = *(float*) &fb_abs;

    if(isnan(f)) {
      assert(fb_abs == u);
    } else {
      assert(f_fb_abs == f_abs);
    }

    u++;
  } while(u != 0);
}
````

For floating-point number `ƒ`, to compute `|ƒ|`, we simply need to zero the sign bit. However, we must first check if `ƒ` is `NaN` and if so, return `ƒ` instead. In a single-precision floating-point value, the most significant bit corresponds to the sign bit, the next `8` significant bits correspond to the exponent field, and the remaining `23` bits correspond to the fraction field. Therefore, we can extract the exponent field with `(f >> 23) & 0xFF` and the fraction field with `f & 0x7FFFFF`. Now, to check if `ƒ` is `NaN`, we must check if the exponent field is all ones, `0xFF`, and the fraction field is all zeros. If it is, we return `ƒ`. Otherwise, we can zero the sign bit with `(0 << 31)` and return the expression `(0 << 31) | (exp << 23) | frac`.

---


### 2.94 ⬥⬥⬥
Following the bit-level floating-point coding rules, implement the function with the following prototype:

````C
/* Compute 2*f. If f is Nan, then return f. */
float_bits float_twice(float_bits f);
````

For floating-point number `ƒ`, this function computes `2.0•ƒ`. If `ƒ` is `NaN`, your function should simply return `ƒ`.

Test your function by evaluating it for all `2^{32}` values of argument `f` and comparing results to what would be obtained using your machine's floating-point operations.

---

````C
/* Compute 2*f. If f is Nan, then return f. */
float_bits float_twice(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = (f >> 23) & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  // Return f if f is NaN or +/-oo
  if(exp == 0xFF) {
    return f;
  }

  if(exp == 0) {
    // Denormalized
    frac <<= 1;
  } else if(exp == (0xFF - 1)) {
    // Normalized to +/-oo
    exp = 0xFF;
    frac = 0;
  } else {
    // Normalized
    exp += 1;
  }

  return (sign << 31) | (exp << 23) | frac;
}

void test_float_twice() {
  unsigned u;
  float f, f_dub, f_fb_twice;
  float_bits fb_twice;

  u = 0;
  do {
    f = *(float*) &u;
    f_dub = f * 2.0;

    fb_twice = float_twice(u);
    f_fb_twice = *(float*) &fb_twice;

    if(isnan(f)) {
      assert(fb_twice == u);
    } else {
      assert(f_fb_twice == f_dub);
    }

    u++;
  } while(u != 0);
}
````

After testing if `ƒ` is `NaN`, we can check if `ƒ` is equal to `±∞` and return `ƒ`, since doubling infinity is still infinity. Once these special cases are considered, there are three cases `ƒ` can be in. Namely, `ƒ` can be denormalized, `2 • ƒ` could yield `±∞`, or `ƒ` could be normalized. If `ƒ` is normalized, that is the exponent field is all zeros, then `2 • ƒ` can be generated by left shifting the fraction field by `1`. We know that `±∞` is a special case when all exponent bits are ones and all fraction bits are zeros. Therefore, if the exponent bits of `ƒ` are equal to `0xFF - 1`, then adding `1` to the exponent field in the calculation of `2 • ƒ` will make the result `±∞`. If this is the case, then we set the exponent field to ones, the fraction field to zeros, and we preserve the sign bit. Lastly, if `ƒ` is a normalized value that will not double to infinity, we can simply add `1` to the exponent field.

---


### 2.95 ⬥⬥⬥
Following the bit-level floating-point coding rules, implement the function with the following prototype:

````C
/* Compute 0.5*f. If f is NaN, then return f. */
float_bits float_half(float_bits f);
````

For floating-point number `ƒ`, this function computes `0.5 • ƒ`. If `ƒ` is `NaN`, your function should simply return `ƒ`.

Test your function by evaluating it for all `2^{32}` values of argument `ƒ` and comparing the results to what would be obtained using your machine's floating-point operations.

---

````C
float_bits float_half(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = (f >> 23) & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  unsigned exp_frac = f & 0x7FFFFFFF;

  // Return f if f is NaN or +/-oo
  if(exp == 0xFF) {
    return f;
  }

  // Check for round to even
  int round = (frac & 0x3) == 0x3;

  if(exp == 0) {
    // Denormalized
    frac >>= 1;
    frac += round;
  } else if(exp == 1) {
    // Normalized to denormalized depending on frac
    exp_frac >>= 1;
    exp_frac += round;
    exp = (exp_frac >> 23) & 0xFF;
    frac = exp_frac & 0x7FFFFF;
  } else {
    // Normalized
    exp -= 1;
  }

  return (sign << 31) | (exp << 23) | frac;
}

void test_float_half() {
  unsigned u;
  float f, f_half, f_fb_half;
  float_bits fb_half;

  u = 0;
  do {
    f = *(float*) &u;
    f_half = 0.5*f;

    fb_half = float_half(u);
    f_fb_half = *(float*) &fb_half;

    if(isnan(f)) {
      assert(fb_half == u);
    } else {
      assert(f_fb_half == f_half);
    }

    u++;
  } while(u != 0);
}
````

We can easily test if `ƒ` is `NaN` or `±∞` by checking if `exp == 0xFF` and return `ƒ` if true. Note that there are two ways to perform the halving calculation. We can either right shift the fraction field or we can decrement the exponent field. If we right shift the fraction field, the least significant bit could be lost, in which case we would need to round towards even. Consider the four combinations of the two least significant bits of the fraction field after a right shift. Clearly, `0b00 >> 1 = 0b0`, `0b01 >> 1 = 0b0`, `0b10 >> 1 = 0b1`, and `0b11 >> 1 = 0b1`. Only when the values `0b01` and `0b11` get right shifted do we lose a `1` bit, forcing rounding. However, `0b01 >> 1 = 0b0` already rounds to even, so we only need to add `1` for rounding when the two least significant bits of the fraction field are `0b11`.

There are three cases to consider. Namely, if `exp == 0`, indicating a denormalized number, if `exp == 1`, indicating that a normalized number may be turned into a denormalized number, and lastly when dealing with a normalized number that will be halved to a normalized number. If `exp == 0`, it is clear we are dealing with a denormalized number. In this case, we simply right shift the fraction field to half its value and add `1` to round to even, if necessary. If `exp == 1`, then we are dealing with a normalized value, but decrementing the decrementing the exponent field would cause the value to become denormalized. Therefore, we must see if we can half the value by right shifting the fraction field instead. Since `exp == 1`, then right shifting `exp_frac` will be equivalent to right shifting the fraction field with the implied leading `1`. Then, since we may have lost a bit, we must round to even, if necessary. Lastly, if `ƒ` is normalized and we can represent `0.5 • ƒ` as a normalized value, then we only need to decrement the exponent field.


---


### 2.96 ⬥⬥⬥⬥
Following the bit-level floating-point coding rules, implement the function with the following prototype:

````C
/*
 * Compute (int) f.
 * If conversion causes overflow or f is Nan, return 0x80000000
 */
int float_f2i(float_bits f);
````

for floating-point number `ƒ`, this function computes `(int) f`. Your function should round toward zero. If `ƒ` cannot be represented as an integer (e.g., it is out of range, or it is `NaN`), then the function should return `0x80000000`.

Test out your function by evaluating it for all `2^{32}` values of argument `ƒ` and comparing the result to what would be obtained using your machine's floating-point operations.

---

````C
int float_f2i(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = (f >> 23) & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  int val, E, M;
  unsigned bias = 0x7F;

  // Return 0x80000000 if f is NaN or +/-oo
  if(exp == 0xFF) {
    return 0x80000000;
  }

  if(exp < bias) {
    // |f| less than 1
    val = 0;
  } else if(exp >= (31 + bias)) {
    // Overflow
    val = 0x80000000;
  } else {
    // Compute exponent
    E = exp - bias;

    // Add implied leading 1
    M = frac | 0x800000;

    // Compensate for fraction field representation
    if(E > 23) {
      val = M << (E - 23);
    } else {
      val = M >> (23 - E);
    }
  }

  // Set sign
  if(sign) {
    return -val;
  } else {
    return val;
  }
}

void test_float_f2i() {
  unsigned u;
  float f;
  int i_f, i_f2i;

  u = 0;
  do {
    f = *(float*) &u;
    i_f = (int) f;

    i_f2i = float_f2i(u);;

    assert(i_f2i == i_f);

    u++;
  } while(u != 0);
}
````

We can easily check for the special cases when `ƒ` is `NaN` or `±∞` by checking if the exponent field is all ones. If so, we return `0x80000000` as is standardized. We must consider the cases where rounding `ƒ` towards zero will equal `0`, where `ƒ` is too large to be represented as type `int`, and where rounding `ƒ` towards zero can be represented as type `int`. The first case, that is when `|ƒ| < 1`, can be flagged when the exponent is less than the bias. In this case, the resulting integer will be `0`. In the second case, the value of `ƒ` rounded towards zero to the nearest integer cannot be represented in type `int`. Under these conditions, we return `0x80000000` as we would when `ƒ = ±∞`. In the last case, we know that we can represented the integer value that `ƒ` rounds towards zero to. We first compute exponent value and the significand with implied leading `1`. Next, we must compensate for the fraction field representation, since this will be equivalent to already left shifting the significand by `23`. Lastly, we set the sign accordingly and return.

---


### 2.97 ⬥⬥⬥⬥
Following the bit-level floating-point coding rules, implement the function with the following prototype:

````C
/* Compute (float) i */
float_bits float_i2f(int i);
````

For argument `i`, this function computes the bit-level representation of `(float) i`.

Test your function by evaluating it for all `2^{32}` values of argument `f` and comparing the results to what would be obtained using your machine's floating-point operations.

---

````C
// Calculate number of bits needed to represent i
int num_bits(int i) {
  unsigned u = i;

  // Logical right shift u until no 1 bits left
  int length = 0;
  while (u > 0) {
    u >>= 1;
    length++;
  }
  return length;
}

float_bits float_i2f(int i) {
  unsigned sign, exp, frac, exp_frac;
  unsigned bits, frac_bits, i_frac_bits, bits_lost, bit_to_round;

  unsigned bias = 0x7F;

  // Special case i=0
  if (i == 0) {
    return 0;
  }

  // Set signn bit appropriately then make i positive
  sign = 0;
  if (i < 0) {
      sign = 1;
      i = -i;
  }

  // Get number of bits needed to represent i
  bits = num_bits(i);
  frac_bits = bits - 1;

  // Set exp value
  exp = bias + frac_bits;

  // Get bit representing fraction field from i
  i_frac_bits = i & ((unsigned) -1 >> (32 - frac_bits));

  // Check for fraction field overflow
  if(frac_bits <= 23) {
    frac = i_frac_bits << (23 - frac_bits);
  } else {
    int num_bits_over = frac_bits - 23;
    unsigned bit_to_round = 1 << (num_bits_over - 1);

    bits_lost = i_frac_bits & ((unsigned) -1 >> (32 - num_bits_over));
    frac = i_frac_bits >> num_bits_over;
    exp_frac = (exp << 23) | frac;

    // Check for rounding
    if(bits_lost > bit_to_round) {
      exp_frac += 1;
    } else if(bits_lost == bit_to_round) {
      // Round to even
      if (frac & 0x1) {
        exp_frac += 1;
      }
    }

    // Split exp and frac
    exp = (exp_frac >> 23) & 0xFF;
    frac = exp_frac & 0x7FFFFF;
  }

  return (sign << 31) | (exp << 23) | frac;
}

void test_float_i2f() {
  unsigned u;
  float f, f_fb_i;
  float_bits fb_i;

  u = 0;
  do {
    f = (float) (int) u;

    fb_i = float_i2f(u);
    f_fb_i = *(float*) &fb_i;

    assert(f_fb_i == f);

    u++;
  } while(u != 0);
}
````

In the function `float_i2f`, we make calls to the function `num_bits(int i)` which returns the number of bits used to represent the argument `i`. That is, we return `k` where the `k`th bit of `i` is the leftmost one bit in `i`.

To start `float_i2f`, we first check for the special case where `i == 0`. If this is the case, it is clear that `float_bits` representation will also be `0`. Next, we set the sign bit and, if negative, make `i` positive. We do this because we are only concerned with the absolute value of `i` in the `float_bits` representation because the sign bit will handle its positivity. We make a call to `num_bits` to get the number of bits needed to represent `i`. Since, the fraction field in `float_bits` has a leading `1` for normalized values, the number of bits required to represent `i` is one less than the number needed by type `int`.

Note that it is possible for the integer to be too large to be stored as type `float_bits`. This occurs when the number of bits required to represent the number as a `float_bits` is more than `23`, the number of fraction field bits. If the number can be represented, we simply set the fraction field with the expression `i_frac_bits << (23 - frac_bits)`. Otherwise, some bits will be lost and rounding to even may be necessary.

Let us consider the case where we cannot represent the integer as a `float_bits`. We can easily calculate how many bits will be lost in the `float_bits` representation and which bit will be the least significant fraction bit in the representation to be rounded. Then we must check if rounding is required and increment the fraction field accordingly. Lastly, we reconstruct the `float_bits` representation from the three fields and return.

----
