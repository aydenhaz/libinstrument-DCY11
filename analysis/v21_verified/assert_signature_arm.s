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
	.file	"assert_signature_probe.cpp"
	.text
	.globl	_Z15assertConditionb            @ -- Begin function _Z15assertConditionb
	.p2align	2
	.type	_Z15assertConditionb,%function
	.code	32
_Z15assertConditionb:                   @ @_Z15assertConditionb
.Lfunc_begin0:
	.file	1 "/workspace/scratch/d8a6667a9410" "rebuild-v3/tests/assert_signature_probe.cpp"
	.loc	1 2 0                           @ rebuild-v3/tests/assert_signature_probe.cpp:2:0
	.fnstart
	.cfi_sections .debug_frame
	.cfi_startproc
@ %bb.0:
	@DEBUG_VALUE: assertCondition:value <- [DW_OP_LLVM_convert 1 7, DW_OP_LLVM_convert 8 7, DW_OP_stack_value] $r0
	.save	{r11, lr}
	push	{r11, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset lr, -4
	.cfi_offset r11, -8
	.setfp	r11, sp
	mov	r11, sp
	.cfi_def_cfa_register r11
.Ltmp0:
	.loc	1 2 36 prologue_end epilogue_begin @ rebuild-v3/tests/assert_signature_probe.cpp:2:36
	pop	{r11, lr}
.Ltmp1:
	b	I_Assert
.Ltmp2:
.Lfunc_end0:
	.size	_Z15assertConditionb, .Lfunc_end0-_Z15assertConditionb
	.cfi_endproc
	.file	2 "/workspace/scratch/d8a6667a9410" "rebuild-v3/stub_include/GFrame/GblAlc.h"
	.fnend
                                        @ -- End function
	.globl	_Z10assertTruev                 @ -- Begin function _Z10assertTruev
	.p2align	2
	.type	_Z10assertTruev,%function
	.code	32
_Z10assertTruev:                        @ @_Z10assertTruev
.Lfunc_begin1:
	.loc	1 3 0                           @ rebuild-v3/tests/assert_signature_probe.cpp:3:0
	.fnstart
	.cfi_startproc
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset lr, -4
	.cfi_offset r11, -8
	.setfp	r11, sp
	mov	r11, sp
	.cfi_def_cfa_register r11
.Ltmp3:
	.loc	1 3 21 prologue_end             @ rebuild-v3/tests/assert_signature_probe.cpp:3:21
	mov	r0, #1
	.loc	1 3 21 epilogue_begin is_stmt 0 @ rebuild-v3/tests/assert_signature_probe.cpp:3:21
	pop	{r11, lr}
.Ltmp4:
	b	I_Assert
.Ltmp5:
.Lfunc_end1:
	.size	_Z10assertTruev, .Lfunc_end1-_Z10assertTruev
	.cfi_endproc
	.fnend
                                        @ -- End function
	.globl	_Z11assertFalsev                @ -- Begin function _Z11assertFalsev
	.p2align	2
	.type	_Z11assertFalsev,%function
	.code	32
_Z11assertFalsev:                       @ @_Z11assertFalsev
.Lfunc_begin2:
	.loc	1 4 0 is_stmt 1                 @ rebuild-v3/tests/assert_signature_probe.cpp:4:0
	.fnstart
	.cfi_startproc
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset lr, -4
	.cfi_offset r11, -8
	.setfp	r11, sp
	mov	r11, sp
	.cfi_def_cfa_register r11
.Ltmp6:
	.loc	1 4 22 prologue_end             @ rebuild-v3/tests/assert_signature_probe.cpp:4:22
	mov	r0, #0
	.loc	1 4 22 epilogue_begin is_stmt 0 @ rebuild-v3/tests/assert_signature_probe.cpp:4:22
	pop	{r11, lr}
.Ltmp7:
	b	I_Assert
.Ltmp8:
.Lfunc_end2:
	.size	_Z11assertFalsev, .Lfunc_end2-_Z11assertFalsev
	.cfi_endproc
	.fnend
                                        @ -- End function
	.section	.debug_abbrev,"",%progbits
	.byte	1                               @ Abbreviation Code
	.byte	17                              @ DW_TAG_compile_unit
	.byte	1                               @ DW_CHILDREN_yes
	.byte	37                              @ DW_AT_producer
	.byte	14                              @ DW_FORM_strp
	.byte	19                              @ DW_AT_language
	.byte	5                               @ DW_FORM_data2
	.byte	3                               @ DW_AT_name
	.byte	14                              @ DW_FORM_strp
	.byte	16                              @ DW_AT_stmt_list
	.byte	23                              @ DW_FORM_sec_offset
	.byte	27                              @ DW_AT_comp_dir
	.byte	14                              @ DW_FORM_strp
	.byte	17                              @ DW_AT_low_pc
	.byte	1                               @ DW_FORM_addr
	.byte	18                              @ DW_AT_high_pc
	.byte	6                               @ DW_FORM_data4
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	2                               @ Abbreviation Code
	.byte	46                              @ DW_TAG_subprogram
	.byte	1                               @ DW_CHILDREN_yes
	.byte	17                              @ DW_AT_low_pc
	.byte	1                               @ DW_FORM_addr
	.byte	18                              @ DW_AT_high_pc
	.byte	6                               @ DW_FORM_data4
	.byte	64                              @ DW_AT_frame_base
	.byte	24                              @ DW_FORM_exprloc
	.ascii	"\227B"                         @ DW_AT_GNU_all_call_sites
	.byte	25                              @ DW_FORM_flag_present
	.byte	110                             @ DW_AT_linkage_name
	.byte	14                              @ DW_FORM_strp
	.byte	3                               @ DW_AT_name
	.byte	14                              @ DW_FORM_strp
	.byte	58                              @ DW_AT_decl_file
	.byte	11                              @ DW_FORM_data1
	.byte	59                              @ DW_AT_decl_line
	.byte	11                              @ DW_FORM_data1
	.byte	63                              @ DW_AT_external
	.byte	25                              @ DW_FORM_flag_present
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	3                               @ Abbreviation Code
	.byte	5                               @ DW_TAG_formal_parameter
	.byte	0                               @ DW_CHILDREN_no
	.byte	2                               @ DW_AT_location
	.byte	24                              @ DW_FORM_exprloc
	.byte	3                               @ DW_AT_name
	.byte	14                              @ DW_FORM_strp
	.byte	58                              @ DW_AT_decl_file
	.byte	11                              @ DW_FORM_data1
	.byte	59                              @ DW_AT_decl_line
	.byte	11                              @ DW_FORM_data1
	.byte	73                              @ DW_AT_type
	.byte	19                              @ DW_FORM_ref4
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	4                               @ Abbreviation Code
	.ascii	"\211\202\001"                  @ DW_TAG_GNU_call_site
	.byte	1                               @ DW_CHILDREN_yes
	.byte	49                              @ DW_AT_abstract_origin
	.byte	19                              @ DW_FORM_ref4
	.ascii	"\225B"                         @ DW_AT_GNU_tail_call
	.byte	25                              @ DW_FORM_flag_present
	.byte	17                              @ DW_AT_low_pc
	.byte	1                               @ DW_FORM_addr
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	5                               @ Abbreviation Code
	.ascii	"\212\202\001"                  @ DW_TAG_GNU_call_site_parameter
	.byte	0                               @ DW_CHILDREN_no
	.byte	2                               @ DW_AT_location
	.byte	24                              @ DW_FORM_exprloc
	.ascii	"\221B"                         @ DW_AT_GNU_call_site_value
	.byte	24                              @ DW_FORM_exprloc
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	6                               @ Abbreviation Code
	.byte	46                              @ DW_TAG_subprogram
	.byte	1                               @ DW_CHILDREN_yes
	.byte	3                               @ DW_AT_name
	.byte	14                              @ DW_FORM_strp
	.byte	58                              @ DW_AT_decl_file
	.byte	11                              @ DW_FORM_data1
	.byte	59                              @ DW_AT_decl_line
	.byte	11                              @ DW_FORM_data1
	.byte	60                              @ DW_AT_declaration
	.byte	25                              @ DW_FORM_flag_present
	.byte	63                              @ DW_AT_external
	.byte	25                              @ DW_FORM_flag_present
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	7                               @ Abbreviation Code
	.byte	5                               @ DW_TAG_formal_parameter
	.byte	0                               @ DW_CHILDREN_no
	.byte	73                              @ DW_AT_type
	.byte	19                              @ DW_FORM_ref4
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	8                               @ Abbreviation Code
	.byte	36                              @ DW_TAG_base_type
	.byte	0                               @ DW_CHILDREN_no
	.byte	3                               @ DW_AT_name
	.byte	14                              @ DW_FORM_strp
	.byte	62                              @ DW_AT_encoding
	.byte	11                              @ DW_FORM_data1
	.byte	11                              @ DW_AT_byte_size
	.byte	11                              @ DW_FORM_data1
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	9                               @ Abbreviation Code
	.ascii	"\211\202\001"                  @ DW_TAG_GNU_call_site
	.byte	0                               @ DW_CHILDREN_no
	.byte	49                              @ DW_AT_abstract_origin
	.byte	19                              @ DW_FORM_ref4
	.ascii	"\225B"                         @ DW_AT_GNU_tail_call
	.byte	25                              @ DW_FORM_flag_present
	.byte	17                              @ DW_AT_low_pc
	.byte	1                               @ DW_FORM_addr
	.byte	0                               @ EOM(1)
	.byte	0                               @ EOM(2)
	.byte	0                               @ EOM(3)
	.section	.debug_info,"",%progbits
.Lcu_begin0:
	.long	.Ldebug_info_end0-.Ldebug_info_start0 @ Length of Unit
.Ldebug_info_start0:
	.short	4                               @ DWARF version number
	.long	.debug_abbrev                   @ Offset Into Abbrev. Section
	.byte	4                               @ Address Size (in bytes)
	.byte	1                               @ Abbrev [1] 0xb:0xa7 DW_TAG_compile_unit
	.long	.Linfo_string0                  @ DW_AT_producer
	.short	33                              @ DW_AT_language
	.long	.Linfo_string1                  @ DW_AT_name
	.long	.Lline_table_start0             @ DW_AT_stmt_list
	.long	.Linfo_string2                  @ DW_AT_comp_dir
	.long	.Lfunc_begin0                   @ DW_AT_low_pc
	.long	.Lfunc_end2-.Lfunc_begin0       @ DW_AT_high_pc
	.byte	2                               @ Abbrev [2] 0x26:0x39 DW_TAG_subprogram
	.long	.Lfunc_begin0                   @ DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       @ DW_AT_high_pc
	.byte	1                               @ DW_AT_frame_base
	.byte	91
                                        @ DW_AT_GNU_all_call_sites
	.long	.Linfo_string5                  @ DW_AT_linkage_name
	.long	.Linfo_string6                  @ DW_AT_name
	.byte	1                               @ DW_AT_decl_file
	.byte	2                               @ DW_AT_decl_line
                                        @ DW_AT_external
	.byte	3                               @ Abbrev [3] 0x3b:0x12 DW_TAG_formal_parameter
	.byte	6                               @ DW_AT_location
	.byte	112
	.byte	0
	.byte	16
	.byte	1
	.byte	26
	.byte	159
	.long	.Linfo_string11                 @ DW_AT_name
	.byte	1                               @ DW_AT_decl_file
	.byte	2                               @ DW_AT_decl_line
	.long	108                             @ DW_AT_type
	.byte	4                               @ Abbrev [4] 0x4d:0x11 DW_TAG_GNU_call_site
	.long	95                              @ DW_AT_abstract_origin
                                        @ DW_AT_GNU_tail_call
	.long	.Ltmp2                          @ DW_AT_low_pc
	.byte	5                               @ Abbrev [5] 0x56:0x7 DW_TAG_GNU_call_site_parameter
	.byte	1                               @ DW_AT_location
	.byte	80
	.byte	3                               @ DW_AT_GNU_call_site_value
	.byte	243
	.byte	1
	.byte	80
	.byte	0                               @ End Of Children Mark
	.byte	0                               @ End Of Children Mark
	.byte	6                               @ Abbrev [6] 0x5f:0xd DW_TAG_subprogram
	.long	.Linfo_string3                  @ DW_AT_name
	.byte	2                               @ DW_AT_decl_file
	.byte	8                               @ DW_AT_decl_line
                                        @ DW_AT_declaration
                                        @ DW_AT_external
	.byte	7                               @ Abbrev [7] 0x66:0x5 DW_TAG_formal_parameter
	.long	108                             @ DW_AT_type
	.byte	0                               @ End Of Children Mark
	.byte	8                               @ Abbrev [8] 0x6c:0x7 DW_TAG_base_type
	.long	.Linfo_string4                  @ DW_AT_name
	.byte	2                               @ DW_AT_encoding
	.byte	1                               @ DW_AT_byte_size
	.byte	2                               @ Abbrev [2] 0x73:0x1f DW_TAG_subprogram
	.long	.Lfunc_begin1                   @ DW_AT_low_pc
	.long	.Lfunc_end1-.Lfunc_begin1       @ DW_AT_high_pc
	.byte	1                               @ DW_AT_frame_base
	.byte	91
                                        @ DW_AT_GNU_all_call_sites
	.long	.Linfo_string7                  @ DW_AT_linkage_name
	.long	.Linfo_string8                  @ DW_AT_name
	.byte	1                               @ DW_AT_decl_file
	.byte	3                               @ DW_AT_decl_line
                                        @ DW_AT_external
	.byte	9                               @ Abbrev [9] 0x88:0x9 DW_TAG_GNU_call_site
	.long	95                              @ DW_AT_abstract_origin
                                        @ DW_AT_GNU_tail_call
	.long	.Ltmp5                          @ DW_AT_low_pc
	.byte	0                               @ End Of Children Mark
	.byte	2                               @ Abbrev [2] 0x92:0x1f DW_TAG_subprogram
	.long	.Lfunc_begin2                   @ DW_AT_low_pc
	.long	.Lfunc_end2-.Lfunc_begin2       @ DW_AT_high_pc
	.byte	1                               @ DW_AT_frame_base
	.byte	91
                                        @ DW_AT_GNU_all_call_sites
	.long	.Linfo_string9                  @ DW_AT_linkage_name
	.long	.Linfo_string10                 @ DW_AT_name
	.byte	1                               @ DW_AT_decl_file
	.byte	4                               @ DW_AT_decl_line
                                        @ DW_AT_external
	.byte	9                               @ Abbrev [9] 0xa7:0x9 DW_TAG_GNU_call_site
	.long	95                              @ DW_AT_abstract_origin
                                        @ DW_AT_GNU_tail_call
	.long	.Ltmp8                          @ DW_AT_low_pc
	.byte	0                               @ End Of Children Mark
	.byte	0                               @ End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_str,"MS",%progbits,1
.Linfo_string0:
	.asciz	"clang version 22.1.8"          @ string offset=0
.Linfo_string1:
	.asciz	"rebuild-v3/tests/assert_signature_probe.cpp" @ string offset=21
.Linfo_string2:
	.asciz	"/workspace/scratch/d8a6667a9410" @ string offset=65
.Linfo_string3:
	.asciz	"I_Assert"                      @ string offset=97
.Linfo_string4:
	.asciz	"bool"                          @ string offset=106
.Linfo_string5:
	.asciz	"_Z15assertConditionb"          @ string offset=111
.Linfo_string6:
	.asciz	"assertCondition"               @ string offset=132
.Linfo_string7:
	.asciz	"_Z10assertTruev"               @ string offset=148
.Linfo_string8:
	.asciz	"assertTrue"                    @ string offset=164
.Linfo_string9:
	.asciz	"_Z11assertFalsev"              @ string offset=175
.Linfo_string10:
	.asciz	"assertFalse"                   @ string offset=192
.Linfo_string11:
	.asciz	"value"                         @ string offset=204
	.ident	"clang version 22.1.8"
	.section	".note.GNU-stack","",%progbits
	.addrsig
	.eabi_attribute	30, 1	@ Tag_ABI_optimization_goals
	.section	.debug_line,"",%progbits
.Lline_table_start0:
