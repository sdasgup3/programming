; ModuleID = 'xT_1-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.fruit = type { i32, i32 }

@Red = global i32 0, align 4
@Yellow = global i32 1, align 4
@Orange = global i32 2, align 4

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %snack = alloca %struct.fruit, align 4
  switch i32 2, label %sw.epilog [
    i32 0, label %sw.bb
    i32 1, label %sw.bb2
    i32 2, label %sw.bb3
  ]

sw.bb:                                            ; preds = %entry
  %0 = load i32* @Red, align 4
  br label %return

sw.bb2:                                           ; preds = %entry
  %1 = load i32* @Yellow, align 4
  br label %return

sw.bb3:                                           ; preds = %entry
  %2 = load i32* @Orange, align 4
  br label %return

sw.epilog:                                        ; preds = %entry
  br label %return

return:                                           ; preds = %sw.epilog, %sw.bb3, %sw.bb2, %sw.bb
  %retval.0 = phi i32 [ 0, %sw.epilog ], [ %2, %sw.bb3 ], [ %1, %sw.bb2 ], [ %0, %sw.bb ]
  ret i32 %retval.0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
