; ModuleID = '2009-12-07-StructReturn-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.barstruct = type { %struct.foostruct, float }
%struct.foostruct = type { [3 x float] }

@bar_ary = common global [4 x %struct.barstruct] zeroinitializer, align 16
@foobify.my_static_foo = internal global %struct.barstruct { %struct.foostruct { [3 x float] [float 4.200000e+01, float 4.200000e+01, float 4.200000e+01] }, float 4.200000e+01 }, align 4
@.str = private unnamed_addr constant [51 x i8] c"error: store clobbered memory outside destination\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define float* @spooky(i32 %i) #0 {
entry:
  %idxprom = sext i32 %i to i64
  %arrayidx = getelementptr inbounds [4 x %struct.barstruct]* @bar_ary, i32 0, i64 %idxprom
  %safe = getelementptr inbounds %struct.barstruct* %arrayidx, i32 0, i32 1
  store float 1.420000e+02, float* %safe, align 4
  %idxprom1 = sext i32 %i to i64
  %arrayidx2 = getelementptr inbounds [4 x %struct.barstruct]* @bar_ary, i32 0, i64 %idxprom1
  %safe3 = getelementptr inbounds %struct.barstruct* %arrayidx2, i32 0, i32 1
  ret float* %safe3
}

; Function Attrs: noinline nounwind uwtable
define { <2 x float>, float } @foobify() #0 {
entry:
  %retval = alloca %struct.foostruct, align 4
  %tmp = alloca { <2 x float>, float }
  %0 = bitcast %struct.foostruct* %retval to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast (%struct.barstruct* @foobify.my_static_foo to i8*), i64 12, i32 4, i1 false)
  %coerce.dive = getelementptr %struct.foostruct* %retval, i32 0, i32 0
  %1 = bitcast { <2 x float>, float }* %tmp to i8*
  %2 = bitcast [3 x float]* %coerce.dive to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* %2, i64 12, i32 1, i1 false)
  %3 = load { <2 x float>, float }* %tmp
  ret { <2 x float>, float } %3
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #2 {
entry:
  %coerce = alloca %struct.foostruct, align 4
  %tmp = alloca { <2 x float>, float }
  %call = call float* @spooky(i32 0)
  %call1 = call { <2 x float>, float } @foobify()
  %coerce.dive = getelementptr %struct.foostruct* %coerce, i32 0, i32 0
  store { <2 x float>, float } %call1, { <2 x float>, float }* %tmp
  %0 = bitcast { <2 x float>, float }* %tmp to i8*
  %1 = bitcast [3 x float]* %coerce.dive to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* %0, i64 12, i32 1, i1 false)
  %2 = bitcast %struct.foostruct* %coerce to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* bitcast ([4 x %struct.barstruct]* @bar_ary to i8*), i8* %2, i64 12, i32 4, i1 false)
  %3 = load float* %call, align 4
  %conv = fpext float %3 to double
  %cmp = fcmp une double %conv, 1.420000e+02
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([51 x i8]* @.str, i32 0, i32 0))
  call void @abort() #5
  unreachable

if.end:                                           ; preds = %entry
  ret i32 0
}

declare i32 @printf(i8*, ...) #3

; Function Attrs: noreturn
declare void @abort() #4

attributes #0 = { noinline nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noreturn }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
