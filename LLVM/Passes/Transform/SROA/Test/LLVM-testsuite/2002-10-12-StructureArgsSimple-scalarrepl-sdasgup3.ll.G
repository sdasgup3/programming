; ModuleID = '2002-10-12-StructureArgsSimple-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.vec2 = type { double, double }

@.str = private unnamed_addr constant [8 x i8] c"%f, %f\0A\00", align 1

; Function Attrs: nounwind uwtable
define void @print(double %S.coerce0, double %S.coerce1) #0 {
entry:
  %S = alloca %struct.vec2, align 8
  %0 = bitcast %struct.vec2* %S to { double, double }*
  %1 = getelementptr { double, double }* %0, i32 0, i32 0
  store double %S.coerce0, double* %1
  %2 = getelementptr { double, double }* %0, i32 0, i32 1
  store double %S.coerce1, double* %2
  %X = getelementptr inbounds %struct.vec2* %S, i32 0, i32 0
  %3 = load double* %X, align 8
  %Y = getelementptr inbounds %struct.vec2* %S, i32 0, i32 1
  %4 = load double* %Y, align 8
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str, i32 0, i32 0), double %3, double %4)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %U = alloca %struct.vec2, align 8
  %X = getelementptr inbounds %struct.vec2* %U, i32 0, i32 0
  store double 5.000000e-01, double* %X, align 8
  %Y = getelementptr inbounds %struct.vec2* %U, i32 0, i32 1
  store double 1.200000e+00, double* %Y, align 8
  %0 = bitcast %struct.vec2* %U to { double, double }*
  %1 = getelementptr { double, double }* %0, i32 0, i32 0
  %2 = load double* %1, align 1
  %3 = getelementptr { double, double }* %0, i32 0, i32 1
  %4 = load double* %3, align 1
  call void @print(double %2, double %4)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
