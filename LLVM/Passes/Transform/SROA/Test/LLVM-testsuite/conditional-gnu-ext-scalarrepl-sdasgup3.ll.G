; ModuleID = 'conditional-gnu-ext-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@getComplex.count = internal global i32 0, align 4

; Function Attrs: nounwind uwtable
define i64 @getComplex(i64 %val.coerce) #0 {
entry:
  %retval = alloca { i32, i32 }, align 4
  %val = alloca { i32, i32 }, align 8
  %0 = bitcast { i32, i32 }* %val to i64*
  store i64 %val.coerce, i64* %0, align 1
  %1 = load i32* @getComplex.count, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, i32* @getComplex.count, align 4
  %tobool = icmp ne i32 %1, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @abort() #2
  unreachable

if.end:                                           ; preds = %entry
  %val.realp = getelementptr inbounds { i32, i32 }* %val, i32 0, i32 0
  %val.real = load i32* %val.realp, align 4
  %val.imagp = getelementptr inbounds { i32, i32 }* %val, i32 0, i32 1
  %val.imag = load i32* %val.imagp, align 4
  %real = getelementptr inbounds { i32, i32 }* %retval, i32 0, i32 0
  %imag = getelementptr inbounds { i32, i32 }* %retval, i32 0, i32 1
  store i32 %val.real, i32* %real, align 4
  store i32 %val.imag, i32* %imag, align 4
  %2 = bitcast { i32, i32 }* %retval to i64*
  %3 = load i64* %2, align 1
  ret i64 %3
}

; Function Attrs: noreturn
declare void @abort() #1

; Function Attrs: nounwind uwtable
define i64 @doo() #0 {
entry:
  %retval = alloca { i32, i32 }, align 4
  %cond = alloca { i32, i32 }, align 4
  %rhs = alloca { i32, i32 }, align 4
  %coerce = alloca { i32, i32 }, align 4
  %coerce1 = alloca { i32, i32 }, align 4
  %real = getelementptr inbounds { i32, i32 }* %coerce, i32 0, i32 0
  %imag = getelementptr inbounds { i32, i32 }* %coerce, i32 0, i32 1
  store i32 1, i32* %real, align 4
  store i32 2, i32* %imag, align 4
  %0 = bitcast { i32, i32 }* %coerce to i64*
  %1 = load i64* %0, align 1
  %call = call i64 @getComplex(i64 %1)
  %2 = bitcast { i32, i32 }* %coerce1 to i64*
  store i64 %call, i64* %2, align 1
  %coerce1.realp = getelementptr inbounds { i32, i32 }* %coerce1, i32 0, i32 0
  %coerce1.real = load i32* %coerce1.realp, align 4
  %coerce1.imagp = getelementptr inbounds { i32, i32 }* %coerce1, i32 0, i32 1
  %coerce1.imag = load i32* %coerce1.imagp, align 4
  %tobool = icmp ne i32 %coerce1.real, 0
  %tobool2 = icmp ne i32 %coerce1.imag, 0
  %tobool3 = or i1 %tobool, %tobool2
  br i1 %tobool3, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  br label %cond.end

cond.false:                                       ; preds = %entry
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond.r = phi i32 [ %coerce1.real, %cond.true ], [ undef, %cond.false ]
  %cond.i = phi i32 [ %coerce1.imag, %cond.true ], [ undef, %cond.false ]
  %real4 = getelementptr inbounds { i32, i32 }* %retval, i32 0, i32 0
  %imag5 = getelementptr inbounds { i32, i32 }* %retval, i32 0, i32 1
  store i32 %cond.r, i32* %real4, align 4
  store i32 %cond.i, i32* %imag5, align 4
  %3 = bitcast { i32, i32 }* %retval to i64*
  %4 = load i64* %3, align 1
  ret i64 %4
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %coerce = alloca { i32, i32 }, align 4
  %call = call i64 @doo()
  %0 = bitcast { i32, i32 }* %coerce to i64*
  store i64 %call, i64* %0, align 1
  %coerce.realp = getelementptr inbounds { i32, i32 }* %coerce, i32 0, i32 0
  %coerce.real = load i32* %coerce.realp, align 4
  %coerce.imagp = getelementptr inbounds { i32, i32 }* %coerce, i32 0, i32 1
  %coerce.imag = load i32* %coerce.imagp, align 4
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
