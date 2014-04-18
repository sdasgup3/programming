; ModuleID = 'xT_12-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.stud_Marks = type { [30 x i32] }
%struct.stud_Res = type { [10 x i32], %struct.stud_Marks }

@.str = private unnamed_addr constant [19 x i8] c"\0ASubject Code : %d\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %0 = alloca [30 x i32]
  %1 = alloca %struct.stud_Marks
  %2 = alloca [10 x i32]
  %result = alloca %struct.stud_Res, align 4
  %arrayidx = getelementptr inbounds [30 x i32]* %0, i32 0, i64 0
  %3 = load i32* %arrayidx, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([19 x i8]* @.str, i32 0, i32 0), i32 %3)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
