; ModuleID = '2007-04-10-BitfieldTest-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.u = type { i64 }
%struct.anon = type { [8 x i8] }

@.str = private unnamed_addr constant [12 x i8] c"p = 0x%02X\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %uu = alloca %union.u, align 8
  %token = bitcast %union.u* %uu to i64*
  store i64 82351535429844991, i64* %token, align 8
  %s = bitcast %union.u* %uu to %struct.anon*
  %0 = bitcast %struct.anon* %s to i64*
  %bf.load = load i64* %0, align 4
  %bf.lshr = lshr i64 %bf.load, 48
  %bf.clear = and i64 %bf.lshr, 255
  %bf.cast = trunc i64 %bf.clear to i32
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), i32 %bf.cast)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
