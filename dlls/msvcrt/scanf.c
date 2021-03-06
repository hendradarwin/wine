/*
 * general implementation of scanf used by scanf, sscanf, fscanf,
 * _cscanf, wscanf, swscanf and fwscanf
 *
 * Copyright 1996,1998 Marcus Meissner
 * Copyright 1996 Jukka Iivonen
 * Copyright 1997,2000 Uwe Bonnes
 * Copyright 2000 Jon Griffiths
 * Copyright 2002 Daniel Gudbjartsson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>
#include <limits.h>
#include <math.h>

#include "windef.h"
#include "winbase.h"
#include "winternl.h"
#include "msvcrt.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(msvcrt);

extern MSVCRT_FILE MSVCRT__iob[];

/* helper function for *scanf.  Returns the value of character c in the
 * given base, or -1 if the given character is not a digit of the base.
 */
static int char2digit(char c, int base) {
    if ((c>='0') && (c<='9') && (c<='0'+base-1)) return (c-'0');
    if (base<=10) return -1;
    if ((c>='A') && (c<='Z') && (c<='A'+base-11)) return (c-'A'+10);
    if ((c>='a') && (c<='z') && (c<='a'+base-11)) return (c-'a'+10);
    return -1;
}

/* helper function for *wscanf.  Returns the value of character c in the
 * given base, or -1 if the given character is not a digit of the base.
 */
static int wchar2digit(MSVCRT_wchar_t c, int base) {
    if ((c>='0') && (c<='9') && (c<='0'+base-1)) return (c-'0');
    if (base<=10) return -1;
    if ((c>='A') && (c<='Z') && (c<='A'+base-11)) return (c-'A'+10);
    if ((c>='a') && (c<='z') && (c<='a'+base-11)) return (c-'a'+10);
    return -1;
}

/* vfscanf_l */
#undef WIDE_SCANF
#undef CONSOLE
#undef STRING
#undef SECURE
#include "scanf.h"

/* vfscanf_s_l */
#define SECURE 1
#include "scanf.h"

/* vfwscanf_l */
#define WIDE_SCANF 1
#undef CONSOLE
#undef STRING
#undef SECURE
#include "scanf.h"

/* vfwscanf_s_l */
#define SECURE 1
#include "scanf.h"

/* vsscanf_l */
#undef WIDE_SCANF
#undef CONSOLE
#define STRING 1
#undef SECURE
#include "scanf.h"

/* vsscanf_s_l */
#define SECURE 1
#include "scanf.h"

/* vsnscanf_l */
#undef SECURE
#define STRING_LEN 1
#include "scanf.h"

/* vsnscanf_s_l */
#define SECURE
#include "scanf.h"

/* vsnwscanf_l */
#define WIDE_SCANF 1
#undef SECURE
#include "scanf.h"

/* vsnwscanf_s_l */
#define SECURE 1
#include "scanf.h"
#undef STRING_LEN

/* vswscanf_l */
#define WIDE_SCANF 1
#undef CONSOLE
#define STRING 1
#undef SECURE
#include "scanf.h"

/* vswscanf_s_l */
#define SECURE 1
#include "scanf.h"

/* vcscanf_l */
#undef WIDE_SCANF
#define CONSOLE 1
#undef STRING
#undef SECURE
#include "scanf.h"

/* vcscanf_s_l */
#define SECURE 1
#include "scanf.h"

/* vcwscanf_l */
#define WIDE_SCANF 1
#define CONSOLE 1
#undef STRING
#undef SECURE
#include "scanf.h"

/* vcwscanf_s_l */
#define SECURE 1
#include "scanf.h"


/*********************************************************************
 *		fscanf (MSVCRT.@)
 */
int CDECL MSVCRT_fscanf(MSVCRT_FILE *file, const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfscanf_l(file, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_fscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__fscanf_l(MSVCRT_FILE *file, const char *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfscanf_l(file, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		fscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT_fscanf_s(MSVCRT_FILE *file, const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfscanf_s_l(file, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_fscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__fscanf_s_l(MSVCRT_FILE *file, const char *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfscanf_s_l(file, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		scanf (MSVCRT.@)
 */
int CDECL MSVCRT_scanf(const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfscanf_l(MSVCRT_stdin, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_scanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__scanf_l(const char *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfscanf_l(MSVCRT_stdin, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		scanf_s (MSVCRT.@)
 */
int CDECL MSVCRT_scanf_s(const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfscanf_s_l(MSVCRT_stdin, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_scanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__scanf_s_l(const char *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfscanf_s_l(MSVCRT_stdin, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		fwscanf (MSVCRT.@)
 */
int CDECL MSVCRT_fwscanf(MSVCRT_FILE *file, const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfwscanf_l(file, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_fwscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__fwscanf_l(MSVCRT_FILE *file, const MSVCRT_wchar_t *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfwscanf_l(file, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		fwscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT_fwscanf_s(MSVCRT_FILE *file, const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfwscanf_s_l(file, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_fwscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__fwscanf_s_l(MSVCRT_FILE *file, const MSVCRT_wchar_t *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfwscanf_s_l(file, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		wscanf (MSVCRT.@)
 */
int CDECL MSVCRT_wscanf(const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfwscanf_l(MSVCRT_stdin, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_wscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__wscanf_l(const MSVCRT_wchar_t *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfwscanf_l(MSVCRT_stdin, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		wscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT_wscanf_s(const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vfwscanf_s_l(MSVCRT_stdin, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_wscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__wscanf_s_l(const MSVCRT_wchar_t *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vfwscanf_s_l(MSVCRT_stdin, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		sscanf (MSVCRT.@)
 */
int CDECL MSVCRT_sscanf(const char *str, const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vsscanf_l(str, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_sscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__sscanf_l(const char *str, const char *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vsscanf_l(str, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		sscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT_sscanf_s(const char *str, const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vsscanf_s_l(str, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_sscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__sscanf_s_l(const char *str, const char *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vsscanf_s_l(str, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		swscanf (MSVCRT.@)
 */
int CDECL MSVCRT_swscanf(const MSVCRT_wchar_t *str, const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vswscanf_l(str, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_swscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__swscanf_l(const MSVCRT_wchar_t *str, const MSVCRT_wchar_t *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vswscanf_l(str, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		swscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT_swscanf_s(const MSVCRT_wchar_t *str, const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vswscanf_s_l(str, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_swscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__swscanf_s_l(const MSVCRT_wchar_t *str, const MSVCRT_wchar_t *format,
        MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vswscanf_s_l(str, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cscanf (MSVCRT.@)
 */
int CDECL _cscanf(const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vcscanf_l(format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cscanf_l (MSVCRT.@)
 */
int CDECL _cscanf_l(const char *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vcscanf_l(format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cscanf_s (MSVCRT.@)
 */
int CDECL _cscanf_s(const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vcscanf_s_l(format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cscanf_s_l (MSVCRT.@)
 */
int CDECL _cscanf_s_l(const char *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vcscanf_s_l(format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cwscanf (MSVCRT.@)
 */
int CDECL _cwscanf(const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vcwscanf_l(format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cwscanf_l (MSVCRT.@)
 */
int CDECL _cwscanf_l(const MSVCRT_wchar_t *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vcwscanf_l(format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cwscanf_s (MSVCRT.@)
 */
int CDECL _cwscanf_s(const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vcwscanf_s_l(format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_cwscanf_s_l (MSVCRT.@)
 */
int CDECL _cwscanf_s_l(const MSVCRT_wchar_t *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vcwscanf_s_l(format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snscanf (MSVCRT.@)
 */
int CDECL MSVCRT__snscanf(char *input, MSVCRT_size_t length, const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vsnscanf_l(input, length, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__snscanf_l(char *input, MSVCRT_size_t length,
        const char *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vsnscanf_l(input, length, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT__snscanf_s(char *input, MSVCRT_size_t length, const char *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vsnscanf_s_l(input, length, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__snscanf_s_l(char *input, MSVCRT_size_t length,
        const char *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vsnscanf_s_l(input, length, format, locale, valist);
    __ms_va_end(valist);
    return res;
}


/*********************************************************************
 *		__stdio_common_vsscanf (MSVCRT.@)
 */
int CDECL MSVCRT__stdio_common_vsscanf(unsigned __int64 options,
                                       const char *input, MSVCRT_size_t length,
                                       const char *format,
                                       MSVCRT__locale_t locale,
                                       __ms_va_list valist)
{
    /* LEGACY_WIDE_SPECIFIERS only has got an effect on the wide
     * scanf. LEGACY_MSVCRT_COMPATIBILITY affects parsing of nan/inf,
     * but parsing of those isn't implemented at all yet. */
    if (options & ~UCRTBASE_SCANF_MASK)
        FIXME("options %s not handled\n", wine_dbgstr_longlong(options));
    if (options & UCRTBASE_SCANF_SECURECRT)
        return MSVCRT_vsnscanf_s_l(input, length, format, locale, valist);
    else
        return MSVCRT_vsnscanf_l(input, length, format, locale, valist);
}

/*********************************************************************
 *		__stdio_common_vswscanf (MSVCRT.@)
 */
int CDECL MSVCRT__stdio_common_vswscanf(unsigned __int64 options,
                                        const MSVCRT_wchar_t *input, MSVCRT_size_t length,
                                        const MSVCRT_wchar_t *format,
                                        MSVCRT__locale_t locale,
                                        __ms_va_list valist)
{
    /* LEGACY_WIDE_SPECIFIERS only has got an effect on the wide
     * scanf. LEGACY_MSVCRT_COMPATIBILITY affects parsing of nan/inf,
     * but parsing of those isn't implemented at all yet. */
    if (options & ~UCRTBASE_SCANF_MASK)
        FIXME("options %s not handled\n", wine_dbgstr_longlong(options));
    if (options & UCRTBASE_SCANF_SECURECRT)
        return MSVCRT_vsnwscanf_s_l(input, length, format, locale, valist);
    else
        return MSVCRT_vsnwscanf_l(input, length, format, locale, valist);
}

/*********************************************************************
 *		__stdio_common_vfscanf (MSVCRT.@)
 */
int CDECL MSVCRT__stdio_common_vfscanf(unsigned __int64 options,
                                       MSVCRT_FILE *file,
                                       const char *format,
                                       MSVCRT__locale_t locale,
                                       __ms_va_list valist)
{
    if (options & ~UCRTBASE_SCANF_SECURECRT)
        FIXME("options %s not handled\n", wine_dbgstr_longlong(options));
    if (options & UCRTBASE_SCANF_SECURECRT)
        return MSVCRT_vfscanf_s_l(file, format, locale, valist);
    else
        return MSVCRT_vfscanf_l(file, format, locale, valist);
}

/*********************************************************************
 *		__stdio_common_vfwscanf (MSVCRT.@)
 */
int CDECL MSVCRT__stdio_common_vfwscanf(unsigned __int64 options,
                                        MSVCRT_FILE *file,
                                        const MSVCRT_wchar_t *format,
                                        MSVCRT__locale_t locale,
                                        __ms_va_list valist)
{
    if (options & ~UCRTBASE_SCANF_SECURECRT)
        FIXME("options %s not handled\n", wine_dbgstr_longlong(options));
    if (options & UCRTBASE_SCANF_SECURECRT)
        return MSVCRT_vfwscanf_s_l(file, format, locale, valist);
    else
        return MSVCRT_vfwscanf_l(file, format, locale, valist);
}

/*********************************************************************
 *		_snwscanf (MSVCRT.@)
 */
int CDECL MSVCRT__snwscanf(MSVCRT_wchar_t *input, MSVCRT_size_t length,
        const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vsnwscanf_l(input, length, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snwscanf_l (MSVCRT.@)
 */
int CDECL MSVCRT__snwscanf_l(MSVCRT_wchar_t *input, MSVCRT_size_t length,
        const MSVCRT_wchar_t *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vsnwscanf_l(input, length, format, locale, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snwscanf_s (MSVCRT.@)
 */
int CDECL MSVCRT__snwscanf_s(MSVCRT_wchar_t *input, MSVCRT_size_t length,
        const MSVCRT_wchar_t *format, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, format);
    res = MSVCRT_vsnwscanf_s_l(input, length, format, NULL, valist);
    __ms_va_end(valist);
    return res;
}

/*********************************************************************
 *		_snscanf_s_l (MSVCRT.@)
 */
int CDECL MSVCRT__snwscanf_s_l(MSVCRT_wchar_t *input, MSVCRT_size_t length,
        const MSVCRT_wchar_t *format, MSVCRT__locale_t locale, ...)
{
    __ms_va_list valist;
    int res;

    __ms_va_start(valist, locale);
    res = MSVCRT_vsnwscanf_s_l(input, length, format, locale, valist);
    __ms_va_end(valist);
    return res;
}
