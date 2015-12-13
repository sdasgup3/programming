
How to compile
---------------
1. Download gcc 4.7.2. C
2. Edit gcc/tree.h. Add a member  variable 'unsigned index: 8; ' in struct tree_base
3. Configure and make it
2. run sourceme
3. make all
4. ./run.py tcases/test1.c
