; ModuleID = 'xT_3-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.fruit = type { i32, i32 }

@.str = private unnamed_addr constant [6 x i8] c"%d %d\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %f = alloca %struct.fruit, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %.01 = phi i32 [ undef, %entry ], [ %add, %for.inc ]
  %.0 = phi i32 [ undef, %entry ], [ %add2, %for.inc ]
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %mul = mul nsw i32 2, %i.0
  %add = add nsw i32 %.01, %mul
  %mul1 = mul nsw i32 5, %i.0
  %add2 = add nsw i32 %.0, %mul1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str, i32 0, i32 0), i32 %.01, i32 %.0)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
