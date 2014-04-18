; ModuleID = '2003-10-12-GlobalVarInitializers-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.anon = type { i32 }
%union.anon.0 = type { i32 }

@GlobalUnion = global %union.anon { i32 1891631104 }, align 4
@main.LocalUnion = private unnamed_addr constant %union.anon.0 { i32 2143289344 }, align 4
@.str = private unnamed_addr constant [7 x i8] c"%f %f\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %LocalUnion = alloca %union.anon.0, align 4
  %0 = bitcast %union.anon.0* %LocalUnion to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast (%union.anon.0* @main.LocalUnion to i8*), i64 4, i32 4, i1 false)
  %1 = load float* bitcast (%union.anon* @GlobalUnion to float*), align 4
  %conv = fpext float %1 to double
  %__d = bitcast %union.anon.0* %LocalUnion to float*
  %2 = load float* %__d, align 4
  %conv1 = fpext float %2 to double
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([7 x i8]* @.str, i32 0, i32 0), double %conv, double %conv1)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

declare i32 @printf(i8*, ...) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
