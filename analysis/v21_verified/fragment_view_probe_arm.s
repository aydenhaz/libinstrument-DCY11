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
	.file	"fragment_view_signature_probe.cpp"
	.text
	.globl	_Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE @ -- Begin function _Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE
	.p2align	2
	.type	_Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE,%function
	.code	32
_Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE: @ @_Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	pop	{r11, lr}
	b	_ZN9GNS_FRAME10CGFragment8inflaterEiPNS_11CGViewGroupE
.Lfunc_end0:
	.size	_Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE, .Lfunc_end0-_Z11inflateViewPN9GNS_FRAME10CGFragmentEiPNS_11CGViewGroupE
	.fnend
                                        @ -- End function
	.globl	_Z10lookupViewPN9GNS_FRAME10CGFragmentEi @ -- Begin function _Z10lookupViewPN9GNS_FRAME10CGFragmentEi
	.p2align	2
	.type	_Z10lookupViewPN9GNS_FRAME10CGFragmentEi,%function
	.code	32
_Z10lookupViewPN9GNS_FRAME10CGFragmentEi: @ @_Z10lookupViewPN9GNS_FRAME10CGFragmentEi
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	pop	{r11, lr}
	b	_ZN9GNS_FRAME10CGFragment12findViewByIdEi
.Lfunc_end1:
	.size	_Z10lookupViewPN9GNS_FRAME10CGFragmentEi, .Lfunc_end1-_Z10lookupViewPN9GNS_FRAME10CGFragmentEi
	.fnend
                                        @ -- End function
	.globl	_Z5isTopPN9GNS_FRAME10CGFragmentE @ -- Begin function _Z5isTopPN9GNS_FRAME10CGFragmentE
	.p2align	2
	.type	_Z5isTopPN9GNS_FRAME10CGFragmentE,%function
	.code	32
_Z5isTopPN9GNS_FRAME10CGFragmentE:      @ @_Z5isTopPN9GNS_FRAME10CGFragmentE
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	pop	{r11, lr}
	b	_ZN9GNS_FRAME10CGFragment13isTopFragmentEv
.Lfunc_end2:
	.size	_Z5isTopPN9GNS_FRAME10CGFragmentE, .Lfunc_end2-_Z5isTopPN9GNS_FRAME10CGFragmentE
	.fnend
                                        @ -- End function
	.ident	"clang version 22.1.8"
	.section	".note.GNU-stack","",%progbits
	.addrsig
	.eabi_attribute	30, 1	@ Tag_ABI_optimization_goals
