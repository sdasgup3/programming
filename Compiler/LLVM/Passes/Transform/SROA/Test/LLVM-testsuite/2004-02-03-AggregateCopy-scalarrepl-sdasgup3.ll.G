; ModuleID = '2004-02-03-AggregateCopy-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.agg = type { i32 }

@.str = private unnamed_addr constant [12 x i8] c"%d, %d, %d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %A = alloca %struct.agg, align 4
  %B = alloca %struct.agg, align 4
  %C = alloca %struct.agg, align 4
  %X = getelementptr inbounds %struct.agg* %A, i32 0, i32 0
  store i32 123, i32* %X, align 4
  %0 = bitcast %struct.agg* %C to i8*
  %1 = bitcast %struct.agg* %A to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 4, i32 4, i1 false)
  %2 = bitcast %struct.agg* %B to i8*
  %3 = bitcast %struct.agg* %C to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 4, i32 4, i1 false)
  %X1 = getelementptr inbounds %struct.agg* %A, i32 0, i32 0
  %4 = load i32* %X1, align 4
  %X2 = getelementptr inbounds %struct.agg* %B, i32 0, i32 0
  %5 = load i32* %X2, align 4
  %X3 = getelementptr inbounds %struct.agg* %C, i32 0, i32 0
  %6 = load i32* %X3, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), i32 %4, i32 %5, i32 %6)
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
