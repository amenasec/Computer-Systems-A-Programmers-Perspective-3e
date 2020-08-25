/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // x ^ y = (x & ~y) | (~x & y)
  //       = ~(~(x & ~y) & ~(~x & y)) by De Morgan's Law
  return ~(~(x & ~y) & ~(~x & y));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // Two's complement minimum integer is 0b10...0
  return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // 1 << 31 = 0b10...0
  // ~(1 << 31) = 0b01...1
  // x ^ ~(1 << 31) = 0 if and only if x = 0b01...1
  return !(x ^ ~(1 << 31));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // 0xAAAAAAAA = 1010...1010
  // x & 0xAAAAAAAA zeros all even bits
  // x & 0xAAAAAAAA) ^ 0xAAAAAAAA returns true if an odd bit in x equals 0
  return !((x & 0xAAAAAAAA) ^ 0xAAAAAAAA);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // See Web Aside DATA:TNEG on page 96 of the text
  return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // A negative two's complement number's most significant bit is 1
  int neg_mask = 1 << 31;

  // Check if x < 0x30 with x - 0x30 < 0
  int lt_0x30 = (x + ~0x30 + 1) & neg_mask;

  // Check if x > 0x39 with 0x39 - x < 0
  int gt_0x39 = (0x39 + ~x + 1) & neg_mask;

  return !lt_0x30 & !gt_0x39;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // If x = 0, then !x = 1, ~(!x) = 0b1...10, and ~(!x) + 1 = 0b1...1
  // If x != 0, then !x = 0, ~(!x) = 0b1...1, and ~(!x) + 1 = 0b0...0
  int mask = ~(!x) + 1;

  // If x = 0, mask = 0b1...1, so (~mask & y) = 0b0...0 and (mask & z) = z
  // If x != 0, mask = 0b0...0, so (~mask & y) = y and (mask & z) = 0b0...0
  return (~mask & y) | (mask & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // A negative two's complement number's most significant bit is 1
  int neg_mask = 1 << 31;

  // Check if x < 0
  int x_neg_flag = (x >> 31) & 1;
  // Check if y < 0
  int y_neg_flag = (y >> 31) & 1;

  // Check if y < x with y - x < 0
  int y_lt_x = (y + ~x + 1) & neg_mask;

  // x <= y when
  //  1. (x < 0) && (y >= 0)
  //  2. (y - x < 0) && (x * y >= 0)
  //     If x > 0 and y < 0, underflow can occur with y - x
  //     Need additioal check that x and y have same sign.
  //     Otherwise, (x >= 0) && (y < 0), which is false
  return (x_neg_flag & !y_neg_flag) || (!y_lt_x & !(x_neg_flag ^ y_neg_flag));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  // A negative two's complement number's most significant bit is 1
  int neg_mask = 1 << 31;

  // Maximum two's complement number is 0b01...1
  int T_MAX = ~neg_mask;

  // Check if x < 0
  int neg_flag = (x >> 31) & 1;

  // If x > 0, x + T_MAX < 0
  int pos_flag = ((x + T_MAX) >> 31) & 1;

  // 
  return (neg_flag ^ 1) & (pos_flag ^ 1);
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  // If x < 0, sign extension will cause neg_mask = 0b1...1
  // If x >= 0, sign extension will cause neg_mask = 0b0...0
  int neg_mask = x >> 31;

  // If x >= 0, x is unchanged
  // If x < 0, x is bit-wise negated
  x = (neg_mask & ~x) | (~neg_mask & x);

  // If there is a 0b1 bit in the 16 most significant bits, (!!(x >> 16)) = 1
  //   Otherwise, (!!(x >> 16)) = 0.
  int ms16b = (!!(x >> 16)) << 4;
  // Right shift x by 16 if at least 16 bits needed, otherwise right shift by 0
  x = x >> ms16b;

  // If there is a 0b1 bit in the next 8 most significant bits, 
  //   (!!(x >> 8)) = 1. Otherwise, (!!(x >> 8)) = 0.
  int ms8b = (!!(x >> 8)) << 3;
  // Right shift x by 8 if at least 8 bits needed, otherwise right shift by 0
  x = x >> ms8b;

  // If there is a 0b1 bit in the next 4 most significant bits, 
  //   (!!(x >> 4)) = 1. Otherwise, (!!(x >> 4)) = 0.
  int ms4b = (!!(x >> 4)) << 2;
  // Right shift x by 4 if at least 4 bits needed, otherwise right shift by 0
  x = x >> ms4b;

  // If there is a 0b1 bit in the next 2 most significant bits, 
  //   (!!(x >> 2)) = 1. Otherwise, (!!(x >> 2)) = 0.
  int ms2b = (!!(x >> 2)) << 1;
  // Right shift x by 2 if at least 2 bits needed, otherwise right shift by 0
  x = x >> ms2b;

  // If there is a 0b1 bit in the next most significant bits, 
  //   (!!(x >> 1)) = 1. Otherwise, (!!(x >> 2)) = 0.
  int ms1b = (!!(x >> 1));
  // Right shift x by 1 if at least 1 bit needed, otherwise right shift by 0
  x = x >> ms1b;

  // Add and return the number of bits used to store x
  // Must add remaining x, for last remaining bit
  // Must add 1 for the sign bit
  return ms16b + ms8b + ms4b + ms2b + ms1b + x + 1; 
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  // Get exponent bits from uf
  int exp = (uf >> 23) & 0xFF;

  // Return uf for special values
  if(exp == 0xFF) {
    return uf;
  }

  // Left shift fraction field and maintain sign for denormalized
  if(exp == 0) {
    return ((uf & 0x7FFFFF) << 1) | (uf & (1 << 31));
  }

  // Add 1 to exponent field for normalized
  return uf + (1 << 23);
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  // Two's complement minimum integer is 0b10...0
  int T_MIN = 1 << 31;

  // Get exponent annd fraction values from uf
  int exp = ((uf >> 23) & 0xFF) - 127;
  int frac = (uf & 0x7FFFFF) | 0x800000;

  // Return 0x80000000 if out of range
  if(exp > 31) {
    return T_MIN;
  }

  // Return 0 if exp < 0
  if(exp < 0) {
    return 0;
  }

  // Calculate integer value i
  int i;
  if(exp > 23) {
    // If exp > 23, decimal value needs to be left shifting
    i = frac << (exp - 23);
  }
  if(exp <= 23) {
    // If exp <= 23, decimal value needs to be right shifted
    i = frac >> (23 - exp);
  }

  // Return -i if uf negative
  if(uf & T_MIN) {
    return -i;
  }
  return i;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  // Add bias to get the exponent value
  int exp = x + 127;

  // Return 0 if result is too small
  if(exp <= 0) {
    return 0;
  }

  // Return +INF if result is too large
  if(exp >= 0xFF) {
    return 0x7F800000;
  }

  // Return left shifted exponent bits
  return exp << 23;
}
