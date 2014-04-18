; ModuleID = 'isX_10-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Zero = type { i32 }
%struct.One = type { i32, %struct.Zero }

@.str = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %0 = alloca i32
  %1 = alloca %struct.Zero
  %2 = alloca i32
  %i = alloca %struct.One, align 4
  %3 = load i32* %2, align 4
  %4 = load i32* %0, align 4
  %mul = mul nsw i32 %3, %4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), i32 %mul)
  %cmp = icmp eq i32* null, %0
  %conv = zext i1 %cmp to i32
  %call4 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), i32 %conv)
  %cmp6 = icmp eq i32* null, %2
  %conv7 = zext i1 %cmp6 to i32
  %call8 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), i32 %conv7)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
