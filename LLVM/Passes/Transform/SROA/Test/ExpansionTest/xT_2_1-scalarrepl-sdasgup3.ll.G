; ModuleID = 'xT_2_1-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.test_struct = type { i32, %struct.test_struct* }

@head = common global %struct.test_struct zeroinitializer, align 8
@curr = common global %struct.test_struct zeroinitializer, align 8

; Function Attrs: nounwind uwtable
define i32 @create_list(i32 %val) #0 {
entry:
  %ptr = alloca %struct.test_struct, align 8
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
