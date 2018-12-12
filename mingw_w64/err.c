/*-
 * Copyright (c) 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __CYGWIN__
#define _getprogname() __argv[0]
#else
extern char __declspec(dllimport) *__progname;
#define _getprogname() __progname
#endif


static FILE *err_file; /* file to use for error output */
static void (*err_exit)(int);

/*
 * This is declared to take a `void *' so that the caller is not required
 * to include <stdio.h> first.  However, it is really a `FILE *', and the
 * manual page documents it as such.
 */
void
err_set_file(void *fp)
{
    if (fp)
        err_file = fp;
    else
        err_file = stderr;
}

void
err_set_exit(void (*ef)(int))
{
    err_exit = ef;
}

// C4702 unreachable code
#pragma warning(disable: 4702)

void
err(int eval, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verrc(eval, errno, fmt, ap);
    va_end(ap);
}

void
verr(int eval, const char *fmt, va_list ap)
{
    verrc(eval, errno, fmt, ap);
}

void
errc(int eval, int code, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verrc(eval, code, fmt, ap);
    va_end(ap);
}

void
verrc(int eval, int code, const char *fmt, va_list ap)
{
    if (err_file == NULL)
        err_set_file((FILE *)0);
    fprintf(err_file, "%s: ", _getprogname());
    if (fmt != NULL) {
        vfprintf(err_file, fmt, ap);
        fprintf(err_file, ": ");
    }
// C4996 'strerror': This function or variable may be unsafe. Consider using strerror_s instead.
#pragma warning(disable: 4996)
    fprintf(err_file, "%s\n", strerror(code));
#pragma warning(default: 4996)
    if (err_exit)
        err_exit(eval);
    exit(eval);
}

void
errx(int eval, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verrx(eval, fmt, ap);
    va_end(ap);
}

void
verrx(int eval, const char *fmt, va_list ap)
{
    if (err_file == NULL)
        err_set_file((FILE *)0);
    fprintf(err_file, "%s: ", _getprogname());
    if (fmt != NULL)
        vfprintf(err_file, fmt, ap);
    fprintf(err_file, "\n");
    if (err_exit)
        err_exit(eval);
    exit(eval);
}

#pragma warning(default: 4702)

void
warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vwarnc(errno, fmt, ap);
    va_end(ap);
}

void
vwarn(const char *fmt, va_list ap)
{
    vwarnc(errno, fmt, ap);
}

void
warnc(int code, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vwarnc(code, fmt, ap);
    va_end(ap);
}

void
vwarnc(int code, const char *fmt, va_list ap)
{
    if (err_file == NULL)
        err_set_file((FILE *)0);
    fprintf(err_file, "%s: ", _getprogname());
    if (fmt != NULL) {
        vfprintf(err_file, fmt, ap);
        fprintf(err_file, ": ");
    }
// C4996 'strerror': This function or variable may be unsafe. Consider using strerror_s instead.
#pragma warning(disable: 4996)
    fprintf(err_file, "%s\n", strerror(code));
#pragma warning(default: 4996)
}

void
warnx(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vwarnx(fmt, ap);
    va_end(ap);
}

void
vwarnx(const char *fmt, va_list ap)
{
    if (err_file == NULL)
        err_set_file((FILE *)0);
    fprintf(err_file, "%s: ", _getprogname());
    if (fmt != NULL)
        vfprintf(err_file, fmt, ap);
    fprintf(err_file, "\n");
}
