; ModuleID = '2003-08-11-VaListArg-scalarrepl-sdasgup3.bc'
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
define void @test(i8* %fmt, %struct.__va_list_tag* %ap) #0 {
entry:
  %dw = alloca %struct.DWordS_struct, align 4
  %qw = alloca %struct.QuadWordS_struct, align 8
  %ls = alloca %struct.LargeS_struct, align 8
  %tmp = alloca %struct.QuadWordS_struct, align 8
  br label %while.cond

while.cond:                                       ; preds = %sw.epilog, %entry
  %fmt.addr.0 = phi i8* [ %fmt, %entry ], [ %incdec.ptr, %sw.epilog ]
  %0 = load i8* %fmt.addr.0, align 1
  %tobool = icmp ne i8 %0, 0
  br i1 %tobool, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %incdec.ptr = getelementptr inbounds i8* %fmt.addr.0, i32 1
  %1 = load i8* %fmt.addr.0, align 1
  %conv = sext i8 %1 to i32
  switch i32 %conv, label %sw.epilog [
    i32 115, label %sw.bb
    i32 105, label %sw.bb1
    i32 100, label %sw.bb14
    i32 108, label %sw.bb24
    i32 99, label %sw.bb37
    i32 68, label %sw.bb52
    i32 81, label %sw.bb67
    i32 76, label %sw.bb85
  ]

sw.bb:                                            ; preds = %while.body
  %gp_offset_p = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 0
  %gp_offset = load i32* %gp_offset_p
  %fits_in_gp = icmp ule i32 %gp_offset, 40
  br i1 %fits_in_gp, label %vaarg.in_reg, label %vaarg.in_mem

vaarg.in_reg:                                     ; preds = %sw.bb
  %2 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area = load i8** %2
  %3 = getelementptr i8* %reg_save_area, i32 %gp_offset
  %4 = bitcast i8* %3 to i8**
  %5 = add i32 %gp_offset, 8
  store i32 %5, i32* %gp_offset_p
  br label %vaarg.end

vaarg.in_mem:                                     ; preds = %sw.bb
  %overflow_arg_area_p = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area = load i8** %overflow_arg_area_p
  %6 = bitcast i8* %overflow_arg_area to i8**
  %overflow_arg_area.next = getelementptr i8* %overflow_arg_area, i32 8
  store i8* %overflow_arg_area.next, i8** %overflow_arg_area_p
  br label %vaarg.end

vaarg.end:                                        ; preds = %vaarg.in_mem, %vaarg.in_reg
  %vaarg.addr = phi i8** [ %4, %vaarg.in_reg ], [ %6, %vaarg.in_mem ]
  %7 = load i8** %vaarg.addr
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str, i32 0, i32 0), i8* %7)
  br label %sw.epilog

sw.bb1:                                           ; preds = %while.body
  %gp_offset_p2 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 0
  %gp_offset3 = load i32* %gp_offset_p2
  %fits_in_gp4 = icmp ule i32 %gp_offset3, 40
  br i1 %fits_in_gp4, label %vaarg.in_reg5, label %vaarg.in_mem7

vaarg.in_reg5:                                    ; preds = %sw.bb1
  %8 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area6 = load i8** %8
  %9 = getelementptr i8* %reg_save_area6, i32 %gp_offset3
  %10 = bitcast i8* %9 to i32*
  %11 = add i32 %gp_offset3, 8
  store i32 %11, i32* %gp_offset_p2
  br label %vaarg.end11

vaarg.in_mem7:                                    ; preds = %sw.bb1
  %overflow_arg_area_p8 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area9 = load i8** %overflow_arg_area_p8
  %12 = bitcast i8* %overflow_arg_area9 to i32*
  %overflow_arg_area.next10 = getelementptr i8* %overflow_arg_area9, i32 8
  store i8* %overflow_arg_area.next10, i8** %overflow_arg_area_p8
  br label %vaarg.end11

vaarg.end11:                                      ; preds = %vaarg.in_mem7, %vaarg.in_reg5
  %vaarg.addr12 = phi i32* [ %10, %vaarg.in_reg5 ], [ %12, %vaarg.in_mem7 ]
  %13 = load i32* %vaarg.addr12
  %call13 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str1, i32 0, i32 0), i32 %13)
  br label %sw.epilog

sw.bb14:                                          ; preds = %while.body
  %fp_offset_p = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 1
  %fp_offset = load i32* %fp_offset_p
  %fits_in_fp = icmp ule i32 %fp_offset, 160
  br i1 %fits_in_fp, label %vaarg.in_reg15, label %vaarg.in_mem17

vaarg.in_reg15:                                   ; preds = %sw.bb14
  %14 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area16 = load i8** %14
  %15 = getelementptr i8* %reg_save_area16, i32 %fp_offset
  %16 = bitcast i8* %15 to double*
  %17 = add i32 %fp_offset, 16
  store i32 %17, i32* %fp_offset_p
  br label %vaarg.end21

vaarg.in_mem17:                                   ; preds = %sw.bb14
  %overflow_arg_area_p18 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area19 = load i8** %overflow_arg_area_p18
  %18 = bitcast i8* %overflow_arg_area19 to double*
  %overflow_arg_area.next20 = getelementptr i8* %overflow_arg_area19, i32 8
  store i8* %overflow_arg_area.next20, i8** %overflow_arg_area_p18
  br label %vaarg.end21

vaarg.end21:                                      ; preds = %vaarg.in_mem17, %vaarg.in_reg15
  %vaarg.addr22 = phi double* [ %16, %vaarg.in_reg15 ], [ %18, %vaarg.in_mem17 ]
  %19 = load double* %vaarg.addr22
  %call23 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str2, i32 0, i32 0), double %19)
  br label %sw.epilog

sw.bb24:                                          ; preds = %while.body
  %gp_offset_p25 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 0
  %gp_offset26 = load i32* %gp_offset_p25
  %fits_in_gp27 = icmp ule i32 %gp_offset26, 40
  br i1 %fits_in_gp27, label %vaarg.in_reg28, label %vaarg.in_mem30

vaarg.in_reg28:                                   ; preds = %sw.bb24
  %20 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area29 = load i8** %20
  %21 = getelementptr i8* %reg_save_area29, i32 %gp_offset26
  %22 = bitcast i8* %21 to i64*
  %23 = add i32 %gp_offset26, 8
  store i32 %23, i32* %gp_offset_p25
  br label %vaarg.end34

vaarg.in_mem30:                                   ; preds = %sw.bb24
  %overflow_arg_area_p31 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area32 = load i8** %overflow_arg_area_p31
  %24 = bitcast i8* %overflow_arg_area32 to i64*
  %overflow_arg_area.next33 = getelementptr i8* %overflow_arg_area32, i32 8
  store i8* %overflow_arg_area.next33, i8** %overflow_arg_area_p31
  br label %vaarg.end34

vaarg.end34:                                      ; preds = %vaarg.in_mem30, %vaarg.in_reg28
  %vaarg.addr35 = phi i64* [ %22, %vaarg.in_reg28 ], [ %24, %vaarg.in_mem30 ]
  %25 = load i64* %vaarg.addr35
  %call36 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([16 x i8]* @.str3, i32 0, i32 0), i64 %25)
  br label %sw.epilog

sw.bb37:                                          ; preds = %while.body
  %gp_offset_p38 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 0
  %gp_offset39 = load i32* %gp_offset_p38
  %fits_in_gp40 = icmp ule i32 %gp_offset39, 40
  br i1 %fits_in_gp40, label %vaarg.in_reg41, label %vaarg.in_mem43

vaarg.in_reg41:                                   ; preds = %sw.bb37
  %26 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area42 = load i8** %26
  %27 = getelementptr i8* %reg_save_area42, i32 %gp_offset39
  %28 = bitcast i8* %27 to i32*
  %29 = add i32 %gp_offset39, 8
  store i32 %29, i32* %gp_offset_p38
  br label %vaarg.end47

vaarg.in_mem43:                                   ; preds = %sw.bb37
  %overflow_arg_area_p44 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area45 = load i8** %overflow_arg_area_p44
  %30 = bitcast i8* %overflow_arg_area45 to i32*
  %overflow_arg_area.next46 = getelementptr i8* %overflow_arg_area45, i32 8
  store i8* %overflow_arg_area.next46, i8** %overflow_arg_area_p44
  br label %vaarg.end47

vaarg.end47:                                      ; preds = %vaarg.in_mem43, %vaarg.in_reg41
  %vaarg.addr48 = phi i32* [ %28, %vaarg.in_reg41 ], [ %30, %vaarg.in_mem43 ]
  %31 = load i32* %vaarg.addr48
  %conv49 = trunc i32 %31 to i8
  %conv50 = sext i8 %conv49 to i32
  %call51 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([9 x i8]* @.str4, i32 0, i32 0), i32 %conv50)
  br label %sw.epilog

sw.bb52:                                          ; preds = %while.body
  %gp_offset_p53 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 0
  %gp_offset54 = load i32* %gp_offset_p53
  %fits_in_gp55 = icmp ule i32 %gp_offset54, 40
  br i1 %fits_in_gp55, label %vaarg.in_reg56, label %vaarg.in_mem58

vaarg.in_reg56:                                   ; preds = %sw.bb52
  %32 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area57 = load i8** %32
  %33 = getelementptr i8* %reg_save_area57, i32 %gp_offset54
  %34 = bitcast i8* %33 to %struct.DWordS_struct*
  %35 = add i32 %gp_offset54, 8
  store i32 %35, i32* %gp_offset_p53
  br label %vaarg.end62

vaarg.in_mem58:                                   ; preds = %sw.bb52
  %overflow_arg_area_p59 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area60 = load i8** %overflow_arg_area_p59
  %36 = bitcast i8* %overflow_arg_area60 to %struct.DWordS_struct*
  %overflow_arg_area.next61 = getelementptr i8* %overflow_arg_area60, i32 8
  store i8* %overflow_arg_area.next61, i8** %overflow_arg_area_p59
  br label %vaarg.end62

vaarg.end62:                                      ; preds = %vaarg.in_mem58, %vaarg.in_reg56
  %vaarg.addr63 = phi %struct.DWordS_struct* [ %34, %vaarg.in_reg56 ], [ %36, %vaarg.in_mem58 ]
  %37 = bitcast %struct.DWordS_struct* %dw to i8*
  %38 = bitcast %struct.DWordS_struct* %vaarg.addr63 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %37, i8* %38, i64 8, i32 4, i1 false)
  %i = getelementptr inbounds %struct.DWordS_struct* %dw, i32 0, i32 0
  %39 = load i32* %i, align 4
  %c64 = getelementptr inbounds %struct.DWordS_struct* %dw, i32 0, i32 1
  %40 = load i8* %c64, align 1
  %conv65 = sext i8 %40 to i32
  %call66 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([18 x i8]* @.str5, i32 0, i32 0), i32 %39, i32 %conv65)
  br label %sw.epilog

sw.bb67:                                          ; preds = %while.body
  %gp_offset_p68 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 0
  %gp_offset69 = load i32* %gp_offset_p68
  %fits_in_gp70 = icmp ule i32 %gp_offset69, 40
  %fp_offset_p71 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 1
  %fp_offset72 = load i32* %fp_offset_p71
  %fits_in_fp73 = icmp ule i32 %fp_offset72, 160
  %41 = and i1 %fits_in_gp70, %fits_in_fp73
  br i1 %41, label %vaarg.in_reg74, label %vaarg.in_mem76

vaarg.in_reg74:                                   ; preds = %sw.bb67
  %42 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 3
  %reg_save_area75 = load i8** %42
  %43 = bitcast %struct.QuadWordS_struct* %tmp to { i32, double }*
  %44 = getelementptr i8* %reg_save_area75, i32 %gp_offset69
  %45 = getelementptr i8* %reg_save_area75, i32 %fp_offset72
  %46 = bitcast i8* %44 to i32*
  %47 = load i32* %46
  %48 = getelementptr inbounds { i32, double }* %43, i32 0, i32 0
  store i32 %47, i32* %48
  %49 = bitcast i8* %45 to double*
  %50 = load double* %49
  %51 = getelementptr inbounds { i32, double }* %43, i32 0, i32 1
  store double %50, double* %51
  %52 = bitcast { i32, double }* %43 to %struct.QuadWordS_struct*
  %53 = add i32 %gp_offset69, 8
  store i32 %53, i32* %gp_offset_p68
  %54 = add i32 %fp_offset72, 16
  store i32 %54, i32* %fp_offset_p71
  br label %vaarg.end80

vaarg.in_mem76:                                   ; preds = %sw.bb67
  %overflow_arg_area_p77 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area78 = load i8** %overflow_arg_area_p77
  %55 = bitcast i8* %overflow_arg_area78 to %struct.QuadWordS_struct*
  %overflow_arg_area.next79 = getelementptr i8* %overflow_arg_area78, i32 16
  store i8* %overflow_arg_area.next79, i8** %overflow_arg_area_p77
  br label %vaarg.end80

vaarg.end80:                                      ; preds = %vaarg.in_mem76, %vaarg.in_reg74
  %vaarg.addr81 = phi %struct.QuadWordS_struct* [ %52, %vaarg.in_reg74 ], [ %55, %vaarg.in_mem76 ]
  %56 = bitcast %struct.QuadWordS_struct* %qw to i8*
  %57 = bitcast %struct.QuadWordS_struct* %vaarg.addr81 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %56, i8* %57, i64 16, i32 8, i1 false)
  %i82 = getelementptr inbounds %struct.QuadWordS_struct* %qw, i32 0, i32 0
  %58 = load i32* %i82, align 4
  %d83 = getelementptr inbounds %struct.QuadWordS_struct* %qw, i32 0, i32 1
  %59 = load double* %d83, align 8
  %call84 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([21 x i8]* @.str6, i32 0, i32 0), i32 %58, double %59)
  br label %sw.epilog

sw.bb85:                                          ; preds = %while.body
  %overflow_arg_area_p86 = getelementptr inbounds %struct.__va_list_tag* %ap, i32 0, i32 2
  %overflow_arg_area87 = load i8** %overflow_arg_area_p86
  %60 = bitcast i8* %overflow_arg_area87 to %struct.LargeS_struct*
  %overflow_arg_area.next88 = getelementptr i8* %overflow_arg_area87, i32 32
  store i8* %overflow_arg_area.next88, i8** %overflow_arg_area_p86
  %61 = bitcast %struct.LargeS_struct* %ls to i8*
  %62 = bitcast %struct.LargeS_struct* %60 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %61, i8* %62, i64 32, i32 8, i1 false)
  %i89 = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 0
  %63 = load i32* %i89, align 4
  %d90 = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 1
  %64 = load double* %d90, align 8
  %ptr = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 2
  %65 = load %struct.DWordS_struct** %ptr, align 8
  %cmp = icmp ne %struct.DWordS_struct* %65, null
  %conv91 = zext i1 %cmp to i32
  %j = getelementptr inbounds %struct.LargeS_struct* %ls, i32 0, i32 3
  %66 = load i32* %j, align 4
  %call92 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([29 x i8]* @.str7, i32 0, i32 0), i32 %63, double %64, i32 %conv91, i32 %66)
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.bb85, %vaarg.end80, %vaarg.end62, %vaarg.end47, %vaarg.end34, %vaarg.end21, %vaarg.end11, %vaarg.end, %while.body
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %67 = bitcast %struct.__va_list_tag* %ap to i8*
  call void @llvm.va_end(i8* %67)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #2

; Function Attrs: nounwind uwtable
define void @testVaListArg(i8* %fmt, ...) #0 {
entry:
  %ap = alloca [1 x %struct.__va_list_tag], align 16
  %arraydecay = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %arraydecay1 = bitcast %struct.__va_list_tag* %arraydecay to i8*
  call void @llvm.va_start(i8* %arraydecay1)
  %arraydecay2 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  call void @test(i8* %fmt, %struct.__va_list_tag* %arraydecay2)
  %arraydecay3 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %arraydecay34 = bitcast %struct.__va_list_tag* %arraydecay3 to i8*
  call void @llvm.va_end(i8* %arraydecay34)
  ret void
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #2

; Function Attrs: nounwind uwtable
define void @testVaCopyArg(i8* %fmt, ...) #0 {
entry:
  %ap = alloca [1 x %struct.__va_list_tag], align 16
  %aq = alloca [1 x %struct.__va_list_tag], align 16
  %arraydecay = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %arraydecay1 = bitcast %struct.__va_list_tag* %arraydecay to i8*
  call void @llvm.va_start(i8* %arraydecay1)
  %arraydecay2 = getelementptr inbounds [1 x %struct.__va_list_tag]* %aq, i32 0, i32 0
  %arraydecay3 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %0 = bitcast %struct.__va_list_tag* %arraydecay2 to i8*
  %1 = bitcast %struct.__va_list_tag* %arraydecay3 to i8*
  call void @llvm.va_copy(i8* %0, i8* %1)
  %arraydecay4 = getelementptr inbounds [1 x %struct.__va_list_tag]* %aq, i32 0, i32 0
  call void @test(i8* %fmt, %struct.__va_list_tag* %arraydecay4)
  %arraydecay5 = getelementptr inbounds [1 x %struct.__va_list_tag]* %aq, i32 0, i32 0
  %arraydecay56 = bitcast %struct.__va_list_tag* %arraydecay5 to i8*
  call void @llvm.va_end(i8* %arraydecay56)
  %arraydecay7 = getelementptr inbounds [1 x %struct.__va_list_tag]* %ap, i32 0, i32 0
  %arraydecay78 = bitcast %struct.__va_list_tag* %arraydecay7 to i8*
  call void @llvm.va_end(i8* %arraydecay78)
  ret void
}

; Function Attrs: nounwind
declare void @llvm.va_copy(i8*, i8*) #2

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
  call void (i8*, ...)* @testVaListArg(i8* getelementptr inbounds ([11 x i8]* @.str8, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8]* @.str9, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8]* @.str10, i32 0, i32 0), i32 -123, i32 97, i32 123, i32 6, i32 7, i32 8, i32 9, i8* getelementptr inbounds ([14 x i8]* @.str11, i32 0, i32 0))
  call void (i8*, ...)* @testVaCopyArg(i8* getelementptr inbounds ([11 x i8]* @.str8, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8]* @.str9, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8]* @.str10, i32 0, i32 0), i32 -123, i32 97, i32 123, i32 6, i32 7, i32 8, i32 9, i8* getelementptr inbounds ([14 x i8]* @.str11, i32 0, i32 0))
  call void (i8*, ...)* @testVaListArg(i8* getelementptr inbounds ([5 x i8]* @.str12, i32 0, i32 0), double 1.000000e+00, double 2.000000e+00, i32 32764, i64 12345677823423)
  call void (i8*, ...)* @testVaCopyArg(i8* getelementptr inbounds ([5 x i8]* @.str12, i32 0, i32 0), double 1.000000e+00, double 2.000000e+00, i32 32764, i64 12345677823423)
  %2 = bitcast %struct.DWordS_struct* %dw to i64*
  %3 = load i64* %2, align 1
  %4 = bitcast %struct.QuadWordS_struct* %qw to { i32, double }*
  %5 = getelementptr { i32, double }* %4, i32 0, i32 0
  %6 = load i32* %5, align 1
  %7 = getelementptr { i32, double }* %4, i32 0, i32 1
  %8 = load double* %7, align 1
  call void (i8*, ...)* @testVaListArg(i8* getelementptr inbounds ([4 x i8]* @.str13, i32 0, i32 0), i64 %3, i32 %6, double %8, %struct.LargeS_struct* byval align 8 %ls)
  %9 = bitcast %struct.DWordS_struct* %dw to i64*
  %10 = load i64* %9, align 1
  %11 = bitcast %struct.QuadWordS_struct* %qw to { i32, double }*
  %12 = getelementptr { i32, double }* %11, i32 0, i32 0
  %13 = load i32* %12, align 1
  %14 = getelementptr { i32, double }* %11, i32 0, i32 1
  %15 = load double* %14, align 1
  call void (i8*, ...)* @testVaCopyArg(i8* getelementptr inbounds ([4 x i8]* @.str13, i32 0, i32 0), i64 %10, i32 %13, double %15, %struct.LargeS_struct* byval align 8 %ls)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
