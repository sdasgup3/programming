; ModuleID = 'xT_9_2-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.student_detail = type { i32, [20 x i8], float, %struct.student_college_detail }
%struct.student_college_detail = type { i32, [50 x i8] }

@.str = private unnamed_addr constant [13 x i8] c" Id is: %d \0A\00", align 1
@.str1 = private unnamed_addr constant [22 x i8] c" Percentage is: %f \0A\0A\00", align 1
@.str2 = private unnamed_addr constant [21 x i8] c" College Id is: %d \0A\00", align 1
@stu_data = common global %struct.student_detail zeroinitializer, align 4

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %0 = alloca [50 x i8]
  %1 = alloca %struct.student_college_detail
  %2 = alloca [20 x i8]
  %stu_data = alloca %struct.student_detail, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([13 x i8]* @.str, i32 0, i32 0), i32 undef)
  %conv = fpext float undef to double
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @.str1, i32 0, i32 0), double %conv)
  %call2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([21 x i8]* @.str2, i32 0, i32 0), i32 undef)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
