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

The `show_bytes` program was run on a 64 bit Ubuntu (Linux) and a 64 bit macOS machine. As can be observed in the output from the program calls, both machines are little-endian, since the bytes representing the integer argument `12345`, `0x3039` in hex, is stored  as `39 30 00 00` by the machines.

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
  unsigned int value =  1;
  char * pointer  = (char*) &value;
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
replace_byte(0x12345678, 2, 0xAB) --> 0x12AB3456
replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB
````

---

````C
unsigned replace_byte(unsigned x, int i, unsigned char b) {
  // if index is negative, return x
  if(i <  0) {
    return x;
  }

  // if index is greater than the size of unsigned, return x
  if(i >  sizeof(unsigned)) {
    return x;
  }

  unsigned mask = ((unsigned) 0xFF) << (i << 3);
  unsigned i_byte = ((unsigned) b) << (i << 3);
  return (x & ~mask) | i_byte;
}
````

The procedure `replace_byte` above works as follows. First, we check that argument `i` is within the range of the size of an `unsigned` variable.  If `i` is not a valid index, we return `x` unchanged. Otherwise, we create a mask for byte `i`. Note that `(i << 3)` yields the bit index from the byte index `i`, since a left shift of `3` is equivalent to multiplying by `8` and there  are `8` bits in a byte. Hence, the expression `((unsigned) 0xFF) << (i << 3)` positions the `0xFF` value to the `i`th byte position. For example, if `i = 2`, then the expression `((unsigned) 0xFF) << (i << 3)` yields  `0x00FF0000`. After creating the mask, we position the value of character `b` to the `i`th byte in a similar manner. For example, if `b=0xAB`, then the expression `((unsigned) b) << (i << 3)` yields `0x00AB0000`. Lastly we return the expression `(x & ~mask) | i_byte`. In this expression, we first perform the bitwise AND between `x` and `~mask` which effectively zeros out the `i`th byte of `x`. Then we perform the bitwise OR between `x & ~mask` and `i_byte`, which returns an unsigned value in which byte `i` of argument `x` is replaced by byte `b`, as required. For example if `x = 0x12345678`, `i=2`, and `b=0xAB`, then it is clear to see that `(x & ~mask) | i_byte  ≡  (0x12345678 & ~0x00FF0000) | 0x00AB0000  ≡  (0x12345678 & 0xFF00FFFF) | 0x00AB0000  ≡ 0x12005678 | 0x00AB0000  ≡  0x12AB5678`.

---


### 2.61 ⬥⬥
Write C expressions that evaluate to `1` when the following conditions are true and to `0` when they are false. Assume `x` is of type `int`.

* Any bit of `x` equals 1.
* Any bit of `x` equals 0.
* Any bit in the least significant byte of `x` equals `1`.
* Any bit in the most significant byte of `x` equals `0`.

Your code should follow the bit-level integer coding rules (page 128), with the additional restriction that you may not use equality (`==`) or inequality (`!=`) tests.

---

* Any bit of `x` equals 1.
````C
!!x
````
If any bit in `x` equals `1`, then `!x` will evaluate to `0` and `!!x` will evaluate to `1`. If no bit in `x` equals `1`, that is, every bit in `x` equals `0`,  then `!x` will evaluate to `1` and `!!x` will evaluate to `0`.

* Any bit of `x` equals 0.
````C
!!~x  
````
If any bit in `x` equals `0`, then at least one bit in `~x` will be `1`. Therefore, `!~x` will evaluate to `0` and `!!~x` will evaluate to `1`. If no bit in `x` equals `0`, that is, every bit in `x` equals `1`, then every bit in `~x` will equal `0`. Therefore, `!~x` will evaluate `1` and `!!~x` will evaluate `0`.

* Any bit in the least significant byte of `x` equals `1`.
````C
!!(x & 0xFF)
````
If any bit in the least significant byte of `x` equals `1`, then at least one bit `(x & 0xFF)` will equal 1. Therefore, `!(x & 0xFF)` will evaluate to `0`  and `!!(x & 0xFF)` will evaluate to `1`. If no bit in the least significant byte of `x` equals `1`, that is, every bit in the least significant byte of `x` equals `0`, then no bit in `(x & 0xFF)` will equal 1. Therefore, `!(x & 0xFF)` will evaluate to `1` and `!!(x & 0xFF)` will evaluate to `0`.

* Any bit in the most significant byte of `x` equals `0`.
````C
!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)
````
As described in the `Bit-Level Integer Coding Rules` section immediately preceeding this problem in the text, the expression `((x >> ((sizeof(int) -  1) << 3)) & 0xFF)` will extract the most significant byte of `x` and position it in the least significant byte, with all other bytes being `0x00`. Hence, the expression `!!((~x >> ((sizeof(int) -1 ) << 3)) & 0xFF)` will yield the same result except the bits in the least significant byte will all be flipped, due to `~x`.
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

Our function `int_shifts_are_arithmetic()` first initializes the variable `test` with the value `-1`. Therefore, all bits in `test` will be set to `1`. If a machine uses arithmetic right shifts for data type `int`, then the expression `(test >> 1)` will also be represented will all bits set to `1`. Therfore the bitwise XOR expression `(test ^ (test >> 1))` will result in all bits set to `0` and `!(test ^ (test >> 1))` will evaluate to `1`. If a machine uses a logical right shift for data type `int`, then the expression `(test >> 1)` will result in all bits  set to `1` except the most significant bit will be set to `0`. Therfore the bitwise XOR expression `(test ^ (test >> 1))` will result in all bits set to `0` except the most significant bit will be set to `1` and `!(test ^ (test >> 1))` will evaluate to `0`.

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

The function `srl` performs a logical right shift using arithmetic right shifts as follows. First, we perform an arithmetic right shift with the statement `unsigned  xsra = (int) x >> k`. Then the expression `int mask = (int) -1 << (w - k)` will yield a mask with only the `k` most significant bits equal to `1`. Finally, we perform the bitwise NOT operation on the mask so that only the `k` most significant bits in the mask are `0`. Therefore, the expression `xsra & ~mask` turns the `k` most significant bits in `xrsa` to `0`, as would be the case in a logical right shift.

The function `sra` performs an arithmetic right shift using logical right shifts as follows. First, we perform an arithmetic right shift with the statement `unsigned  xsrl = (int) x >> k`. Then the expression `int mask = (int) -1 << (w - k)` will yield a mask with only the `k` most significant bits equal to `1`. We must create another mask for just the most significant bit with the expression `int top_mask = (unsigned) 1 << (w - 1)`. Now we must test if the most significant bit of `x` is a `1` or `0`. If the most significant bit of `x` is a `1`, then the expression `!(x & top_mask) - 1` will evaluate to `-1`. Therefore, `mask` remains unchanged by the full expression `mask &= !(x & top_mask) - 1`, since `-1` is represented by all bits equaling `1`. Then the expression `xsrl | mask` sets the `k` most significant bits of `xsrl` to `1` as it would be in an arithmetic shift. If the most significant bit of `x` is a `0`, then the expression `!(x & top_mask) - 1` will evaluate to `0`. Therefore, all bits in `mask` are cleared to `0` by the full expression `mask &= !(x & top_mask) - 1`. Then the expression `xsrl | mask` will just yield `xsrl` as would be calculated in an arithmetic shift.

---


### 2.64 ⬥
Write code to implement the following function:
````C
// Return 1 when any odd bit of x equals 1; 0 otherwise.
// Assume w=32
int any_odd_one(unsigned x) {

}
````
Your function should follow the  bit-level integer coding rules (page  128), except that you may assume that data type `int` has `ω = 32` bits.

---

````C
int any_odd_one(unsigned x) {
  return !!(x & 0xAAAAAAAA);
}
````

The function `any_odd_one` works as follows. Let us assume that `x` has at least one odd bit that equals `1`. Then the expression `(x & 0xAAAAAAAA)` will result in at least one bit being equal to `1`, since `0xA = 0b1010`. Note that the least significant bit is considered the `0`th bit so all the `1` bits in `0b1010` are odd bits. Then the expression `!(x & 0xAAAAAAAA)` will evaluate to `0`, since we have argued that at least one bit will be `1` in `(x & 0xAAAAAAAA)`. Therefore, `!!(x & 0xAAAAAAAA)` will evaluate to `1` as required. Now let us assume that `x` has no odd bits equal to `1`. Then the expression `(x & 0xAAAAAAAA)` will result in no bits equal to `1`. Hence, the expression `!(x & 0xAAAAAAAA)` will evaluate to `1` and `!!(x & 0xAAAAAAAA)` will evaluate to `0`.

----
