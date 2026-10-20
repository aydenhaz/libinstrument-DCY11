	.syntax	unified
	.eabi_attribute	67, "2.09"	@ Tag_conformance
	.eabi_attribute	6, 10	@ Tag_CPU_arch
	.eabi_attribute	7, 65	@ Tag_CPU_arch_profile
	.eabi_attribute	8, 1	@ Tag_ARM_ISA_use
	.eabi_attribute	9, 2	@ Tag_THUMB_ISA_use
	.fpu	vfpv3
	.eabi_attribute	34, 1	@ Tag_CPU_unaligned_access
	.eabi_attribute	17, 1	@ Tag_ABI_PCS_GOT_use
	.eabi_attribute	20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute	21, 0	@ Tag_ABI_FP_exceptions
	.eabi_attribute	23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute	24, 1	@ Tag_ABI_align_needed
	.eabi_attribute	25, 1	@ Tag_ABI_align_preserved
	.eabi_attribute	38, 1	@ Tag_ABI_FP_16bit_format
	.eabi_attribute	18, 4	@ Tag_ABI_PCS_wchar_t
	.eabi_attribute	26, 2	@ Tag_ABI_enum_size
	.eabi_attribute	14, 0	@ Tag_ABI_PCS_R9_use
	.file	"cover_rgb_configuration_signature_probe.cpp"
	.text
	.globl	_Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE @ -- Begin function _Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE
	.p2align	2
	.type	_Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE,%function
	.code	32
_Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE: @ @_Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	pop	{r11, lr}
	b	_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE
.Lfunc_end0:
	.size	_Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE, .Lfunc_end0-_Z14configureCoverRKN7IPL_ABI15CoverRgbAverageE
	.fnend
                                        @ -- End function
	.section	.text._ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE,"axG",%progbits,_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE,comdat
	.weak	_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE @ -- Begin function _ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE
	.p2align	2
	.type	_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE,%function
	.code	32
_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE: @ @_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE
.Lfunc_begin0:
	.fnstart
@ %bb.0:
	.save	{r4, r5, r11, lr}
	push	{r4, r5, r11, lr}
	.setfp	r11, sp, #8
	add	r11, sp, #8
	.pad	#16
	sub	sp, sp, #16
	movw	r5, :lower16:.L.str
	ldr	r2, [r0]
	mov	r4, r0
	movt	r5, :upper16:.L.str
	add	r0, sp, #12
	mov	r1, r5
	bl	_ZN3hsl11StringUtils6FormatEPKtz
	ldr	r2, [r4, #4]
.Ltmp0:                                 @ EH_LABEL
	add	r0, sp, #8
	mov	r1, r5
	bl	_ZN3hsl11StringUtils6FormatEPKtz
.Ltmp1:                                 @ EH_LABEL
@ %bb.1:
	ldr	r2, [r4, #8]
.Ltmp3:                                 @ EH_LABEL
	movw	r1, :lower16:.L.str
	add	r0, sp, #4
	movt	r1, :upper16:.L.str
	bl	_ZN3hsl11StringUtils6FormatEPKtz
.Ltmp4:                                 @ EH_LABEL
@ %bb.2:
.Ltmp6:                                 @ EH_LABEL
	mov	r0, #36
	bl	_Znaj
.Ltmp7:                                 @ EH_LABEL
@ %bb.3:
	mov	r4, r0
	mov	r0, #12
	movw	r1, :lower16:.L.str.1
	str	r0, [r4, #4]
	str	r0, [r4]
	add	r0, r4, #8
	movt	r1, :upper16:.L.str.1
	mov	r2, #24
	bl	memcpy
	mov	r0, #0
	str	r4, [sp]
	strh	r0, [r4, #32]
.Ltmp9:                                 @ EH_LABEL
	add	r1, sp, #12
	mov	r0, sp
	bl	_ZN9CAutoUtil16setSMConfigValueERKN4dice8String16ES3_
.Ltmp10:                                @ EH_LABEL
@ %bb.4:
	ldr	r0, [sp]
	cmp	r0, #0
	beq	.LBB1_6
@ %bb.5:
	bl	_ZdaPv
.LBB1_6:
.Ltmp12:                                @ EH_LABEL
	mov	r0, #36
	bl	_Znaj
.Ltmp13:                                @ EH_LABEL
@ %bb.7:
	mov	r4, r0
	mov	r0, #12
	movw	r1, :lower16:.L.str.2
	str	r0, [r4, #4]
	str	r0, [r4]
	add	r0, r4, #8
	movt	r1, :upper16:.L.str.2
	mov	r2, #24
	bl	memcpy
	mov	r0, #0
	str	r4, [sp]
	strh	r0, [r4, #32]
.Ltmp15:                                @ EH_LABEL
	add	r1, sp, #8
	mov	r0, sp
	bl	_ZN9CAutoUtil16setSMConfigValueERKN4dice8String16ES3_
.Ltmp16:                                @ EH_LABEL
@ %bb.8:
	ldr	r0, [sp]
	cmp	r0, #0
	beq	.LBB1_10
@ %bb.9:
	bl	_ZdaPv
.LBB1_10:
.Ltmp18:                                @ EH_LABEL
	mov	r0, #36
	bl	_Znaj
.Ltmp19:                                @ EH_LABEL
@ %bb.11:
	mov	r4, r0
	mov	r0, #12
	movw	r1, :lower16:.L.str.3
	str	r0, [r4, #4]
	str	r0, [r4]
	add	r0, r4, #8
	movt	r1, :upper16:.L.str.3
	mov	r2, #24
	bl	memcpy
	mov	r0, #0
	str	r4, [sp]
	strh	r0, [r4, #32]
.Ltmp21:                                @ EH_LABEL
	add	r1, sp, #4
	mov	r0, sp
	bl	_ZN9CAutoUtil16setSMConfigValueERKN4dice8String16ES3_
.Ltmp22:                                @ EH_LABEL
@ %bb.12:
	ldr	r0, [sp]
	cmp	r0, #0
	beq	.LBB1_14
@ %bb.13:
	bl	_ZdaPv
.LBB1_14:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq	.LBB1_16
@ %bb.15:
	bl	_ZdaPv
.LBB1_16:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	beq	.LBB1_18
@ %bb.17:
	bl	_ZdaPv
.LBB1_18:
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq	.LBB1_20
@ %bb.19:
	bl	_ZdaPv
.LBB1_20:
	sub	sp, r11, #8
	pop	{r4, r5, r11, pc}
.LBB1_21:
.Ltmp23:                                @ EH_LABEL
	b	.LBB1_26
.LBB1_22:
.Ltmp20:                                @ EH_LABEL
	b	.LBB1_29
.LBB1_23:
.Ltmp17:                                @ EH_LABEL
	b	.LBB1_26
.LBB1_24:
.Ltmp14:                                @ EH_LABEL
	b	.LBB1_29
.LBB1_25:
.Ltmp11:                                @ EH_LABEL
.LBB1_26:
	ldr	r0, [sp]
	cmp	r0, #0
	beq	.LBB1_29
@ %bb.27:
	bl	_ZdaPv
	b	.LBB1_29
.LBB1_28:
.Ltmp8:                                 @ EH_LABEL
.LBB1_29:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq	.LBB1_32
@ %bb.30:
	bl	_ZdaPv
	b	.LBB1_32
.LBB1_31:
.Ltmp5:                                 @ EH_LABEL
.LBB1_32:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	beq	.LBB1_35
@ %bb.33:
	bl	_ZdaPv
	b	.LBB1_35
.LBB1_34:
.Ltmp2:                                 @ EH_LABEL
.LBB1_35:
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq	.LBB1_37
@ %bb.36:
	bl	_ZdaPv
.LBB1_37:
	bl	__cxa_end_cleanup
.Lfunc_end1:
	.size	_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE, .Lfunc_end1-_ZN7IPL_ABI26writeCoverRgbConfigurationERKNS_15CoverRgbAverageE
	.personality __gxx_personality_v0
	.handlerdata
	.p2align	2
GCC_except_table1:
.Lexception0:
	.byte	255                             @ @LPStart Encoding = omit
	.byte	255                             @ @TType Encoding = omit
	.byte	1                               @ Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    @ >> Call Site 1 <<
	.uleb128 .Ltmp0-.Lfunc_begin0           @   Call between .Lfunc_begin0 and .Ltmp0
	.byte	0                               @     has no landing pad
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp0-.Lfunc_begin0           @ >> Call Site 2 <<
	.uleb128 .Ltmp1-.Ltmp0                  @   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0           @     jumps to .Ltmp2
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp3-.Lfunc_begin0           @ >> Call Site 3 <<
	.uleb128 .Ltmp4-.Ltmp3                  @   Call between .Ltmp3 and .Ltmp4
	.uleb128 .Ltmp5-.Lfunc_begin0           @     jumps to .Ltmp5
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp6-.Lfunc_begin0           @ >> Call Site 4 <<
	.uleb128 .Ltmp7-.Ltmp6                  @   Call between .Ltmp6 and .Ltmp7
	.uleb128 .Ltmp8-.Lfunc_begin0           @     jumps to .Ltmp8
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp7-.Lfunc_begin0           @ >> Call Site 5 <<
	.uleb128 .Ltmp9-.Ltmp7                  @   Call between .Ltmp7 and .Ltmp9
	.byte	0                               @     has no landing pad
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp9-.Lfunc_begin0           @ >> Call Site 6 <<
	.uleb128 .Ltmp10-.Ltmp9                 @   Call between .Ltmp9 and .Ltmp10
	.uleb128 .Ltmp11-.Lfunc_begin0          @     jumps to .Ltmp11
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp12-.Lfunc_begin0          @ >> Call Site 7 <<
	.uleb128 .Ltmp13-.Ltmp12                @   Call between .Ltmp12 and .Ltmp13
	.uleb128 .Ltmp14-.Lfunc_begin0          @     jumps to .Ltmp14
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp13-.Lfunc_begin0          @ >> Call Site 8 <<
	.uleb128 .Ltmp15-.Ltmp13                @   Call between .Ltmp13 and .Ltmp15
	.byte	0                               @     has no landing pad
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp15-.Lfunc_begin0          @ >> Call Site 9 <<
	.uleb128 .Ltmp16-.Ltmp15                @   Call between .Ltmp15 and .Ltmp16
	.uleb128 .Ltmp17-.Lfunc_begin0          @     jumps to .Ltmp17
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp18-.Lfunc_begin0          @ >> Call Site 10 <<
	.uleb128 .Ltmp19-.Ltmp18                @   Call between .Ltmp18 and .Ltmp19
	.uleb128 .Ltmp20-.Lfunc_begin0          @     jumps to .Ltmp20
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp19-.Lfunc_begin0          @ >> Call Site 11 <<
	.uleb128 .Ltmp21-.Ltmp19                @   Call between .Ltmp19 and .Ltmp21
	.byte	0                               @     has no landing pad
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp21-.Lfunc_begin0          @ >> Call Site 12 <<
	.uleb128 .Ltmp22-.Ltmp21                @   Call between .Ltmp21 and .Ltmp22
	.uleb128 .Ltmp23-.Lfunc_begin0          @     jumps to .Ltmp23
	.byte	0                               @   On action: cleanup
	.uleb128 .Ltmp22-.Lfunc_begin0          @ >> Call Site 13 <<
	.uleb128 .Lfunc_end1-.Ltmp22            @   Call between .Ltmp22 and .Lfunc_end1
	.byte	0                               @     has no landing pad
	.byte	0                               @   On action: cleanup
.Lcst_end0:
	.p2align	2
	.fnend
                                        @ -- End function
	.type	.L.str,%object                  @ @.str
	.section	.rodata.str2.2,"aMS",%progbits,2
	.p2align	1, 0x0
.L.str:
	.short	37                              @ 0x25
	.short	100                             @ 0x64
	.short	0                               @ 0x0
	.size	.L.str, 6

	.type	.L.str.1,%object                @ @.str.1
	.section	.rodata.str2.8,"aMS",%progbits,2
	.p2align	3, 0x0
.L.str.1:
	.short	119                             @ 0x77
	.short	105                             @ 0x69
	.short	100                             @ 0x64
	.short	103                             @ 0x67
	.short	101                             @ 0x65
	.short	116                             @ 0x74
	.short	67                              @ 0x43
	.short	111                             @ 0x6f
	.short	118                             @ 0x76
	.short	101                             @ 0x65
	.short	114                             @ 0x72
	.short	82                              @ 0x52
	.short	0                               @ 0x0
	.size	.L.str.1, 26

	.type	.L.str.2,%object                @ @.str.2
	.p2align	3, 0x0
.L.str.2:
	.short	119                             @ 0x77
	.short	105                             @ 0x69
	.short	100                             @ 0x64
	.short	103                             @ 0x67
	.short	101                             @ 0x65
	.short	116                             @ 0x74
	.short	67                              @ 0x43
	.short	111                             @ 0x6f
	.short	118                             @ 0x76
	.short	101                             @ 0x65
	.short	114                             @ 0x72
	.short	71                              @ 0x47
	.short	0                               @ 0x0
	.size	.L.str.2, 26

	.type	.L.str.3,%object                @ @.str.3
	.p2align	3, 0x0
.L.str.3:
	.short	119                             @ 0x77
	.short	105                             @ 0x69
	.short	100                             @ 0x64
	.short	103                             @ 0x67
	.short	101                             @ 0x65
	.short	116                             @ 0x74
	.short	67                              @ 0x43
	.short	111                             @ 0x6f
	.short	118                             @ 0x76
	.short	101                             @ 0x65
	.short	114                             @ 0x72
	.short	66                              @ 0x42
	.short	0                               @ 0x0
	.size	.L.str.3, 26

	.ident	"clang version 22.1.8"
	.section	".note.GNU-stack","",%progbits
	.addrsig
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym __cxa_end_cleanup
	.eabi_attribute	30, 1	@ Tag_ABI_optimization_goals
