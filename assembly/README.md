### high level assembly syntax
-  AT&T syntax (used by as,gas) & Intel (use by nasm,tasm, masm)
- [Good read](http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm)

### x86/x86_64 Syntax
- push arg
  - SP -- ; MEM[SP] = arg 
- pop arg
  - arg = MEM[SP]; SP++ 
