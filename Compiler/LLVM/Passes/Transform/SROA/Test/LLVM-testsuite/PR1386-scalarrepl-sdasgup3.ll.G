; ModuleID = 'PR1386-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.X = type <{ [16 x i8] }>

@.str = private unnamed_addr constant [16 x i8] c"bad bits: %llx\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %x = alloca %struct.X, align 1
  %0 = bitcast %struct.X* %x to i128*
  %bf.load = load i128* %0, align 1
  %bf.clear = and i128 %bf.load, -16
  %bf.set = or i128 %bf.clear, 15
  store i128 %bf.set, i128* %0, align 1
  %1 = bitcast %struct.X* %x to i128*
  %bf.load1 = load i128* %1, align 1
  %bf.clear2 = and i128 %bf.load1, -295147905179352825841
  %bf.set3 = or i128 %bf.clear2, 295147905179352825840
  store i128 %bf.set3, i128* %1, align 1
  %2 = bitcast %struct.X* %x to i128*
  %bf.load4 = load i128* %2, align 1
  %bf.clear5 = and i128 %bf.load4, 295147905179352825855
  %bf.set6 = or i128 %bf.clear5, -295147905179352825856
  store i128 %bf.set6, i128* %2, align 1
  %3 = bitcast %struct.X* %x to i64*
  %add.ptr = getelementptr inbounds i64* %3, i64 1
  %4 = load i64* %add.ptr, align 8
  %xor = xor i64 -1, %4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([16 x i8]* @.str, i32 0, i32 0), i64 %xor)
  %cmp = icmp ne i64 %xor, 0
  %conv = zext i1 %cmp to i32
  ret i32 %conv
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
