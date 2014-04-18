; ModuleID = '2003-05-23-TransparentUnion-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.UNION = type { float* }

@.str = private unnamed_addr constant [3 x i8] c"ok\00", align 1

; Function Attrs: nounwind uwtable
define i32 @try(float* %U.coerce) #0 {
entry:
  %U = alloca %union.UNION, align 8
  ret i32 1
}

; Function Attrs: nounwind uwtable
define i32 @test() #0 {
entry:
  %I = alloca i32, align 4
  %F = alloca float, align 4
  %agg.tmp = alloca %union.UNION, align 8
  %agg.tmp1 = alloca %union.UNION, align 8
  %__iptr = bitcast %union.UNION* %agg.tmp to i32**
  store i32* %I, i32** %__iptr, align 8
  %coerce.dive = getelementptr %union.UNION* %agg.tmp, i32 0, i32 0
  %0 = load float** %coerce.dive
  %call = call i32 @try(float* %0)
  %__fptr = bitcast %union.UNION* %agg.tmp1 to float**
  store float* %F, float** %__fptr, align 8
  %coerce.dive2 = getelementptr %union.UNION* %agg.tmp1, i32 0, i32 0
  %1 = load float** %coerce.dive2
  %call3 = call i32 @try(float* %1)
  %or = or i32 %call, %call3
  ret i32 %or
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %call = call i32 @test()
  %tobool = icmp ne i32 %call, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
