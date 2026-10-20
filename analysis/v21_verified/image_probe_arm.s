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
	.file	"image_signature_probe.cpp"
	.text
	.globl	_Z16createCoverImagev           @ -- Begin function _Z16createCoverImagev
	.p2align	2
	.type	_Z16createCoverImagev,%function
	.code	32
_Z16createCoverImagev:                  @ @_Z16createCoverImagev
.Lfunc_begin0:
	.fnstart
@ %bb.0:
	.save	{r4, r5, r11, lr}
	push	{r4, r5, r11, lr}
	.setfp	r11, sp, #8
	add	r11, sp, #8
	.pad	#16
	sub	sp, sp, #16
	mov	r0, #480
	mov	r5, #480
	bl	_Znwj
	mov	r4, r0
.Ltmp0:                                 @ EH_LABEL
	bl	_ZN9GNS_FRAME7CGImageC1Ev
.Ltmp1:                                 @ EH_LABEL
@ %bb.1:
	mov	r0, #640
	add	r1, sp, #4
	str	r0, [sp, #8]
	movw	r0, :lower16:_ZTVN3GPI6CGSizeE
	movt	r0, :upper16:_ZTVN3GPI6CGSizeE
	movw	r2, #257
	add	r0, r0, #8
	str	r0, [sp, #4]
	mov	r0, r4
	mov	r3, #0
	str	r5, [sp, #12]
	bl	_ZN9GNS_FRAME7CGImage6createERKN3GPI6CGSizeE12EGColorType_Ph
	mov	r5, r0
	mov	r0, r4
	bl	_ZN9GNS_FRAME7CGImageD1Ev
	bl	_ZdlPv
	mov	r0, r5
	sub	sp, r11, #8
	pop	{r4, r5, r11, pc}
.LBB0_2:
.Ltmp2:                                 @ EH_LABEL
	mov	r0, r4
	bl	_ZdlPv
	bl	__cxa_end_cleanup
.Lfunc_end0:
	.size	_Z16createCoverImagev, .Lfunc_end0-_Z16createCoverImagev
	.personality __gxx_personality_v0
	.handlerdata
	.p2align	2
GCC_except_table0:
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
	.uleb128 .Ltmp1-.Lfunc_begin0           @ >> Call Site 3 <<
	.uleb128 .Lfunc_end0-.Ltmp1             @   Call between .Ltmp1 and .Lfunc_end0
	.byte	0                               @     has no landing pad
	.byte	0                               @   On action: cleanup
.Lcst_end0:
	.p2align	2
	.fnend
                                        @ -- End function
	.section	.text._ZN3GPI6CGSizeD2Ev,"axG",%progbits,_ZN3GPI6CGSizeD2Ev,comdat
	.weak	_ZN3GPI6CGSizeD2Ev              @ -- Begin function _ZN3GPI6CGSizeD2Ev
	.p2align	2
	.type	_ZN3GPI6CGSizeD2Ev,%function
	.code	32
_ZN3GPI6CGSizeD2Ev:                     @ @_ZN3GPI6CGSizeD2Ev
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	pop	{r11, pc}
.Lfunc_end1:
	.size	_ZN3GPI6CGSizeD2Ev, .Lfunc_end1-_ZN3GPI6CGSizeD2Ev
	.fnend
                                        @ -- End function
	.section	.text._ZN3GPI6CGSizeD0Ev,"axG",%progbits,_ZN3GPI6CGSizeD0Ev,comdat
	.weak	_ZN3GPI6CGSizeD0Ev              @ -- Begin function _ZN3GPI6CGSizeD0Ev
	.p2align	2
	.type	_ZN3GPI6CGSizeD0Ev,%function
	.code	32
_ZN3GPI6CGSizeD0Ev:                     @ @_ZN3GPI6CGSizeD0Ev
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	pop	{r11, lr}
	b	_ZdlPv
.Lfunc_end2:
	.size	_ZN3GPI6CGSizeD0Ev, .Lfunc_end2-_ZN3GPI6CGSizeD0Ev
	.fnend
                                        @ -- End function
	.type	_ZTVN3GPI6CGSizeE,%object       @ @_ZTVN3GPI6CGSizeE
	.section	.rodata._ZTVN3GPI6CGSizeE,"aG",%progbits,_ZTVN3GPI6CGSizeE,comdat
	.weak	_ZTVN3GPI6CGSizeE
	.p2align	2, 0x0
_ZTVN3GPI6CGSizeE:
	.long	0
	.long	_ZTIN3GPI6CGSizeE
	.long	_ZN3GPI6CGSizeD2Ev
	.long	_ZN3GPI6CGSizeD0Ev
	.size	_ZTVN3GPI6CGSizeE, 16

	.type	_ZTIN3GPI6CGSizeE,%object       @ @_ZTIN3GPI6CGSizeE
	.section	.rodata._ZTIN3GPI6CGSizeE,"aG",%progbits,_ZTIN3GPI6CGSizeE,comdat
	.weak	_ZTIN3GPI6CGSizeE
	.p2align	2, 0x0
_ZTIN3GPI6CGSizeE:
	.long	_ZTVN10__cxxabiv121__vmi_class_type_infoE+8
	.long	_ZTSN3GPI6CGSizeE
	.long	0                               @ 0x0
	.long	1                               @ 0x1
	.long	_ZTI8STGSize_
	.long	1026                            @ 0x402
	.size	_ZTIN3GPI6CGSizeE, 24

	.type	_ZTSN3GPI6CGSizeE,%object       @ @_ZTSN3GPI6CGSizeE
	.section	.rodata._ZTSN3GPI6CGSizeE,"aG",%progbits,_ZTSN3GPI6CGSizeE,comdat
	.weak	_ZTSN3GPI6CGSizeE
_ZTSN3GPI6CGSizeE:
	.asciz	"N3GPI6CGSizeE"
	.size	_ZTSN3GPI6CGSizeE, 14

	.type	_ZTI8STGSize_,%object           @ @_ZTI8STGSize_
	.section	.rodata._ZTI8STGSize_,"aG",%progbits,_ZTI8STGSize_,comdat
	.weak	_ZTI8STGSize_
	.p2align	2, 0x0
_ZTI8STGSize_:
	.long	_ZTVN10__cxxabiv117__class_type_infoE+8
	.long	_ZTS8STGSize_
	.size	_ZTI8STGSize_, 8

	.type	_ZTS8STGSize_,%object           @ @_ZTS8STGSize_
	.section	.rodata._ZTS8STGSize_,"aG",%progbits,_ZTS8STGSize_,comdat
	.weak	_ZTS8STGSize_
_ZTS8STGSize_:
	.asciz	"8STGSize_"
	.size	_ZTS8STGSize_, 10

	.ident	"clang version 22.1.8"
	.section	".note.GNU-stack","",%progbits
	.addrsig
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym __cxa_end_cleanup
	.addrsig_sym _ZTIN3GPI6CGSizeE
	.addrsig_sym _ZTVN10__cxxabiv121__vmi_class_type_infoE
	.addrsig_sym _ZTSN3GPI6CGSizeE
	.addrsig_sym _ZTI8STGSize_
	.addrsig_sym _ZTVN10__cxxabiv117__class_type_infoE
	.addrsig_sym _ZTS8STGSize_
	.eabi_attribute	30, 1	@ Tag_ABI_optimization_goals
