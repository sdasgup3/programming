; ModuleID = '2010-05-24-BitfieldTest-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.ia32_mcg_cap_t = type { i64 }
%struct.anon = type { [2 x i8], [6 x i8] }

@.str = private unnamed_addr constant [11 x i8] c"ctl_p == 0\00", align 1
@.str1 = private unnamed_addr constant [26 x i8] c"2010-05-24-BitfieldTest.c\00", align 1
@__PRETTY_FUNCTION__.main = private unnamed_addr constant [23 x i8] c"int main(int, char **)\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %ia32_mcg_cap = alloca %union.ia32_mcg_cap_t, align 8
  %u64 = bitcast %union.ia32_mcg_cap_t* %ia32_mcg_cap to i64*
  store i64 2054, i64* %u64, align 8
  %bits = bitcast %union.ia32_mcg_cap_t* %ia32_mcg_cap to %struct.anon*
  %0 = bitcast %struct.anon* %bits to i16*
  %bf.load = load i16* %0, align 8
  %bf.lshr = lshr i16 %bf.load, 8
  %bf.clear = and i16 %bf.lshr, 1
  %bf.cast = zext i16 %bf.clear to i64
  %cmp = icmp eq i64 %bf.cast, 0
  br i1 %cmp, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  br label %cond.end

cond.false:                                       ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([11 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8]* @.str1, i32 0, i32 0), i32 19, i8* getelementptr inbounds ([23 x i8]* @__PRETTY_FUNCTION__.main, i32 0, i32 0)) #2
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  ret i32 0
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noreturn nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
