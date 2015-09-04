samples/neon/README.txt

 this directory has some sample code for ARM Neon

loadstore.c - load and store vectors
neonintrinsics.txt - short descriptions of intrinsics used in examples
neon_table.c - look at the neon table vtbl1_u8 intrinsic for byte permutation
neon_widen.c - turning an unsigned char vector into a floating point vector
README.txt - this file 
temperatures_neon.c - convert Celsius to Fahrenheit as floating point
     math, fixed point math, and using a multiply-accumulate
     instruction that combines the add and multiply into one step.


  Each of these files has a listing of the expected results in it. If
your results are different, check first to make sure I didn't make a
mistake, then check for a compiler bug.

