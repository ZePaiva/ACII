/*
 * Copyright (c) 1996-2004 MIPS Technologies, Inc.
 * All rights reserved.
 * 
 * Unpublished rights (if any) reserved under the copyright laws of the
 * United States of America and other countries.
 * 
 * This code is proprietary to MIPS Technologies, Inc. ("MIPS
 * Technologies").  Any copying, reproducing, modifying or use of
 * this code (in whole or in part) that is not expressly permitted
 * in writing by MIPS Technologies or an authorized third party is
 * strictly prohibited.  At a minimum, this code is protected under
 * unfair competition and copyright laws.  Violations thereof may result
 * in criminal penalties and fines.
 * 
 * MIPS Technologies reserves the right to change this code to improve
 * function, design or otherwise.  MIPS Technologies does not assume any
 * liability arising out of the application or use of this code, or of
 * any error or omission in such code.  Any warranties, whether express,
 * statutory, implied or otherwise, including but not limited to the
 * implied warranties of merchantability or fitness for a particular
 * purpose, are excluded.  Except as expressly provided in any written
 * license agreement from MIPS Technologies or an authorized third party,
 * the furnishing of this code does not give recipient any license to
 * any intellectual property rights, including any patent rights, that
 * cover this code.
 * 
 * This code shall not be exported, reexported, transferred, or released,
 * directly or indirectly, in violation of the law of any country or
 * international law, regulation, treaty, Executive Order, statute,
 * amendments or supplements thereto.  Should a conflict arise regarding
 * the export, reexport, transfer, or release of this code, the laws of
 * the United States of America shall be the governing law.
 * 
 * This code constitutes one or more of the following: commercial
 * computer software, commercial computer software documentation or
 * other commercial items.  If the user of this code, or any related
 * documentation of any kind, including related technical data or
 * manuals, is an agency, department, or other entity of the United
 * States government ("Government"), the use, duplication, reproduction,
 * release, modification, disclosure, or transfer of this code, or
 * any related documentation of any kind, is restricted in accordance
 * with Federal Acquisition Regulation 12.212 for civilian agencies
 * and Defense Federal Acquisition Regulation Supplement 227.7202 for
 * military agencies.  The use of this code by the Government is further
 * restricted in accordance with the terms of the license agreement(s)
 * and/or applicable contract terms and conditions covering this code
 * from MIPS Technologies or an authorized third party.
 * 
 * 
 */

/*
 * stdarg.h : ANSI stdarg definitions
 */


#ifndef __STDARG_H

#ifndef __need___va_list
#define __STDARG_H
#endif
#undef __need___va_list

/* CPU-independent macros for GNU C 2.96 and above */

#ifndef __VA_LIST
#define __VA_LIST
/* A generally safe declaration for stdio.h, without loading
   everything else and polluting the namespace */
typedef __builtin_va_list __va_list;
#endif

/* Define same as __gnuc_va_list for GNU programs.  */
#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

/* Define the standard macros for the user,
   if this invocation was from the user program.  */
#ifdef __STDARG_H

#ifdef  __VARARGS_H

/* These macros implement traditional (non-ANSI) varargs
   for GNU C.  */
#define va_alist  __builtin_va_alist

typedef int __builtin_va_alist_t __attribute__((__mode__(__arg__)));

/* ??? It would be nice to get rid of the ellipsis here.  It causes
   current_function_varargs to be set in cc1.  */
#define va_dcl		__builtin_va_alist_t __builtin_va_alist; ...
#define va_start(v)	__builtin_varargs_start((v))

#else

/* These macros implement ANSI stdarg for GNU C.  */

/* Note that the type used in va_arg is supposed to match the
   actual type **after default promotions**.
   Thus, va_arg (..., short) is not valid.  */
#define va_start(v,l)	__builtin_stdarg_start((v),l)

#endif

/* These macros are common.  */

#define va_end		__builtin_va_end
#define va_arg		__builtin_va_arg
#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L
#define va_copy(d,s)	__builtin_va_copy((d),(s))
#endif
#define __va_copy(d,s)	__builtin_va_copy((d),(s))

/* Define va_list from __va_list */
typedef __va_list va_list;

#endif /* __STDARG_H */

#endif /* !defined __STDARG_H */
