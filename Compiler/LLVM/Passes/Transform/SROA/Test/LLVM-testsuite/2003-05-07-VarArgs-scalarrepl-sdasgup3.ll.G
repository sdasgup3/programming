; ModuleID = '2003-05-07-VarArgs-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.QuadWordS_struct = type { i32, double }
%struct.__va_list_tag = type { i32, i32, i8*, i8* }
%struct.DWordS_struct = type { i32, i8 }
%struct.LargeS_struct = type { i32, double, %struct.DWordS_struct*, i32 }

@.str = private unnamed_addr constant [11 x i8] c"string %s\0A\00", align 1
@.str1 = private unnamed_addr constant [8 x i8] c"int %d\0A\00", align 1
@.str2 = private unnamed_addr constant [11 x i8] c"double %f\0A\00", align 1
@.str3 = private unnamed_addr constant [16 x i8] c"long long %lld\0A\00", align 1
@.str4 = private unnamed_addr constant [9 x i8] c"char %c\0A\00", align 1
@.str5 = private unnamed_addr constant [18 x i8] c"DWord { %d, %c }\0A\00", align 1
@.str6 = private unnamed_addr constant [21 x i8] c"QuadWord { %d, %f }\0A\00", align 1
@.str7 = private unnamed_addr constant [29 x i8] c"LargeS { %d, %f, 0x%d, %d }\0A\00", align 1
@main.dw = private unnamed_addr constant { i32, i8, [3 x i8] } { i32 18, i8 97, [3 x i8] undef }, align 4
@main.qw = private unnamed_addr constant %struct.QuadWordS_struct { i32 19, double 2.000000e+01 }, align 8
@.str8 = private unnamed_addr constant [11 x i8] c"ssiciiiiis\00", align 1
@.str9 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@.str10 = private unnamed_addr constant [4 x i8] c"def\00", align 1
@.str11 = private unnamed_addr constant [14 x i8] c"10 args done!\00", align 1
@.str12 = private unnamed_addr constant [5 x i8] c"ddil\00", align 1
@.str13 = private unnamed_addr constant [4 x i8] c"DQL\00", align 1

; Function Attrs: nounwind uwtable
define void @test(i8* %fmt, ...) #0 {
entry:
  %ap = alloca [1 x %struct.__va_list_tag], align 16
  %aq = alloca [1 x %struct.__va_list_tag], align 16
  %dw = alloca %struct.DWordS_struct, align 4
  %qw = alloca %struct.QuadWordS_struct, align 8
  %ls = alloca %struct.LargeS_struct, align 8
  %tmp = alloca %struct.QuadWordS_struct, align 8
  %arraydecay = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %arraydecay1 = bitcast %struct.__va_list_tag* %arraydecay to i8*
  call void @llvm.va_start(i8* %arraydecay1)
  %arraydecay2 = getelementptr inbounds [1 x %struct.__va_list_tag]* %aq, i32 0, i32 0
  %arraydecay3 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %0 = bitcast %struct.__va_list_tag* %arraydecay2 to i8*
  %1 = bitcast %struct.__va_list_tag* %arraydecay3 to i8*
  call void @llvm.va_copy(i8* %0, i8* %1)
  %arraydecay4 = getelementptr inbounds [1 x %struct.__va_list_tag]* %aq, i32 0, i32 0
  %arraydecay45 = bitcast %struct.__va_list_tag* %arraydecay4 to i8*
  call void @llvm.va_end(i8* %arraydecay45)
  br label %while.cond

while.cond:                                       ; preds = %sw.epilog, %entry
  %fmt.addr.0 = phi i8* [ %fmt, %entry ], [ %incdec.ptr, %sw.epilog ]
  %2 = load i8* %fmt.addr.0, align 1
  %tobool = icmp ne i8 %2, 0
  br i1 %tobool, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %incdec.ptr = getelementptr inbounds i8* %fmt.addr.0, i32 1
  %3 = load i8* %fmt.addr.0, align 1
  %conv = sext i8 %3 to i32
  switch i32 %conv, label %sw.epilog [
    i32 115, label %sw.bb
    i32 105, label %sw.bb7
    i32 100, label %sw.bb21
    i32 108, label %sw.bb32
    i32 99, label %sw.bb46
    i32 68, label %sw.bb62
    i32 81, label %sw.bb78
    i32 76, label %sw.bb97
  ]

sw.bb:                                            ; preds = %while.body
  %arraydecay6 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %gp_offset_p = getelementptr inbounds %struct.__va_list_tag* %arraydecay6, i32 0, i32 0
  %gp_offset = load i32* %gp_offset_p
  %fits_in_gp = icmp ule i32 %gp_offset, 40
  br i1 %fits_in_gp, label %vaarg.in_reg, label %vaarg.in_mem

vaarg.in_reg:                                     ; preds = %sw.bb
  %4 = getelementptr inbounds %struct.__va_list_tag* %arraydecay6, i32 0, i32 3
  %reg_save_area = load i8** %4
  %5 = getelementptr i8* %reg_save_area, i32 %gp_offset
  %6 = bitcast i8* %5 to i8**
  %7 = add i32 %gp_offset, 8
  store i32 %7, i32* %gp_offset_p
  br label %vaarg.end

vaarg.in_mem:                                     ; preds = %sw.bb
  %overflow_arg_area_p = getelementptr inbounds %struct.__va_list_tag* %arraydecay6, i32 0, i32 2
  %overflow_arg_area = load i8** %overflow_arg_area_p
  %8 = bitcast i8* %overflow_arg_area to i8**
  %overflow_arg_area.next = getelementptr i8* %overflow_arg_area, i32 8
  store i8* %overflow_arg_area.next, i8** %overflow_arg_area_p
  br label %vaarg.end

vaarg.end:                                        ; preds = %vaarg.in_mem, %vaarg.in_reg
  %vaarg.addr = phi i8** [ %6, %vaarg.in_reg ], [ %8, %vaarg.in_mem ]
  %9 = load i8** %vaarg.addr
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str, i32 0, i32 0), i8* %9)
  br label %sw.epilog

sw.bb7:                                           ; preds = %while.body
  %arraydecay8 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %gp_offset_p9 = getelementptr inbounds %struct.__va_list_tag* %arraydecay8, i32 0, i32 0
  %gp_offset10 = load i32* %gp_offset_p9
  %fits_in_gp11 = icmp ule i32 %gp_offset10, 40
  br i1 %fits_in_gp11, label %vaarg.in_reg12, label %vaarg.in_mem14

vaarg.in_reg12:                                   ; preds = %sw.bb7
  %10 = getelementptr inbounds %struct.__va_list_tag* %arraydecay8, i32 0, i32 3
  %reg_save_area13 = load i8** %10
  %11 = getelementptr i8* %reg_save_area13, i32 %gp_offset10
  %12 = bitcast i8* %11 to i32*
  %13 = add i32 %gp_offset10, 8
  store i32 %13, i32* %gp_offset_p9
  br label %vaarg.end18

vaarg.in_mem14:                                   ; preds = %sw.bb7
  %overflow_arg_area_p15 = getelementptr inbounds %struct.__va_list_tag* %arraydecay8, i32 0, i32 2
  %overflow_arg_area16 = load i8** %overflow_arg_area_p15
  %14 = bitcast i8* %overflow_arg_area16 to i32*
  %overflow_arg_area.next17 = getelementptr i8* %overflow_arg_area16, i32 8
  store i8* %overflow_arg_area.next17, i8** %overflow_arg_area_p15
  br label %vaarg.end18

vaarg.end18:                                      ; preds = %vaarg.in_mem14, %vaarg.in_reg12
  %vaarg.addr19 = phi i32* [ %12, %vaarg.in_reg12 ], [ %14, %vaarg.in_mem14 ]
  %15 = load i32* %vaarg.addr19
  %call20 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str1, i32 0, i32 0), i32 %15)
  br label %sw.epilog

sw.bb21:                                          ; preds = %while.body
  %arraydecay22 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %fp_offset_p = getelementptr inbounds %struct.__va_list_tag* %arraydecay22, i32 0, i32 1
  %fp_offset = load i32* %fp_offset_p
  %fits_in_fp = icmp ule i32 %fp_offset, 160
  br i1 %fits_in_fp, label %vaarg.in_reg23, label %vaarg.in_mem25

vaarg.in_reg23:                                   ; preds = %sw.bb21
  %16 = getelementptr inbounds %struct.__va_list_tag* %arraydecay22, i32 0, i32 3
  %reg_save_area24 = load i8** %16
  %17 = getelementptr i8* %reg_save_area24, i32 %fp_offset
  %18 = bitcast i8* %17 to double*
  %19 = add i32 %fp_offset, 16
  store i32 %19, i32* %fp_offset_p
  br label %vaarg.end29

vaarg.in_mem25:                                   ; preds = %sw.bb21
  %overflow_arg_area_p26 = getelementptr inbounds %struct.__va_list_tag* %arraydecay22, i32 0, i32 2
  %overflow_arg_area27 = load i8** %overflow_arg_area_p26
  %20 = bitcast i8* %overflow_arg_area27 to double*
  %overflow_arg_area.next28 = getelementptr i8* %overflow_arg_area27, i32 8
  store i8* %overflow_arg_area.next28, i8** %overflow_arg_area_p26
  br label %vaarg.end29

vaarg.end29:                                      ; preds = %vaarg.in_mem25, %vaarg.in_reg23
  %vaarg.addr30 = phi double* [ %18, %vaarg.in_reg23 ], [ %20, %vaarg.in_mem25 ]
  %21 = load double* %vaarg.addr30
  %call31 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str2, i32 0, i32 0), double %21)
  br label %sw.epilog

sw.bb32:                                          ; preds = %while.body
  %arraydecay33 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %gp_offset_p34 = getelementptr inbounds %struct.__va_list_tag* %arraydecay33, i32 0, i32 0
  %gp_offset35 = load i32* %gp_offset_p34
  %fits_in_gp36 = icmp ule i32 %gp_offset35, 40
  br i1 %fits_in_gp36, label %vaarg.in_reg37, label %vaarg.in_mem39

vaarg.in_reg37:                                   ; preds = %sw.bb32
  %22 = getelementptr inbounds %struct.__va_list_tag* %arraydecay33, i32 0, i32 3
  %reg_save_area38 = load i8** %22
  %23 = getelementptr i8* %reg_save_area38, i32 %gp_offset35
  %24 = bitcast i8* %23 to i64*
  %25 = add i32 %gp_offset35, 8
  store i32 %25, i32* %gp_offset_p34
  br label %vaarg.end43

vaarg.in_mem39:                                   ; preds = %sw.bb32
  %overflow_arg_area_p40 = getelementptr inbounds %struct.__va_list_tag* %arraydecay33, i32 0, i32 2
  %overflow_arg_area41 = load i8** %overflow_arg_area_p40
  %26 = bitcast i8* %overflow_arg_area41 to i64*
  %overflow_arg_area.next42 = getelementptr i8* %overflow_arg_area41, i32 8
  store i8* %overflow_arg_area.next42, i8** %overflow_arg_area_p40
  br label %vaarg.end43

vaarg.end43:                                      ; preds = %vaarg.in_mem39, %vaarg.in_reg37
  %vaarg.addr44 = phi i64* [ %24, %vaarg.in_reg37 ], [ %26, %vaarg.in_mem39 ]
  %27 = load i64* %vaarg.addr44
  %call45 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([16 x i8]* @.str3, i32 0, i32 0), i64 %27)
  br label %sw.epilog

sw.bb46:                                          ; preds = %while.body
  %arraydecay47 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %gp_offset_p48 = getelementptr inbounds %struct.__va_list_tag* %arraydecay47, i32 0, i32 0
  %gp_offset49 = load i32* %gp_offset_p48
  %fits_in_gp50 = icmp ule i32 %gp_offset49, 40
  br i1 %fits_in_gp50, label %vaarg.in_reg51, label %vaarg.in_mem53

vaarg.in_reg51:                                   ; preds = %sw.bb46
  %28 = getelementptr inbounds %struct.__va_list_tag* %arraydecay47, i32 0, i32 3
  %reg_save_area52 = load i8** %28
  %29 = getelementptr i8* %reg_save_area52, i32 %gp_offset49
  %30 = bitcast i8* %29 to i32*
  %31 = add i32 %gp_offset49, 8
  store i32 %31, i32* %gp_offset_p48
  br label %vaarg.end57

vaarg.in_mem53:                                   ; preds = %sw.bb46
  %overflow_arg_area_p54 = getelementptr inbounds %struct.__va_list_tag* %arraydecay47, i32 0, i32 2
  %overflow_arg_area55 = load i8** %overflow_arg_area_p54
  %32 = bitcast i8* %overflow_arg_area55 to i32*
  %overflow_arg_area.next56 = getelementptr i8* %overflow_arg_area55, i32 8
  store i8* %overflow_arg_area.next56, i8** %overflow_arg_area_p54
  br label %vaarg.end57

vaarg.end57:                                      ; preds = %vaarg.in_mem53, %vaarg.in_reg51
  %vaarg.addr58 = phi i32* [ %30, %vaarg.in_reg51 ], [ %32, %vaarg.in_mem53 ]
  %33 = load i32* %vaarg.addr58
  %conv59 = trunc i32 %33 to i8
  %conv60 = sext i8 %conv59 to i32
  %call61 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([9 x i8]* @.str4, i32 0, i32 0), i32 %conv60)
  br label %sw.epilog

sw.bb62:                                          ; preds = %while.body
  %arraydecay63 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %gp_offset_p64 = getelementptr inbounds %struct.__va_list_tag* %arraydecay63, i32 0, i32 0
  %gp_offset65 = load i32* %gp_offset_p64
  %fits_in_gp66 = icmp ule i32 %gp_offset65, 40
  br i1 %fits_in_gp66, label %vaarg.in_reg67, label %vaarg.in_mem69

vaarg.in_reg67:                                   ; preds = %sw.bb62
  %34 = getelementptr inbounds %struct.__va_list_tag* %arraydecay63, i32 0, i32 3
  %reg_save_area68 = load i8** %34
  %35 = getelementptr i8* %reg_save_area68, i32 %gp_offset65
  %36 = bitcast i8* %35 to %struct.DWordS_struct*
  %37 = add i32 %gp_offset65, 8
  store i32 %37, i32* %gp_offset_p64
  br label %vaarg.end73

vaarg.in_mem69:                                   ; preds = %sw.bb62
  %overflow_arg_area_p70 = getelementptr inbounds %struct.__va_list_tag* %arraydecay63, i32 0, i32 2
  %overflow_arg_area71 = load i8** %overflow_arg_area_p70
  %38 = bitcast i8* %overflow_arg_area71 to %struct.DWordS_struct*
  %overflow_arg_area.next72 = getelementptr i8* %overflow_arg_area71, i32 8
  store i8* %overflow_arg_area.next72, i8** %overflow_arg_area_p70
  br label %vaarg.end73

vaarg.end73:                                      ; preds = %vaarg.in_mem69, %vaarg.in_reg67
  %vaarg.addr74 = phi %struct.DWordS_struct* [ %36, %vaarg.in_reg67 ], [ %38, %vaarg.in_mem69 ]
  %39 = bitcast %struct.DWordS_struct* %dw to i8*
  %40 = bitcast %struct.DWordS_struct* %vaarg.addr74 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %39, i8* %40, i64 8, i32 4, i1 false)
  %i = getelementptr inbounds %struct.DWordS_struct* %dw, i32 0, i32 0
  %41 = load i32* %i, align 4
  %c75 = getelementptr inbounds %struct.DWordS_struct* %dw, i32 0, i32 1
  %42 = load i8* %c75, align 1
  %conv76 = sext i8 %42 to i32
  %call77 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([18 x i8]* @.str5, i32 0, i32 0), i32 %41, i32 %conv76)
  br label %sw.epilog

sw.bb78:                                          ; preds = %while.body
  %arraydecay79 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %gp_offset_p80 = getelementptr inbounds %struct.__va_list_tag* %arraydecay79, i32 0, i32 0
  %gp_offset81 = load i32* %gp_offset_p80
  %fits_in_gp82 = icmp ule i32 %gp_offset81, 40
  %fp_offset_p83 = getelementptr inbounds %struct.__va_list_tag* %arraydecay79, i32 0, i32 1
  %fp_offset84 = load i32* %fp_offset_p83
  %fits_in_fp85 = icmp ule i32 %fp_offset84, 160
  %43 = and i1 %fits_in_gp82, %fits_in_fp85
  br i1 %43, label %vaarg.in_reg86, label %vaarg.in_mem88

vaarg.in_reg86:                                   ; preds = %sw.bb78
  %44 = getelementptr inbounds %struct.__va_list_tag* %arraydecay79, i32 0, i32 3
  %reg_save_area87 = load i8** %44
  %45 = bitcast %struct.QuadWordS_struct* %tmp to { i32, double }*
  %46 = getelementptr i8* %reg_save_area87, i32 %gp_offset81
  %47 = getelementptr i8* %reg_save_area87, i32 %fp_offset84
  %48 = bitcast i8* %46 to i32*
  %49 = load i32* %48
  %50 = getelementptr inbounds { i32, double }* %45, i32 0, i32 0
  store i32 %49, i32* %50
  %51 = bitcast i8* %47 to double*
  %52 = load double* %51
  %53 = getelementptr inbounds { i32, double }* %45, i32 0, i32 1
  store double %52, double* %53
  %54 = bitcast { i32, double }* %45 to %struct.QuadWordS_struct*
  %55 = add i32 %gp_offset81, 8
  store i32 %55, i32* %gp_offset_p80
  %56 = add i32 %fp_offset84, 16
  store i32 %56, i32* %fp_offset_p83
  br label %vaarg.end92

vaarg.in_mem88:                                   ; preds = %sw.bb78
  %overflow_arg_area_p89 = getelementptr inbounds %struct.__va_list_tag* %arraydecay79, i32 0, i32 2
  %overflow_arg_area90 = load i8** %overflow_arg_area_p89
  %57 = bitcast i8* %overflow_arg_area90 to %struct.QuadWordS_struct*
  %overflow_arg_area.next91 = getelementptr i8* %overflow_arg_area90, i32 16
  store i8* %overflow_arg_area.next91, i8** %overflow_arg_area_p89
  br label %vaarg.end92

vaarg.end92:                                      ; preds = %vaarg.in_mem88, %vaarg.in_reg86
  %vaarg.addr93 = phi %struct.QuadWordS_struct* [ %54, %vaarg.in_reg86 ], [ %57, %vaarg.in_mem88 ]
  %58 = bitcast %struct.QuadWordS_struct* %qw to i8*
  %59 = bitcast %struct.QuadWordS_struct* %vaarg.addr93 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %58, i8* %59, i64 16, i32 8, i1 false)
  %i94 = getelementptr inbounds %struct.QuadWordS_struct* %qw, i32 0, i32 0
  %60 = load i32* %i94, align 4
  %d95 = getelementptr inbounds %struct.QuadWordS_struct* %qw, i32 0, i32 1
  %61 = load double* %d95, align 8
  %call96 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([21 x i8]* @.str6, i32 0, i32 0), i32 %60, double %61)
  br label %sw.epilog

sw.bb97:                                          ; preds = %while.body
  %arraydecay98 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %overflow_arg_area_p99 = getelementptr inbounds %struct.__va_list_tag* %arraydecay98, i32 0, i32 2
  %overflow_arg_area100 = load i8** %overflow_arg_area_p99
  %62 = bitcast i8* %overflow_arg_area100 to %struct.LargeS_struct*
  %overflow_arg_area.next101 = getelementptr i8* %overflow_arg_area100, i32 32
  store i8* %overflow_arg_area.next101, i8** %overflow_arg_area_p99
  %63 = bitcast %struct.LargeS_struct* %ls to i8*
  %64 = bitcast %struct.LargeS_struct* %62 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %63, i8* %64, i64 32, i32 8, i1 false)
  %i102 = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 0
  %65 = load i32* %i102, align 4
  %d103 = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 1
  %66 = load double* %d103, align 8
  %ptr = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 2
  %67 = load %struct.DWordS_struct** %ptr, align 8
  %cmp = icmp ne %struct.DWordS_struct* %67, null
  %conv104 = zext i1 %cmp to i32
  %j = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 3
  %68 = load i32* %j, align 4
  %call105 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([29 x i8]* @.str7, i32 0, i32 0), i32 %65, double %66, i32 %conv104, i32 %68)
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.bb97, %vaarg.end92, %vaarg.end73, %vaarg.end57, %vaarg.end43, %vaarg.end29, %vaarg.end18, %vaarg.end, %while.body
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %arraydecay106 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %arraydecay106107 = bitcast %struct.__va_list_tag* %arraydecay106 to i8*
  call void @llvm.va_end(i8* %arraydecay106107)
  ret void
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #1

; Function Attrs: nounwind
declare void @llvm.va_copy(i8*, i8*) #1

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %dw = alloca %struct.DWordS_struct, align 4
  %qw = alloca %struct.QuadWordS_struct, align 8
  %ls = alloca %struct.LargeS_struct, align 8
  %0 = bitcast %struct.DWordS_struct* %dw to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ({ i32, i8, [3 x i8] }* @main.dw to i8*), i64 8, i32 4, i1 false)
  %1 = bitcast %struct.QuadWordS_struct* %qw to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* bitcast (%struct.QuadWordS_struct* @main.qw to i8*), i64 16, i32 8, i1 false)
  %i = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 0
  store i32 21, i32* %i, align 4
  %d = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 1
  store double 2.200000e+01, double* %d, align 8
  %ptr = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 2
  store %struct.DWordS_struct* %dw, %struct.DWordS_struct** %ptr, align 8
  %j = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 3
  store i32 23, i32* %j, align 4
  call void (i8*, ...)* @test(i8* getelementptr inbounds ([11 x i8]* @.str8, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8]* @.str9, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8]* @.str10, i32 0, i32 0), i32 -123, i32 97, i32 123, i32 6, i32 7, i32 8, i32 9, i8* getelementptr inbounds ([14 x i8]* @.str11, i32 0, i32 0))
  call void (i8*, ...)* @test(i8* getelementptr inbounds ([5 x i8]* @.str12, i32 0, i32 0), double 1.000000e+00, double 2.000000e+00, i32 32764, i64 12345677823423)
  %2 = bitcast %struct.DWordS_struct* %dw to i64*
  %3 = load i64* %2, align 1
  %4 = bitcast %struct.QuadWordS_struct* %qw to { i32, double }*
  %5 = getelementptr { i32, double }* %4, i32 0, i32 0
  %6 = load i32* %5, align 1
  %7 = getelementptr { i32, double }* %4, i32 0, i32 1
  %8 = load double* %7, align 1
  call void (i8*, ...)* @test(i8* getelementptr inbounds ([4 x i8]* @.str13, i32 0, i32 0), i64 %3, i32 %6, double %8, %struct.LargeS_struct* byval align 8 %ls)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
