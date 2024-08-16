/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nspr_haiku_defs_h___
#define nspr_haiku_defs_h___

#include "prthread.h"

#define PR_LINKER_ARCH  "haiku"
#define _PR_SI_SYSNAME  "HAIKU"
#if defined(__i386__)
#define _PR_SI_ARCHITECTURE "x86"
#elif defined(__alpha__)
#define _PR_SI_ARCHITECTURE "alpha"
#elif defined(__sparc__)
#define _PR_SI_ARCHITECTURE "sparc"
#elif defined(__ia64__)
#define _PR_SI_ARCHITECTURE "ia64"
#elif defined(__amd64__)
#define _PR_SI_ARCHITECTURE "amd64"
#elif defined(__powerpc64__)
#define _PR_SI_ARCHITECTURE "powerpc64"
#elif defined(__powerpc__)
#define _PR_SI_ARCHITECTURE "powerpc"
#elif defined(__aarch64__)
#define _PR_SI_ARCHITECTURE "aarch64"
#elif defined(__arm__)
#define _PR_SI_ARCHITECTURE "arm"
#elif defined(__mips64__)
#define _PR_SI_ARCHITECTURE "mips64"
#elif defined(__mips__)
#define _PR_SI_ARCHITECTURE "mips"
#else
#error "Unknown CPU architecture"
#endif
#if defined(__ELF__)
#define PR_DLL_SUFFIX          ".so"
#else
#define PR_DLL_SUFFIX          ".so.1.0"
#endif

#define _PR_VMBASE              0x30000000
#define _PR_STACK_VMBASE    0x50000000
#define _MD_DEFAULT_STACK_SIZE  65536L
#define _MD_MMAP_FLAGS          MAP_PRIVATE

#undef  HAVE_STACK_GROWING_UP
#define HAVE_DLL
#define USE_DLFCN
#define _PR_HAVE_SOCKADDR_LEN
#define _PR_STAT_HAS_ST_ATIM
#define _PR_HAVE_LARGE_OFF_T

#define _PR_POLL_AVAILABLE
#undef _PR_USE_POLL

#define _PR_HAVE_SYSV_SEMAPHORES
#define PR_HAVE_POSIX_NAMED_SHARED_MEMORY

#define _PR_INET6
#define _PR_HAVE_INET_NTOP
#define _PR_HAVE_GETHOSTBYNAME2
#define _PR_HAVE_GETADDRINFO
#define _PR_INET6_PROBE
#define _PR_IPV6_V6ONLY_PROBE

#define USE_SETJMP

extern void _MD_EarlyInit(void);

#define _MD_EARLY_INIT                  _MD_EarlyInit
#define _MD_FINAL_INIT          _PR_UnixInit
#define _PR_HAVE_CLOCK_MONOTONIC

/* freebsd has INADDR_LOOPBACK defined, but in /usr/include/rpc/types.h, and I didn't
   want to be including that.. */
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK         (u_long)0x7F000001
#endif

/* For writev() */
#include <sys/uio.h>

#endif /* nspr_haiku_defs_h___ */
