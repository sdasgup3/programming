; ModuleID = 'uint64_to_float-scalarrepl-sdasgup3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.float_bits = type { i32 }

@.str = private unnamed_addr constant [28 x i8] c"Error detected @ 0x%016llx\0A\00", align 1
@.str1 = private unnamed_addr constant [31 x i8] c"\09Expected result: %a (0x%08x)\0A\00", align 1
@.str2 = private unnamed_addr constant [31 x i8] c"\09Observed result: %a (0x%08x)\0A\00", align 1
@main.roundingModes = internal constant [4 x i32] zeroinitializer, align 16
@.str3 = private unnamed_addr constant [11 x i8] c"to nearest\00", align 1
@.str4 = private unnamed_addr constant [5 x i8] c"down\00", align 1
@.str5 = private unnamed_addr constant [3 x i8] c"up\00", align 1
@.str6 = private unnamed_addr constant [13 x i8] c"towards zero\00", align 1
@main.modeNames = private unnamed_addr constant [4 x i8*] [i8* getelementptr inbounds ([11 x i8]* @.str3, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8]* @.str4, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8]* @.str5, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8]* @.str6, i32 0, i32 0)], align 16
@.str7 = private unnamed_addr constant [55 x i8] c"Testing uint64_t --> float conversions in round %s...\0A\00", align 1
@.str8 = private unnamed_addr constant [19 x i8] c"Finished Testing.\0A\00", align 1

; Function Attrs: nounwind uwtable
define float @floatundisf(i64 %a) #0 {
entry:
  %fb = alloca %union.float_bits, align 4
  %cmp = icmp eq i64 %a, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %return

if.end:                                           ; preds = %entry
  %0 = call i64 @llvm.ctlz.i64(i64 %a, i1 true)
  %cast = trunc i64 %0 to i32
  %sub = sub i32 64, %cast
  %sub1 = sub nsw i32 %sub, 1
  %cmp2 = icmp sgt i32 %sub, 24
  br i1 %cmp2, label %if.then3, label %if.else

if.then3:                                         ; preds = %if.end
  switch i32 %sub, label %sw.default [
    i32 25, label %sw.bb
    i32 26, label %sw.bb4
  ]

sw.bb:                                            ; preds = %if.then3
  %shl = shl i64 %a, 1
  br label %sw.epilog

sw.bb4:                                           ; preds = %if.then3
  br label %sw.epilog

sw.default:                                       ; preds = %if.then3
  %sub5 = sub nsw i32 %sub, 26
  %sh_prom = zext i32 %sub5 to i64
  %shr = lshr i64 %a, %sh_prom
  %sub6 = sub i32 90, %sub
  %sh_prom7 = zext i32 %sub6 to i64
  %shr8 = lshr i64 -1, %sh_prom7
  %and = and i64 %a, %shr8
  %cmp9 = icmp ne i64 %and, 0
  %conv = zext i1 %cmp9 to i32
  %conv10 = sext i32 %conv to i64
  %or = or i64 %shr, %conv10
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %sw.bb4, %sw.bb
  %a.addr.0 = phi i64 [ %or, %sw.default ], [ %a, %sw.bb4 ], [ %shl, %sw.bb ]
  %and11 = and i64 %a.addr.0, 4
  %cmp12 = icmp ne i64 %and11, 0
  %conv13 = zext i1 %cmp12 to i32
  %conv14 = sext i32 %conv13 to i64
  %or15 = or i64 %a.addr.0, %conv14
  %inc = add i64 %or15, 1
  %shr16 = lshr i64 %inc, 2
  %and17 = and i64 %shr16, 16777216
  %tobool = icmp ne i64 %and17, 0
  br i1 %tobool, label %if.then18, label %if.end21

if.then18:                                        ; preds = %sw.epilog
  %shr19 = lshr i64 %shr16, 1
  %inc20 = add nsw i32 %sub1, 1
  br label %if.end21

if.end21:                                         ; preds = %if.then18, %sw.epilog
  %e.0 = phi i32 [ %inc20, %if.then18 ], [ %sub1, %sw.epilog ]
  %a.addr.1 = phi i64 [ %shr19, %if.then18 ], [ %shr16, %sw.epilog ]
  br label %if.end25

if.else:                                          ; preds = %if.end
  %sub22 = sub nsw i32 24, %sub
  %sh_prom23 = zext i32 %sub22 to i64
  %shl24 = shl i64 %a, %sh_prom23
  br label %if.end25

if.end25:                                         ; preds = %if.else, %if.end21
  %e.1 = phi i32 [ %e.0, %if.end21 ], [ %sub1, %if.else ]
  %a.addr.2 = phi i64 [ %a.addr.1, %if.end21 ], [ %shl24, %if.else ]
  %add = add nsw i32 %e.1, 127
  %shl26 = shl i32 %add, 23
  %conv27 = trunc i64 %a.addr.2 to i32
  %and28 = and i32 %conv27, 8388607
  %or29 = or i32 %shl26, %and28
  %u = bitcast %union.float_bits* %fb to i32*
  store i32 %or29, i32* %u, align 4
  %f = bitcast %union.float_bits* %fb to float*
  %1 = load float* %f, align 4
  br label %return

return:                                           ; preds = %if.end25, %if.then
  %retval.0 = phi float [ 0.000000e+00, %if.then ], [ %1, %if.end25 ]
  ret float %retval.0
}

; Function Attrs: nounwind readnone
declare i64 @llvm.ctlz.i64(i64, i1) #1

; Function Attrs: nounwind uwtable
define void @test(i64 %x) #0 {
entry:
  %expected = alloca %union.float_bits, align 4
  %observed = alloca %union.float_bits, align 4
  %f = bitcast %union.float_bits* %expected to float*
  %call = call float @floatundisf(i64 %x)
  store float %call, float* %f, align 4
  %f1 = bitcast %union.float_bits* %observed to float*
  %conv = uitofp i64 %x to float
  store float %conv, float* %f1, align 4
  %u = bitcast %union.float_bits* %expected to i32*
  %0 = load i32* %u, align 4
  %u2 = bitcast %union.float_bits* %observed to i32*
  %1 = load i32* %u2, align 4
  %cmp = icmp ne i32 %0, %1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call4 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([28 x i8]* @.str, i32 0, i32 0), i64 %x)
  %f5 = bitcast %union.float_bits* %expected to float*
  %2 = load float* %f5, align 4
  %conv6 = fpext float %2 to double
  %u7 = bitcast %union.float_bits* %expected to i32*
  %3 = load i32* %u7, align 4
  %call8 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([31 x i8]* @.str1, i32 0, i32 0), double %conv6, i32 %3)
  %f9 = bitcast %union.float_bits* %observed to float*
  %4 = load float* %f9, align 4
  %conv10 = fpext float %4 to double
  %u11 = bitcast %union.float_bits* %observed to i32*
  %5 = load i32* %u11, align 4
  %call12 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([31 x i8]* @.str2, i32 0, i32 0), double %conv10, i32 %5)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret void
}

declare i32 @printf(i8*, ...) #2

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %modeNames = alloca [4 x i8*], align 16
  %0 = bitcast [4 x i8*]* %modeNames to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([4 x i8*]* @main.modeNames to i8*), i64 32, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc284, %entry
  %m.0 = phi i32 [ 0, %entry ], [ %inc285, %for.inc284 ]
  %cmp = icmp slt i32 %m.0, 4
  br i1 %cmp, label %for.body, label %for.end286

for.body:                                         ; preds = %for.cond
  %1 = load i32* getelementptr inbounds ([4 x i32]* @main.roundingModes, i32 0, i64 0), align 4
  %call = call i32 @fesetround(i32 %1) #3
  %idxprom = sext i32 %m.0 to i64
  %arrayidx = getelementptr inbounds [4 x i8*]* %modeNames, i32 0, i64 %idxprom
  %2 = load i8** %arrayidx, align 8
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([55 x i8]* @.str7, i32 0, i32 0), i8* %2)
  call void @test(i64 0)
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc281, %for.body
  %i.0 = phi i32 [ 0, %for.body ], [ %inc282, %for.inc281 ]
  %cmp3 = icmp slt i32 %i.0, 64
  br i1 %cmp3, label %for.body4, label %for.end283

for.body4:                                        ; preds = %for.cond2
  %sh_prom = zext i32 %i.0 to i64
  %shl = shl i64 1, %sh_prom
  call void @test(i64 %shl)
  %sh_prom5 = zext i32 %i.0 to i64
  %shl6 = shl i64 -1, %sh_prom5
  call void @test(i64 %shl6)
  br label %for.cond7

for.cond7:                                        ; preds = %for.inc278, %for.body4
  %j.0 = phi i32 [ 0, %for.body4 ], [ %inc279, %for.inc278 ]
  %cmp8 = icmp slt i32 %j.0, %i.0
  br i1 %cmp8, label %for.body9, label %for.end280

for.body9:                                        ; preds = %for.cond7
  %sh_prom10 = zext i32 %i.0 to i64
  %shl11 = shl i64 1, %sh_prom10
  %sh_prom12 = zext i32 %j.0 to i64
  %shl13 = shl i64 1, %sh_prom12
  %add = add i64 %shl11, %shl13
  call void @test(i64 %add)
  %sh_prom14 = zext i32 %i.0 to i64
  %shl15 = shl i64 1, %sh_prom14
  %sh_prom16 = zext i32 %j.0 to i64
  %shl17 = shl i64 -1, %sh_prom16
  %add18 = add i64 %shl15, %shl17
  call void @test(i64 %add18)
  %sh_prom19 = zext i32 %i.0 to i64
  %shl20 = shl i64 -1, %sh_prom19
  %sh_prom21 = zext i32 %j.0 to i64
  %shl22 = shl i64 1, %sh_prom21
  %add23 = add i64 %shl20, %shl22
  call void @test(i64 %add23)
  %sh_prom24 = zext i32 %i.0 to i64
  %shl25 = shl i64 -1, %sh_prom24
  %sh_prom26 = zext i32 %j.0 to i64
  %shl27 = shl i64 -1, %sh_prom26
  %add28 = add i64 %shl25, %shl27
  call void @test(i64 %add28)
  br label %for.cond29

for.cond29:                                       ; preds = %for.inc275, %for.body9
  %k.0 = phi i32 [ 0, %for.body9 ], [ %inc276, %for.inc275 ]
  %cmp30 = icmp slt i32 %k.0, %j.0
  br i1 %cmp30, label %for.body31, label %for.end277

for.body31:                                       ; preds = %for.cond29
  %sh_prom32 = zext i32 %i.0 to i64
  %shl33 = shl i64 1, %sh_prom32
  %sh_prom34 = zext i32 %j.0 to i64
  %shl35 = shl i64 1, %sh_prom34
  %add36 = add i64 %shl33, %shl35
  %sh_prom37 = zext i32 %k.0 to i64
  %shl38 = shl i64 1, %sh_prom37
  %add39 = add i64 %add36, %shl38
  call void @test(i64 %add39)
  %sh_prom40 = zext i32 %i.0 to i64
  %shl41 = shl i64 1, %sh_prom40
  %sh_prom42 = zext i32 %j.0 to i64
  %shl43 = shl i64 1, %sh_prom42
  %add44 = add i64 %shl41, %shl43
  %sh_prom45 = zext i32 %k.0 to i64
  %shl46 = shl i64 -1, %sh_prom45
  %add47 = add i64 %add44, %shl46
  call void @test(i64 %add47)
  %sh_prom48 = zext i32 %i.0 to i64
  %shl49 = shl i64 1, %sh_prom48
  %sh_prom50 = zext i32 %j.0 to i64
  %shl51 = shl i64 -1, %sh_prom50
  %add52 = add i64 %shl49, %shl51
  %sh_prom53 = zext i32 %k.0 to i64
  %shl54 = shl i64 1, %sh_prom53
  %add55 = add i64 %add52, %shl54
  call void @test(i64 %add55)
  %sh_prom56 = zext i32 %i.0 to i64
  %shl57 = shl i64 1, %sh_prom56
  %sh_prom58 = zext i32 %j.0 to i64
  %shl59 = shl i64 -1, %sh_prom58
  %add60 = add i64 %shl57, %shl59
  %sh_prom61 = zext i32 %k.0 to i64
  %shl62 = shl i64 -1, %sh_prom61
  %add63 = add i64 %add60, %shl62
  call void @test(i64 %add63)
  %sh_prom64 = zext i32 %i.0 to i64
  %shl65 = shl i64 -1, %sh_prom64
  %sh_prom66 = zext i32 %j.0 to i64
  %shl67 = shl i64 1, %sh_prom66
  %add68 = add i64 %shl65, %shl67
  %sh_prom69 = zext i32 %k.0 to i64
  %shl70 = shl i64 1, %sh_prom69
  %add71 = add i64 %add68, %shl70
  call void @test(i64 %add71)
  %sh_prom72 = zext i32 %i.0 to i64
  %shl73 = shl i64 -1, %sh_prom72
  %sh_prom74 = zext i32 %j.0 to i64
  %shl75 = shl i64 1, %sh_prom74
  %add76 = add i64 %shl73, %shl75
  %sh_prom77 = zext i32 %k.0 to i64
  %shl78 = shl i64 -1, %sh_prom77
  %add79 = add i64 %add76, %shl78
  call void @test(i64 %add79)
  %sh_prom80 = zext i32 %i.0 to i64
  %shl81 = shl i64 -1, %sh_prom80
  %sh_prom82 = zext i32 %j.0 to i64
  %shl83 = shl i64 -1, %sh_prom82
  %add84 = add i64 %shl81, %shl83
  %sh_prom85 = zext i32 %k.0 to i64
  %shl86 = shl i64 1, %sh_prom85
  %add87 = add i64 %add84, %shl86
  call void @test(i64 %add87)
  %sh_prom88 = zext i32 %i.0 to i64
  %shl89 = shl i64 -1, %sh_prom88
  %sh_prom90 = zext i32 %j.0 to i64
  %shl91 = shl i64 -1, %sh_prom90
  %add92 = add i64 %shl89, %shl91
  %sh_prom93 = zext i32 %k.0 to i64
  %shl94 = shl i64 -1, %sh_prom93
  %add95 = add i64 %add92, %shl94
  call void @test(i64 %add95)
  br label %for.cond96

for.cond96:                                       ; preds = %for.inc, %for.body31
  %l.0 = phi i32 [ 0, %for.body31 ], [ %inc, %for.inc ]
  %cmp97 = icmp slt i32 %l.0, %k.0
  br i1 %cmp97, label %for.body98, label %for.end

for.body98:                                       ; preds = %for.cond96
  %sh_prom99 = zext i32 %i.0 to i64
  %shl100 = shl i64 1, %sh_prom99
  %sh_prom101 = zext i32 %j.0 to i64
  %shl102 = shl i64 1, %sh_prom101
  %add103 = add i64 %shl100, %shl102
  %sh_prom104 = zext i32 %k.0 to i64
  %shl105 = shl i64 1, %sh_prom104
  %add106 = add i64 %add103, %shl105
  %sh_prom107 = zext i32 %l.0 to i64
  %shl108 = shl i64 1, %sh_prom107
  %add109 = add i64 %add106, %shl108
  call void @test(i64 %add109)
  %sh_prom110 = zext i32 %i.0 to i64
  %shl111 = shl i64 1, %sh_prom110
  %sh_prom112 = zext i32 %j.0 to i64
  %shl113 = shl i64 1, %sh_prom112
  %add114 = add i64 %shl111, %shl113
  %sh_prom115 = zext i32 %k.0 to i64
  %shl116 = shl i64 1, %sh_prom115
  %add117 = add i64 %add114, %shl116
  %sh_prom118 = zext i32 %l.0 to i64
  %shl119 = shl i64 -1, %sh_prom118
  %add120 = add i64 %add117, %shl119
  call void @test(i64 %add120)
  %sh_prom121 = zext i32 %i.0 to i64
  %shl122 = shl i64 1, %sh_prom121
  %sh_prom123 = zext i32 %j.0 to i64
  %shl124 = shl i64 1, %sh_prom123
  %add125 = add i64 %shl122, %shl124
  %sh_prom126 = zext i32 %k.0 to i64
  %shl127 = shl i64 -1, %sh_prom126
  %add128 = add i64 %add125, %shl127
  %sh_prom129 = zext i32 %l.0 to i64
  %shl130 = shl i64 1, %sh_prom129
  %add131 = add i64 %add128, %shl130
  call void @test(i64 %add131)
  %sh_prom132 = zext i32 %i.0 to i64
  %shl133 = shl i64 1, %sh_prom132
  %sh_prom134 = zext i32 %j.0 to i64
  %shl135 = shl i64 1, %sh_prom134
  %add136 = add i64 %shl133, %shl135
  %sh_prom137 = zext i32 %k.0 to i64
  %shl138 = shl i64 -1, %sh_prom137
  %add139 = add i64 %add136, %shl138
  %sh_prom140 = zext i32 %l.0 to i64
  %shl141 = shl i64 -1, %sh_prom140
  %add142 = add i64 %add139, %shl141
  call void @test(i64 %add142)
  %sh_prom143 = zext i32 %i.0 to i64
  %shl144 = shl i64 1, %sh_prom143
  %sh_prom145 = zext i32 %j.0 to i64
  %shl146 = shl i64 -1, %sh_prom145
  %add147 = add i64 %shl144, %shl146
  %sh_prom148 = zext i32 %k.0 to i64
  %shl149 = shl i64 1, %sh_prom148
  %add150 = add i64 %add147, %shl149
  %sh_prom151 = zext i32 %l.0 to i64
  %shl152 = shl i64 1, %sh_prom151
  %add153 = add i64 %add150, %shl152
  call void @test(i64 %add153)
  %sh_prom154 = zext i32 %i.0 to i64
  %shl155 = shl i64 1, %sh_prom154
  %sh_prom156 = zext i32 %j.0 to i64
  %shl157 = shl i64 -1, %sh_prom156
  %add158 = add i64 %shl155, %shl157
  %sh_prom159 = zext i32 %k.0 to i64
  %shl160 = shl i64 1, %sh_prom159
  %add161 = add i64 %add158, %shl160
  %sh_prom162 = zext i32 %l.0 to i64
  %shl163 = shl i64 -1, %sh_prom162
  %add164 = add i64 %add161, %shl163
  call void @test(i64 %add164)
  %sh_prom165 = zext i32 %i.0 to i64
  %shl166 = shl i64 1, %sh_prom165
  %sh_prom167 = zext i32 %j.0 to i64
  %shl168 = shl i64 -1, %sh_prom167
  %add169 = add i64 %shl166, %shl168
  %sh_prom170 = zext i32 %k.0 to i64
  %shl171 = shl i64 -1, %sh_prom170
  %add172 = add i64 %add169, %shl171
  %sh_prom173 = zext i32 %l.0 to i64
  %shl174 = shl i64 1, %sh_prom173
  %add175 = add i64 %add172, %shl174
  call void @test(i64 %add175)
  %sh_prom176 = zext i32 %i.0 to i64
  %shl177 = shl i64 1, %sh_prom176
  %sh_prom178 = zext i32 %j.0 to i64
  %shl179 = shl i64 -1, %sh_prom178
  %add180 = add i64 %shl177, %shl179
  %sh_prom181 = zext i32 %k.0 to i64
  %shl182 = shl i64 -1, %sh_prom181
  %add183 = add i64 %add180, %shl182
  %sh_prom184 = zext i32 %l.0 to i64
  %shl185 = shl i64 -1, %sh_prom184
  %add186 = add i64 %add183, %shl185
  call void @test(i64 %add186)
  %sh_prom187 = zext i32 %i.0 to i64
  %shl188 = shl i64 -1, %sh_prom187
  %sh_prom189 = zext i32 %j.0 to i64
  %shl190 = shl i64 1, %sh_prom189
  %add191 = add i64 %shl188, %shl190
  %sh_prom192 = zext i32 %k.0 to i64
  %shl193 = shl i64 1, %sh_prom192
  %add194 = add i64 %add191, %shl193
  %sh_prom195 = zext i32 %l.0 to i64
  %shl196 = shl i64 1, %sh_prom195
  %add197 = add i64 %add194, %shl196
  call void @test(i64 %add197)
  %sh_prom198 = zext i32 %i.0 to i64
  %shl199 = shl i64 -1, %sh_prom198
  %sh_prom200 = zext i32 %j.0 to i64
  %shl201 = shl i64 1, %sh_prom200
  %add202 = add i64 %shl199, %shl201
  %sh_prom203 = zext i32 %k.0 to i64
  %shl204 = shl i64 1, %sh_prom203
  %add205 = add i64 %add202, %shl204
  %sh_prom206 = zext i32 %l.0 to i64
  %shl207 = shl i64 -1, %sh_prom206
  %add208 = add i64 %add205, %shl207
  call void @test(i64 %add208)
  %sh_prom209 = zext i32 %i.0 to i64
  %shl210 = shl i64 -1, %sh_prom209
  %sh_prom211 = zext i32 %j.0 to i64
  %shl212 = shl i64 1, %sh_prom211
  %add213 = add i64 %shl210, %shl212
  %sh_prom214 = zext i32 %k.0 to i64
  %shl215 = shl i64 -1, %sh_prom214
  %add216 = add i64 %add213, %shl215
  %sh_prom217 = zext i32 %l.0 to i64
  %shl218 = shl i64 1, %sh_prom217
  %add219 = add i64 %add216, %shl218
  call void @test(i64 %add219)
  %sh_prom220 = zext i32 %i.0 to i64
  %shl221 = shl i64 -1, %sh_prom220
  %sh_prom222 = zext i32 %j.0 to i64
  %shl223 = shl i64 1, %sh_prom222
  %add224 = add i64 %shl221, %shl223
  %sh_prom225 = zext i32 %k.0 to i64
  %shl226 = shl i64 -1, %sh_prom225
  %add227 = add i64 %add224, %shl226
  %sh_prom228 = zext i32 %l.0 to i64
  %shl229 = shl i64 -1, %sh_prom228
  %add230 = add i64 %add227, %shl229
  call void @test(i64 %add230)
  %sh_prom231 = zext i32 %i.0 to i64
  %shl232 = shl i64 -1, %sh_prom231
  %sh_prom233 = zext i32 %j.0 to i64
  %shl234 = shl i64 -1, %sh_prom233
  %add235 = add i64 %shl232, %shl234
  %sh_prom236 = zext i32 %k.0 to i64
  %shl237 = shl i64 1, %sh_prom236
  %add238 = add i64 %add235, %shl237
  %sh_prom239 = zext i32 %l.0 to i64
  %shl240 = shl i64 1, %sh_prom239
  %add241 = add i64 %add238, %shl240
  call void @test(i64 %add241)
  %sh_prom242 = zext i32 %i.0 to i64
  %shl243 = shl i64 -1, %sh_prom242
  %sh_prom244 = zext i32 %j.0 to i64
  %shl245 = shl i64 -1, %sh_prom244
  %add246 = add i64 %shl243, %shl245
  %sh_prom247 = zext i32 %k.0 to i64
  %shl248 = shl i64 1, %sh_prom247
  %add249 = add i64 %add246, %shl248
  %sh_prom250 = zext i32 %l.0 to i64
  %shl251 = shl i64 -1, %sh_prom250
  %add252 = add i64 %add249, %shl251
  call void @test(i64 %add252)
  %sh_prom253 = zext i32 %i.0 to i64
  %shl254 = shl i64 -1, %sh_prom253
  %sh_prom255 = zext i32 %j.0 to i64
  %shl256 = shl i64 -1, %sh_prom255
  %add257 = add i64 %shl254, %shl256
  %sh_prom258 = zext i32 %k.0 to i64
  %shl259 = shl i64 -1, %sh_prom258
  %add260 = add i64 %add257, %shl259
  %sh_prom261 = zext i32 %l.0 to i64
  %shl262 = shl i64 1, %sh_prom261
  %add263 = add i64 %add260, %shl262
  call void @test(i64 %add263)
  %sh_prom264 = zext i32 %i.0 to i64
  %shl265 = shl i64 -1, %sh_prom264
  %sh_prom266 = zext i32 %j.0 to i64
  %shl267 = shl i64 -1, %sh_prom266
  %add268 = add i64 %shl265, %shl267
  %sh_prom269 = zext i32 %k.0 to i64
  %shl270 = shl i64 -1, %sh_prom269
  %add271 = add i64 %add268, %shl270
  %sh_prom272 = zext i32 %l.0 to i64
  %shl273 = shl i64 -1, %sh_prom272
  %add274 = add i64 %add271, %shl273
  call void @test(i64 %add274)
  br label %for.inc

for.inc:                                          ; preds = %for.body98
  %inc = add nsw i32 %l.0, 1
  br label %for.cond96

for.end:                                          ; preds = %for.cond96
  br label %for.inc275

for.inc275:                                       ; preds = %for.end
  %inc276 = add nsw i32 %k.0, 1
  br label %for.cond29

for.end277:                                       ; preds = %for.cond29
  br label %for.inc278

for.inc278:                                       ; preds = %for.end277
  %inc279 = add nsw i32 %j.0, 1
  br label %for.cond7

for.end280:                                       ; preds = %for.cond7
  br label %for.inc281

for.inc281:                                       ; preds = %for.end280
  %inc282 = add nsw i32 %i.0, 1
  br label %for.cond2

for.end283:                                       ; preds = %for.cond2
  br label %for.inc284

for.inc284:                                       ; preds = %for.end283
  %inc285 = add nsw i32 %m.0, 1
  br label %for.cond

for.end286:                                       ; preds = %for.cond
  %call287 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([19 x i8]* @.str8, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #3

; Function Attrs: nounwind
declare i32 @fesetround(i32) #4

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 201464)"}
