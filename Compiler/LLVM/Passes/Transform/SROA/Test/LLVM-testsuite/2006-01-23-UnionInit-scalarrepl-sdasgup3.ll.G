; ModuleID = '2006-01-23-UnionInit-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.task_struct = type { [16 x i8] }
%struct.anon = type { i8, i8 }
%union.bt4 = type { %struct.at4 }
%struct.at4 = type { i64 }
%struct.anon.0 = type { i8, i8 }
%union.anon.3 = type { %struct.dvd_send_key }
%struct.dvd_send_key = type { i8, [5 x i8], [2 x i8] }
%struct.s = type { i32, i32, i32 }
%struct.foo2 = type <{ i16, [2 x i8], i64, i16, [2 x i8] }>
%struct.emptystruct = type {}
%union.ucast = type { %struct.s }
%struct.spinlock_t = type { %struct.emptystruct }
%struct.inflate_huft_s = type { %union.anon, i32 }
%union.anon = type { i32 }
%struct.bt1 = type { [2 x i8], [1 x i8], i8 }
%struct.one = type { i32, [0 x i32] }
%struct.anon.1 = type { i32, [0 x i32] }
%struct.anon.2 = type { i8*, [0 x i8] }

@init_task_union = global { %struct.task_struct, [16368 x i8] } { %struct.task_struct { [16 x i8] c"swapper\00\00\00\00\00\00\00\00\00" }, [16368 x i8] undef }, align 8
@fixed_tl = global <{ { { %struct.anon, [2 x i8] }, i32 } }> <{ { { %struct.anon, [2 x i8] }, i32 } { { %struct.anon, [2 x i8] } { %struct.anon { i8 96, i8 7 }, [2 x i8] undef }, i32 256 } }>, align 4
@bkv4 = global %union.bt4 { %struct.at4 { i64 61172160 } }, align 8
@cav1 = global { i8, i8, [1 x i8], i8 } { i8 100, i8 1, [1 x i8] c"\02", i8 undef }, align 4
@V = global { %struct.anon.0, [2 x i8] } { %struct.anon.0 { i8 1, i8 2 }, [2 x i8] undef }, align 4
@nd = global { [64 x i8] } { [64 x i8] c"relname\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00" }, align 4
@hobbit = global { i32, [3 x i32] } { i32 5, [3 x i32] [i32 1, i32 2, i32 3] }, align 4
@hobbit2 = global { i32, [3 x i32] } { i32 5, [3 x i32] [i32 1, i32 2, i32 3] }, align 4
@data = global { i32, [3 x i32] } { i32 0, [3 x i32] [i32 1, i32 2, i32 3] }, align 4
@s = global { i8*, [19 x i8] } { i8* null, [19 x i8] c"   xyzkasjdlf     \00" }, align 8
@ai = global %union.anon.3 { %struct.dvd_send_key { i8 3, [5 x i8] c"foo\00\00", [2 x i8] undef } }, align 4
@.str = private unnamed_addr constant [20 x i8] c"returning raw_lock\0A\00", align 1
@ucast_test.y = private unnamed_addr constant %struct.s { i32 1, i32 2, i32 0 }, align 4
@foo2 = global %struct.foo2 <{ i16 23122, [2 x i8] undef, i64 -12312731, i16 -312, [2 x i8] undef }>, align 4
@.str1 = private unnamed_addr constant [11 x i8] c"PR156: %s\0A\00", align 1
@.str2 = private unnamed_addr constant [21 x i8] c"PR295/PR568: %d, %d\0A\00", align 1
@.str3 = private unnamed_addr constant [23 x i8] c"PR574: %d, %d, %d, %d\0A\00", align 1
@.str4 = private unnamed_addr constant [19 x i8] c"PR162: %d, %d, %d\0A\00", align 1
@.str5 = private unnamed_addr constant [15 x i8] c"PR650: %s, %d\0A\00", align 1
@.str6 = private unnamed_addr constant [23 x i8] c"PR199: %d, %d, %d, %d\0A\00", align 1
@.str7 = private unnamed_addr constant [23 x i8] c"PR431: %d, %d, %d, %d\0A\00", align 1
@.str8 = private unnamed_addr constant [18 x i8] c"PR654: %ld, '%s'\0A\00", align 1
@.str9 = private unnamed_addr constant [17 x i8] c"PR323: %d, '%s'\0A\00", align 1
@.str10 = private unnamed_addr constant [11 x i8] c"PR627: %d\0A\00", align 1
@.str11 = private unnamed_addr constant [22 x i8] c"PR684: %d, %d, %d %d\0A\00", align 1
@.str12 = private unnamed_addr constant [28 x i8] c"rdar://6828787: %d, %d, %d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i8* @foo() #0 {
entry:
  ret i8* bitcast ({ i32, [3 x i32] }* @hobbit2 to i8*)
}

; Function Attrs: nounwind uwtable
define void @one_raw_spinlock() #0 {
entry:
  %retval = alloca %struct.emptystruct, align 1
  %raw = alloca %struct.emptystruct, align 1
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([20 x i8]* @.str, i32 0, i32 0))
  %0 = bitcast %struct.emptystruct* %retval to i8*
  %1 = bitcast %struct.emptystruct* %raw to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 0, i32 1, i1 false)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind uwtable
define { i64, i32 } @ucast_test() #0 {
entry:
  %retval = alloca %union.ucast, align 4
  %y = alloca %struct.s, align 4
  %z = alloca %union.ucast, align 4
  %tmp = alloca { i64, i32 }
  %0 = bitcast %struct.s* %y to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast (%struct.s* @ucast_test.y to i8*), i64 12, i32 4, i1 false)
  %1 = bitcast %union.ucast* %z to %struct.s*
  %2 = bitcast %struct.s* %1 to i8*
  %3 = bitcast %struct.s* %y to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 12, i32 4, i1 false)
  %4 = bitcast %union.ucast* %retval to i8*
  %5 = bitcast %union.ucast* %z to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 12, i32 4, i1 false)
  %coerce.dive = getelementptr %union.ucast* %retval, i32 0, i32 0
  %6 = bitcast { i64, i32 }* %tmp to i8*
  %7 = bitcast %struct.s* %coerce.dive to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 12, i32 1, i1 false)
  %8 = load { i64, i32 }* %tmp
  ret { i64, i32 } %8
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %XX = alloca %union.ucast, align 4
  %lock = alloca %struct.spinlock_t, align 1
  %.compoundliteral = alloca %struct.spinlock_t, align 1
  %undef.agg.tmp = alloca %struct.emptystruct, align 1
  %coerce = alloca %union.ucast, align 4
  %tmp = alloca { i64, i32 }
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str1, i32 0, i32 0), i8* getelementptr inbounds ({ %struct.task_struct, [16368 x i8] }* @init_task_union, i32 0, i32 0, i32 0, i32 0))
  %0 = load i32* bitcast (<{ { { %struct.anon, [2 x i8] }, i32 } }>* @fixed_tl to i32*), align 4
  %1 = load i32* getelementptr inbounds ([1 x %struct.inflate_huft_s]* bitcast (<{ { { %struct.anon, [2 x i8] }, i32 } }>* @fixed_tl to [1 x %struct.inflate_huft_s]*), i32 0, i64 0, i32 1), align 4
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([21 x i8]* @.str2, i32 0, i32 0), i32 %0, i32 %1)
  %2 = load i64* getelementptr inbounds (%union.bt4* @bkv4, i32 0, i32 0, i32 0), align 8
  %conv = trunc i64 %2 to i32
  %bf.load = load i16* bitcast ({ i8, i8, [1 x i8], i8 }* @cav1 to i16*), align 4
  %bf.shl = shl i16 %bf.load, 12
  %bf.ashr = ashr i16 %bf.shl, 12
  %bf.cast = sext i16 %bf.ashr to i32
  %bf.load2 = load i16* bitcast ({ i8, i8, [1 x i8], i8 }* @cav1 to i16*), align 4
  %bf.shl3 = shl i16 %bf.load2, 6
  %bf.ashr4 = ashr i16 %bf.shl3, 10
  %bf.cast5 = sext i16 %bf.ashr4 to i32
  %3 = load i8* getelementptr inbounds (%struct.bt1* bitcast ({ i8, i8, [1 x i8], i8 }* @cav1 to %struct.bt1*), i32 0, i32 1, i64 0), align 1
  %conv6 = zext i8 %3 to i32
  %call7 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([23 x i8]* @.str3, i32 0, i32 0), i32 %conv, i32 %bf.cast, i32 %bf.cast5, i32 %conv6)
  %4 = load i8* getelementptr inbounds ({ %struct.anon.0, [2 x i8] }* @V, i32 0, i32 0, i32 0), align 1
  %conv8 = sext i8 %4 to i32
  %5 = load i8* getelementptr inbounds ({ %struct.anon.0, [2 x i8] }* @V, i32 0, i32 0, i32 1), align 1
  %conv9 = sext i8 %5 to i32
  %6 = load i32* bitcast ({ %struct.anon.0, [2 x i8] }* @V to i32*), align 4
  %call10 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([19 x i8]* @.str4, i32 0, i32 0), i32 %conv8, i32 %conv9, i32 %6)
  %7 = load i32* bitcast ({ [64 x i8] }* @nd to i32*), align 4
  %call11 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([15 x i8]* @.str5, i32 0, i32 0), i8* getelementptr inbounds ({ [64 x i8] }* @nd, i32 0, i32 0, i32 0), i32 %7)
  %8 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit to %struct.one*), i32 0, i32 0), align 4
  %9 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit to %struct.one*), i32 0, i32 1, i64 0), align 1
  %10 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit to %struct.one*), i32 0, i32 1, i64 1), align 1
  %11 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit to %struct.one*), i32 0, i32 1, i64 2), align 1
  %call12 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([23 x i8]* @.str6, i32 0, i32 0), i32 %8, i32 %9, i32 %10, i32 %11)
  %12 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit2 to %struct.one*), i32 0, i32 0), align 4
  %13 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit2 to %struct.one*), i32 0, i32 1, i64 0), align 1
  %14 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit2 to %struct.one*), i32 0, i32 1, i64 1), align 1
  %15 = load i32* getelementptr inbounds (%struct.one* bitcast ({ i32, [3 x i32] }* @hobbit2 to %struct.one*), i32 0, i32 1, i64 2), align 1
  %call13 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([23 x i8]* @.str6, i32 0, i32 0), i32 %12, i32 %13, i32 %14, i32 %15)
  %16 = load i32* getelementptr inbounds (%struct.anon.1* bitcast ({ i32, [3 x i32] }* @data to %struct.anon.1*), i32 0, i32 0), align 4
  %17 = load i32* getelementptr inbounds (%struct.anon.1* bitcast ({ i32, [3 x i32] }* @data to %struct.anon.1*), i32 0, i32 1, i64 0), align 1
  %18 = load i32* getelementptr inbounds (%struct.anon.1* bitcast ({ i32, [3 x i32] }* @data to %struct.anon.1*), i32 0, i32 1, i64 1), align 1
  %19 = load i32* getelementptr inbounds (%struct.anon.1* bitcast ({ i32, [3 x i32] }* @data to %struct.anon.1*), i32 0, i32 1, i64 2), align 1
  %call14 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([23 x i8]* @.str7, i32 0, i32 0), i32 %16, i32 %17, i32 %18, i32 %19)
  %20 = load i8** getelementptr inbounds (%struct.anon.2* bitcast ({ i8*, [19 x i8] }* @s to %struct.anon.2*), i32 0, i32 0), align 8
  %21 = ptrtoint i8* %20 to i64
  %call15 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([18 x i8]* @.str8, i32 0, i32 0), i64 %21, i8* getelementptr inbounds (%struct.anon.2* bitcast ({ i8*, [19 x i8] }* @s to %struct.anon.2*), i32 0, i32 1, i32 0))
  %bf.load16 = load i8* getelementptr inbounds (%union.anon.3* @ai, i32 0, i32 0, i32 0), align 4
  %bf.clear = and i8 %bf.load16, 3
  %bf.cast17 = zext i8 %bf.clear to i32
  %call18 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([17 x i8]* @.str9, i32 0, i32 0), i32 %bf.cast17, i8* getelementptr inbounds (%union.anon.3* @ai, i32 0, i32 0, i32 1, i32 0))
  call void @one_raw_spinlock()
  %22 = bitcast %struct.spinlock_t* %lock to i8*
  %23 = bitcast %struct.spinlock_t* %.compoundliteral to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %22, i8* %23, i64 0, i32 1, i1 false)
  %call19 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str10, i32 0, i32 0), i32 0)
  %call20 = call { i64, i32 } @ucast_test()
  %coerce.dive = getelementptr %union.ucast* %coerce, i32 0, i32 0
  store { i64, i32 } %call20, { i64, i32 }* %tmp
  %24 = bitcast { i64, i32 }* %tmp to i8*
  %25 = bitcast %struct.s* %coerce.dive to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %25, i8* %24, i64 12, i32 1, i1 false)
  %26 = bitcast %union.ucast* %XX to i8*
  %27 = bitcast %union.ucast* %coerce to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %26, i8* %27, i64 12, i32 4, i1 false)
  %s = bitcast %union.ucast* %XX to %struct.s*
  %i1 = getelementptr inbounds %struct.s* %s, i32 0, i32 0
  %28 = load i32* %i1, align 4
  %s21 = bitcast %union.ucast* %XX to %struct.s*
  %i2 = getelementptr inbounds %struct.s* %s21, i32 0, i32 1
  %29 = load i32* %i2, align 4
  %s22 = bitcast %union.ucast* %XX to %struct.s*
  %i3 = getelementptr inbounds %struct.s* %s22, i32 0, i32 2
  %30 = load i32* %i3, align 4
  %i = bitcast %union.ucast* %XX to i32*
  %31 = load i32* %i, align 4
  %call23 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @.str11, i32 0, i32 0), i32 %28, i32 %29, i32 %30, i32 %31)
  %32 = load i16* getelementptr inbounds (%struct.foo2* @foo2, i32 0, i32 0), align 2
  %conv24 = sext i16 %32 to i32
  %33 = load i64* getelementptr inbounds (%struct.foo2* @foo2, i32 0, i32 2), align 4
  %conv25 = trunc i64 %33 to i32
  %34 = load i16* getelementptr inbounds (%struct.foo2* @foo2, i32 0, i32 3), align 2
  %conv26 = sext i16 %34 to i32
  %call27 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([28 x i8]* @.str12, i32 0, i32 0), i32 %conv24, i32 %conv25, i32 %conv26)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
