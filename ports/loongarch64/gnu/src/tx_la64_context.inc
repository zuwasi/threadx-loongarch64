/* Shared LoongArch64 context frame operations.
 *
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 */

.macro tx_la64_save_gprs
    st.d    $r1,  $sp, 8
    st.d    $r2,  $sp, 16
    st.d    $r4,  $sp, 32
    st.d    $r5,  $sp, 40
    st.d    $r6,  $sp, 48
    st.d    $r7,  $sp, 56
    st.d    $r8,  $sp, 64
    st.d    $r9,  $sp, 72
    st.d    $r10, $sp, 80
    st.d    $r11, $sp, 88
    st.d    $r12, $sp, 96
    st.d    $r13, $sp, 104
    st.d    $r14, $sp, 112
    st.d    $r15, $sp, 120
    st.d    $r16, $sp, 128
    st.d    $r17, $sp, 136
    st.d    $r18, $sp, 144
    st.d    $r19, $sp, 152
    st.d    $r20, $sp, 160
    st.d    $r21, $sp, 168
    st.d    $r22, $sp, 176
    st.d    $r23, $sp, 184
    st.d    $r24, $sp, 192
    st.d    $r25, $sp, 200
    st.d    $r26, $sp, 208
    st.d    $r27, $sp, 216
    st.d    $r28, $sp, 224
    st.d    $r29, $sp, 232
    st.d    $r30, $sp, 240
    st.d    $r31, $sp, 248
    addi.d  $r12, $sp, TX_LA64_CONTEXT_FRAME_SIZE
    st.d    $r12, $sp, 24
.endm

.macro tx_la64_save_fprs
    fst.d   $f0,  $sp, 272
    fst.d   $f1,  $sp, 280
    fst.d   $f2,  $sp, 288
    fst.d   $f3,  $sp, 296
    fst.d   $f4,  $sp, 304
    fst.d   $f5,  $sp, 312
    fst.d   $f6,  $sp, 320
    fst.d   $f7,  $sp, 328
    fst.d   $f8,  $sp, 336
    fst.d   $f9,  $sp, 344
    fst.d   $f10, $sp, 352
    fst.d   $f11, $sp, 360
    fst.d   $f12, $sp, 368
    fst.d   $f13, $sp, 376
    fst.d   $f14, $sp, 384
    fst.d   $f15, $sp, 392
    fst.d   $f16, $sp, 400
    fst.d   $f17, $sp, 408
    fst.d   $f18, $sp, 416
    fst.d   $f19, $sp, 424
    fst.d   $f20, $sp, 432
    fst.d   $f21, $sp, 440
    fst.d   $f22, $sp, 448
    fst.d   $f23, $sp, 456
    fst.d   $f24, $sp, 464
    fst.d   $f25, $sp, 472
    fst.d   $f26, $sp, 480
    fst.d   $f27, $sp, 488
    fst.d   $f28, $sp, 496
    fst.d   $f29, $sp, 504
    fst.d   $f30, $sp, 512
    fst.d   $f31, $sp, 520
    movfcsr2gr $r12, $fcsr0
    st.d    $r12, $sp, 528
    movcf2gr $r12, $fcc0
    st.d    $r12, $sp, 536
    movcf2gr $r12, $fcc1
    st.d    $r12, $sp, 544
    movcf2gr $r12, $fcc2
    st.d    $r12, $sp, 552
    movcf2gr $r12, $fcc3
    st.d    $r12, $sp, 560
    movcf2gr $r12, $fcc4
    st.d    $r12, $sp, 568
    movcf2gr $r12, $fcc5
    st.d    $r12, $sp, 576
    movcf2gr $r12, $fcc6
    st.d    $r12, $sp, 584
    movcf2gr $r12, $fcc7
    st.d    $r12, $sp, 592
.endm

.macro tx_la64_restore_fprs
    fld.d   $f0,  $r21, 272
    fld.d   $f1,  $r21, 280
    fld.d   $f2,  $r21, 288
    fld.d   $f3,  $r21, 296
    fld.d   $f4,  $r21, 304
    fld.d   $f5,  $r21, 312
    fld.d   $f6,  $r21, 320
    fld.d   $f7,  $r21, 328
    fld.d   $f8,  $r21, 336
    fld.d   $f9,  $r21, 344
    fld.d   $f10, $r21, 352
    fld.d   $f11, $r21, 360
    fld.d   $f12, $r21, 368
    fld.d   $f13, $r21, 376
    fld.d   $f14, $r21, 384
    fld.d   $f15, $r21, 392
    fld.d   $f16, $r21, 400
    fld.d   $f17, $r21, 408
    fld.d   $f18, $r21, 416
    fld.d   $f19, $r21, 424
    fld.d   $f20, $r21, 432
    fld.d   $f21, $r21, 440
    fld.d   $f22, $r21, 448
    fld.d   $f23, $r21, 456
    fld.d   $f24, $r21, 464
    fld.d   $f25, $r21, 472
    fld.d   $f26, $r21, 480
    fld.d   $f27, $r21, 488
    fld.d   $f28, $r21, 496
    fld.d   $f29, $r21, 504
    fld.d   $f30, $r21, 512
    fld.d   $f31, $r21, 520
    ld.d    $r12, $r21, 528
    movgr2fcsr $fcsr0, $r12
    ld.d    $r12, $r21, 536
    movgr2cf $fcc0, $r12
    ld.d    $r12, $r21, 544
    movgr2cf $fcc1, $r12
    ld.d    $r12, $r21, 552
    movgr2cf $fcc2, $r12
    ld.d    $r12, $r21, 560
    movgr2cf $fcc3, $r12
    ld.d    $r12, $r21, 568
    movgr2cf $fcc4, $r12
    ld.d    $r12, $r21, 576
    movgr2cf $fcc5, $r12
    ld.d    $r12, $r21, 584
    movgr2cf $fcc6, $r12
    ld.d    $r12, $r21, 592
    movgr2cf $fcc7, $r12
.endm

.macro tx_la64_restore_gprs
    ld.d    $r1,  $r21, 8
    ld.d    $r2,  $r21, 16
    ld.d    $r4,  $r21, 32
    ld.d    $r5,  $r21, 40
    ld.d    $r6,  $r21, 48
    ld.d    $r7,  $r21, 56
    ld.d    $r8,  $r21, 64
    ld.d    $r9,  $r21, 72
    ld.d    $r10, $r21, 80
    ld.d    $r11, $r21, 88
    ld.d    $r12, $r21, 96
    ld.d    $r13, $r21, 104
    ld.d    $r14, $r21, 112
    ld.d    $r15, $r21, 120
    ld.d    $r16, $r21, 128
    ld.d    $r17, $r21, 136
    ld.d    $r18, $r21, 144
    ld.d    $r19, $r21, 152
    ld.d    $r20, $r21, 160
    ld.d    $r22, $r21, 176
    ld.d    $r23, $r21, 184
    ld.d    $r24, $r21, 192
    ld.d    $r25, $r21, 200
    ld.d    $r26, $r21, 208
    ld.d    $r27, $r21, 216
    ld.d    $r28, $r21, 224
    ld.d    $r29, $r21, 232
    ld.d    $r30, $r21, 240
    ld.d    $r31, $r21, 248
    ld.d    $r3,  $r21, 24
    ld.d    $r21, $r21, 168
.endm
