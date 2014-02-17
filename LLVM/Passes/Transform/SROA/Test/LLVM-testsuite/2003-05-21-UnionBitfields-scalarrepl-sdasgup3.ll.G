; ModuleID = '2003-05-21-UnionBitfields-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.anon = type { double }
%struct.anon = type { i32, [4 x i8] }

@.str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @target_isinf(double %x) #0 {
entry:
  %u = alloca %union.anon, align 8
  %d = bitcast %union.anon* %u to double*
  store double %x, double* %d, align 8
  %big_endian = bitcast %union.anon* %u to %struct.anon*
  %exponent = getelementptr inbounds %struct.anon* %big_endian, i32 0, i32 1
  %0 = bitcast [4 x i8]* %exponent to i32*
  %bf.load = load i32* %0, align 4
  %bf.lshr = lshr i32 %bf.load, 20
  %bf.clear = and i32 %bf.lshr, 2047
  %cmp = icmp eq i32 %bf.clear, 2047
  br i1 %cmp, label %land.lhs.true, label %land.end

land.lhs.true:                                    ; preds = %entry
  %big_endian1 = bitcast %union.anon* %u to %struct.anon*
  %mantissa1 = getelementptr inbounds %struct.anon* %big_endian1, i32 0, i32 1
  %1 = bitcast [4 x i8]* %mantissa1 to i32*
  %bf.load2 = load i32* %1, align 4
  %bf.clear3 = and i32 %bf.load2, 1048575
  %cmp4 = icmp eq i32 %bf.clear3, 0
  br i1 %cmp4, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %land.lhs.true
  %big_endian5 = bitcast %union.anon* %u to %struct.anon*
  %mantissa2 = getelementptr inbounds %struct.anon* %big_endian5, i32 0, i32 0
  %2 = load i32* %mantissa2, align 4
  %cmp6 = icmp eq i32 %2, 0
  br label %land.end

land.end:                                         ; preds = %land.rhs, %land.lhs.true, %entry
  %3 = phi i1 [ false, %land.lhs.true ], [ false, %entry ], [ %cmp6, %land.rhs ]
  %land.ext = zext i1 %3 to i32
  ret i32 %land.ext
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %call = call i32 @target_isinf(double 1.234420e+03)
  %call1 = call i32 @target_isinf(double 0x7FF0000000000000)
  %call2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([7 x i8]* @.str, i32 0, i32 0), i32 %call, i32 %call1)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
