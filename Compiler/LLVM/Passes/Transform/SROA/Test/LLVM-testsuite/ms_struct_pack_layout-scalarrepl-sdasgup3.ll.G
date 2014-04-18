; ModuleID = 'ms_struct_pack_layout-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.one = type { i32, i8, i8, i8, i8, i8 }
%struct.two = type { i32, i8, [3 x i8], i8, [3 x i8], i8 }
%struct.three = type { i16, [2 x i8], i8 }
%struct.four = type { [2 x i8], [2 x i8], i8, [3 x i8] }
%struct.five = type { i8, i8, i8 }
%struct.six = type { i8, [3 x i8], i8, i8, [2 x i8] }
%struct.seven = type { i8, i8, i8 }
%struct.eight = type { i8, i8, i8, i8 }
%union.nine = type { i8, [7 x i8] }
%struct.ten = type { i8, [7 x i8], i8, [7 x i8] }

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %filler = alloca [16 x i8], align 16
  %test_one = alloca %struct.one, align 4
  %test_two = alloca %struct.two, align 4
  %test_three = alloca %struct.three, align 2
  %test_four = alloca %struct.four, align 4
  %test_five = alloca %struct.five, align 1
  %test_six = alloca %struct.six, align 4
  %test_seven = alloca %struct.seven, align 1
  %test_eight = alloca %struct.eight, align 2
  %test_nine = alloca %union.nine, align 8
  %test_ten = alloca %struct.ten, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i8 [ 0, %entry ], [ %inc, %for.inc ]
  %conv = zext i8 %i.0 to i32
  %cmp = icmp slt i32 %conv, 16
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = zext i8 %i.0 to i64
  %arrayidx = getelementptr inbounds [16 x i8]* %filler, i32 0, i64 %idxprom
  store i8 %i.0, i8* %arrayidx, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add i8 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %0 = bitcast %struct.one* %test_one to i8*
  %1 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 12, i32 4, i1 false)
  %c = getelementptr inbounds %struct.one* %test_one, i32 0, i32 5
  %2 = load i8* %c, align 1
  %conv2 = sext i8 %2 to i32
  %conv3 = zext i8 8 to i32
  %cmp4 = icmp ne i32 %conv2, %conv3
  br i1 %cmp4, label %if.then, label %if.end

if.then:                                          ; preds = %for.end
  call void @abort() #3
  unreachable

if.end:                                           ; preds = %for.end
  %3 = bitcast %struct.two* %test_two to i8*
  %4 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3, i8* %4, i64 16, i32 4, i1 false)
  %c6 = getelementptr inbounds %struct.two* %test_two, i32 0, i32 5
  %5 = load i8* %c6, align 1
  %conv7 = sext i8 %5 to i32
  %conv8 = zext i8 12 to i32
  %cmp9 = icmp ne i32 %conv7, %conv8
  br i1 %cmp9, label %if.then11, label %if.end12

if.then11:                                        ; preds = %if.end
  call void @abort() #3
  unreachable

if.end12:                                         ; preds = %if.end
  %6 = bitcast %struct.three* %test_three to i8*
  %7 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 6, i32 2, i1 false)
  %c13 = getelementptr inbounds %struct.three* %test_three, i32 0, i32 2
  %bf.load = load i8* %c13, align 2
  %bf.clear = and i8 %bf.load, 127
  %conv14 = zext i8 %bf.clear to i32
  %conv15 = zext i8 4 to i32
  %cmp16 = icmp ne i32 %conv14, %conv15
  br i1 %cmp16, label %if.then18, label %if.end19

if.then18:                                        ; preds = %if.end12
  call void @abort() #3
  unreachable

if.end19:                                         ; preds = %if.end12
  %8 = bitcast %struct.four* %test_four to i8*
  %9 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %8, i8* %9, i64 8, i32 4, i1 false)
  %c20 = getelementptr inbounds %struct.four* %test_four, i32 0, i32 2
  %bf.load21 = load i8* %c20, align 4
  %bf.clear22 = and i8 %bf.load21, 127
  %conv23 = zext i8 %bf.clear22 to i32
  %conv24 = zext i8 4 to i32
  %cmp25 = icmp ne i32 %conv23, %conv24
  br i1 %cmp25, label %if.then27, label %if.end28

if.then27:                                        ; preds = %if.end19
  call void @abort() #3
  unreachable

if.end28:                                         ; preds = %if.end19
  %10 = bitcast %struct.five* %test_five to i8*
  %11 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %10, i8* %11, i64 3, i32 1, i1 false)
  %c29 = getelementptr inbounds %struct.five* %test_five, i32 0, i32 2
  %12 = load i8* %c29, align 1
  %conv30 = sext i8 %12 to i32
  %conv31 = sext i8 2 to i32
  %cmp32 = icmp ne i32 %conv30, %conv31
  br i1 %cmp32, label %if.then34, label %if.end35

if.then34:                                        ; preds = %if.end28
  call void @abort() #3
  unreachable

if.end35:                                         ; preds = %if.end28
  %13 = bitcast %struct.six* %test_six to i8*
  %14 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %13, i8* %14, i64 8, i32 4, i1 false)
  %c36 = getelementptr inbounds %struct.six* %test_six, i32 0, i32 3
  %15 = load i8* %c36, align 1
  %conv37 = sext i8 %15 to i32
  %conv38 = sext i8 5 to i32
  %cmp39 = icmp ne i32 %conv37, %conv38
  br i1 %cmp39, label %if.then41, label %if.end42

if.then41:                                        ; preds = %if.end35
  call void @abort() #3
  unreachable

if.end42:                                         ; preds = %if.end35
  %16 = bitcast %struct.seven* %test_seven to i8*
  %17 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %16, i8* %17, i64 3, i32 1, i1 false)
  %c43 = getelementptr inbounds %struct.seven* %test_seven, i32 0, i32 2
  %18 = load i8* %c43, align 1
  %conv44 = sext i8 %18 to i32
  %conv45 = sext i8 2 to i32
  %cmp46 = icmp ne i32 %conv44, %conv45
  br i1 %cmp46, label %if.then48, label %if.end49

if.then48:                                        ; preds = %if.end42
  call void @abort() #3
  unreachable

if.end49:                                         ; preds = %if.end42
  %19 = bitcast %struct.eight* %test_eight to i8*
  %20 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %19, i8* %20, i64 4, i32 2, i1 false)
  %c50 = getelementptr inbounds %struct.eight* %test_eight, i32 0, i32 2
  %21 = load i8* %c50, align 1
  %conv51 = sext i8 %21 to i32
  %conv52 = sext i8 2 to i32
  %cmp53 = icmp ne i32 %conv51, %conv52
  br i1 %cmp53, label %if.then55, label %if.end56

if.then55:                                        ; preds = %if.end49
  call void @abort() #3
  unreachable

if.end56:                                         ; preds = %if.end49
  %22 = bitcast %union.nine* %test_nine to i8*
  %23 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %22, i8* %23, i64 8, i32 8, i1 false)
  %c57 = bitcast %union.nine* %test_nine to i8*
  %24 = load i8* %c57, align 1
  %conv58 = sext i8 %24 to i32
  %conv59 = sext i8 0 to i32
  %cmp60 = icmp ne i32 %conv58, %conv59
  br i1 %cmp60, label %if.then62, label %if.end63

if.then62:                                        ; preds = %if.end56
  call void @abort() #3
  unreachable

if.end63:                                         ; preds = %if.end56
  %25 = bitcast %struct.ten* %test_ten to i8*
  %26 = bitcast [16 x i8]* %filler to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %25, i8* %26, i64 16, i32 8, i1 false)
  %c64 = getelementptr inbounds %struct.ten* %test_ten, i32 0, i32 2
  %27 = load i8* %c64, align 1
  %conv65 = sext i8 %27 to i32
  %conv66 = sext i8 8 to i32
  %cmp67 = icmp ne i32 %conv65, %conv66
  br i1 %cmp67, label %if.then69, label %if.end70

if.then69:                                        ; preds = %if.end63
  call void @abort() #3
  unreachable

if.end70:                                         ; preds = %if.end63
  %cmp71 = icmp ne i64 12, 12
  br i1 %cmp71, label %if.then73, label %if.end74

if.then73:                                        ; preds = %if.end70
  call void @abort() #3
  unreachable

if.end74:                                         ; preds = %if.end70
  %cmp75 = icmp ne i64 16, 16
  br i1 %cmp75, label %if.then77, label %if.end78

if.then77:                                        ; preds = %if.end74
  call void @abort() #3
  unreachable

if.end78:                                         ; preds = %if.end74
  %cmp79 = icmp ne i64 6, 6
  br i1 %cmp79, label %if.then81, label %if.end82

if.then81:                                        ; preds = %if.end78
  call void @abort() #3
  unreachable

if.end82:                                         ; preds = %if.end78
  %cmp83 = icmp ne i64 8, 8
  br i1 %cmp83, label %if.then85, label %if.end86

if.then85:                                        ; preds = %if.end82
  call void @abort() #3
  unreachable

if.end86:                                         ; preds = %if.end82
  %cmp87 = icmp ne i64 3, 3
  br i1 %cmp87, label %if.then89, label %if.end90

if.then89:                                        ; preds = %if.end86
  call void @abort() #3
  unreachable

if.end90:                                         ; preds = %if.end86
  %cmp91 = icmp ne i64 8, 8
  br i1 %cmp91, label %if.then93, label %if.end94

if.then93:                                        ; preds = %if.end90
  call void @abort() #3
  unreachable

if.end94:                                         ; preds = %if.end90
  %cmp95 = icmp ne i64 3, 3
  br i1 %cmp95, label %if.then97, label %if.end98

if.then97:                                        ; preds = %if.end94
  call void @abort() #3
  unreachable

if.end98:                                         ; preds = %if.end94
  %cmp99 = icmp ne i64 4, 4
  br i1 %cmp99, label %if.then101, label %if.end102

if.then101:                                       ; preds = %if.end98
  call void @abort() #3
  unreachable

if.end102:                                        ; preds = %if.end98
  %cmp103 = icmp ne i64 8, 8
  br i1 %cmp103, label %if.then105, label %if.end106

if.then105:                                       ; preds = %if.end102
  call void @abort() #3
  unreachable

if.end106:                                        ; preds = %if.end102
  %cmp107 = icmp ne i64 16, 16
  br i1 %cmp107, label %if.then109, label %if.end110

if.then109:                                       ; preds = %if.end106
  call void @abort() #3
  unreachable

if.end110:                                        ; preds = %if.end106
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: noreturn
declare void @abort() #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noreturn }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
