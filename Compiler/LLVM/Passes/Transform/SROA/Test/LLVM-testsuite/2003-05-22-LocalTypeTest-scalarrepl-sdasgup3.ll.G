; ModuleID = '2003-05-22-LocalTypeTest-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.sometimes.1 = type { i16, i16, i16, i16 }
%struct.sometimes = type { i32, i32 }
%struct.sometimes.0 = type { i8 }

@Y = common global %struct.sometimes.1 zeroinitializer, align 2
@.str = private unnamed_addr constant [14 x i8] c"Result is %d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %S = alloca %struct.sometimes, align 4
  %S3 = alloca %struct.sometimes.0, align 1
  %conv = sext i8 -1 to i32
  %add = add nsw i32 1, %conv
  %0 = load i16* getelementptr inbounds (%struct.sometimes.1* @Y, i32 0, i32 0), align 2
  %conv6 = sext i16 %0 to i32
  %add7 = add nsw i32 %add, %conv6
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str, i32 0, i32 0), i32 %add7)
  ret i32 %add7
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
