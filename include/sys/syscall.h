/*
 * sys/syscall.h
 *
 * Generic system call interface macros
 */
#ifndef _SYS_SYSCALL_H

#include <errno.h>
#include <asm/unistd.h>

/*
 * If we're compiling i386 in PIC mode, we need to treat %ebx
 * specially.  Most of these are copied from the equivalent file in
 * newlib and were written by Werner Almesberger.
 */
#if defined(__PIC__) && defined(__i386__)

/* _syscall0() is the same as non-PIC */

#undef _syscall1
#define _syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
long __res; \
__asm__ __volatile__ ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1))); \
__syscall_return(type,__res); \
}

#undef _syscall2
#define _syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
long __res; \
__asm__ __volatile__ ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1)),"c" ((long)(arg2))); \
__syscall_return(type,__res); \
}

#undef _syscall3
#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
long __res; \
__asm__ __volatile__ ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1)),"c" ((long)(arg2)), \
		"d" ((long)(arg3))); \
__syscall_return(type,__res); \
}

#undef _syscall4
#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
long __res; \
__asm__ __volatile__ ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4))); \
__syscall_return(type,__res); \
}

#undef _syscall5
#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
long __res; \
__asm__ __volatile__ ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"m" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5))); \
__syscall_return(type,__res); \
}

#undef _syscall6
#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
__asm__ __volatile__ ("push %%ebx; pushl %%ebp; movl %2,%%ebx; " \
                      "movl %%eax,%%ebp; movl %1,%%eax; int $0x80; " \
                      "pop %%ebp ; pop %%ebx" \
	: "=a" (__res) \
	: "i" (__NR_##name),"m" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5)), \
	  "a" ((long)(arg6))); \
__syscall_return(type,__res); \
}

#elif defined(__alpha__)

/* Alpha has some bizarre Tru64-derived system calls which return two
   different values in $0 and $20(!), respectively.  The standard
   macros can't deal with these; even the ones that give the right
   return value have the wrong clobbers. */

#define _syscall0_dual0(type, name)                                     \
type name(void)                                                         \
{                                                                       \
        long _sc_ret, _sc_err;                                          \
        {                                                               \
                register long _sc_0 __asm__("$0");                      \
                register long _sc_19 __asm__("$19");                    \
                register long _sc_20 __asm__("$20");                    \
                                                                        \
                _sc_0 = __NR_##name;                                    \
                __asm__("callsys"                                       \
                        : "=r"(_sc_0), "=r"(_sc_19), "=r" (_sc_20)      \
                        : "0"(_sc_0)                                    \
                        : _syscall_clobbers);                           \
                _sc_ret = _sc_0, _sc_err = _sc_19; (void)(_sc_20);      \
        }                                                               \
        _syscall_return(type);                                          \
}

#define _syscall0_dual1(type, name)                                     \
type name(void)                                                         \
{                                                                       \
        long _sc_ret, _sc_err;                                          \
        {                                                               \
                register long _sc_0 __asm__("$0");                      \
                register long _sc_19 __asm__("$19");                    \
                register long _sc_20 __asm__("$20");                    \
                                                                        \
                _sc_0 = __NR_##name;                                    \
                __asm__("callsys"                                       \
                        : "=r"(_sc_0), "=r"(_sc_19), "=r" (_sc_20)      \
                        : "0"(_sc_0)                                    \
                        : _syscall_clobbers);                           \
                _sc_ret = _sc_20, _sc_err = _sc_19; (void)(_sc_0);      \
        }                                                               \
        _syscall_return(type);                                          \
}

#elif defined(__sparc64__)

/* SPARC64 seems to lack _syscall6() in its headers */

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
  type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
register long __g1 __asm__ ("g1") = __NR_##name; \
register long __o0 __asm__ ("o0") = (long)(arg1); \
register long __o1 __asm__ ("o1") = (long)(arg2); \
register long __o2 __asm__ ("o2") = (long)(arg3); \
register long __o3 __asm__ ("o3") = (long)(arg4); \
register long __o4 __asm__ ("o4") = (long)(arg5); \
register long __o5 __asm__ ("o5") = (long)(arg6); \
__asm__ __volatile__ ("t 0x6d\n\t" \
      "sub %%g0, %%o0, %0\n\t" \
      "movcc %%xcc, %%o0, %0\n\t" \
      : "=r" (__res), "=&r" (__o0) \
      : "1" (__o0), "r" (__o1), "r" (__o2), "r" (__o3), "r" (__o4), "r" (__o5), "r" (__g1) \
      : "cc"); \
if (__res>=0) \
return (type) __res; \
errno = -__res; \
return (type)-1; \
}

#elif defined(__sparc__)

/* SPARC64 seems to lack _syscall6() in its headers */

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
  type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
register long __g1 __asm__ ("g1") = __NR_##name; \
register long __o0 __asm__ ("o0") = (long)(arg1); \
register long __o1 __asm__ ("o1") = (long)(arg2); \
register long __o2 __asm__ ("o2") = (long)(arg3); \
register long __o3 __asm__ ("o3") = (long)(arg4); \
register long __o4 __asm__ ("o4") = (long)(arg5); \
register long __o5 __asm__ ("o5") = (long)(arg6); \
__asm__ __volatile__ ("t 0x10\n\t" \
      "bcc 1f\n\t" \
      "mov %%o0, %0\n\t" \
      "sub %%g0, %%o0, %0\n\t" \
      "1:\n\t" \
      : "=r" (__res), "=&r" (__o0) \
      : "1" (__o0), "r" (__o1), "r" (__o2), "r" (__o3), "r" (__o4), "r" (__o5), "r" (__g1) \
      : "cc"); \
if (__res < -255 || __res>=0) \
return (type) __res; \
errno = -__res; \
return (type)-1; \
}

#elif defined(__powerpc__)

/* PowerPC seems to lack _syscall6() in its headers */
/* This seems to work on both 32- and 64-bit ppc */

#ifndef _syscall6

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
        unsigned long __sc_ret, __sc_err;                               \
        {                                                               \
                register unsigned long __sc_0 __asm__ ("r0");           \
                register unsigned long __sc_3 __asm__ ("r3");           \
                register unsigned long __sc_4 __asm__ ("r4");           \
                register unsigned long __sc_5 __asm__ ("r5");           \
                register unsigned long __sc_6 __asm__ ("r6");           \
                register unsigned long __sc_7 __asm__ ("r7");           \
                register unsigned long __sc_8 __asm__ ("r8");           \
                                                                        \
                __sc_3 = (unsigned long) (arg1);                        \
                __sc_4 = (unsigned long) (arg2);                        \
                __sc_5 = (unsigned long) (arg3);                        \
                __sc_6 = (unsigned long) (arg4);                        \
                __sc_7 = (unsigned long) (arg5);                        \
                __sc_8 = (unsigned long) (arg6);                        \
                __sc_0 = __NR_##name;                                   \
                __asm__ __volatile__                                    \
                        ("sc           \n\t"                            \
                         "mfcr %1      "                                \
                        : "=&r" (__sc_3), "=&r" (__sc_0)                \
                        : "0"   (__sc_3), "1"   (__sc_0),               \
                          "r"   (__sc_4),                               \
                          "r"   (__sc_5),                               \
                          "r"   (__sc_6),                               \
                          "r"   (__sc_7),                               \
                          "r"   (__sc_8)                                \
                        : __syscall_clobbers);                          \
                __sc_ret = __sc_3;                                      \
                __sc_err = __sc_0;                                      \
        }                                                               \
        __syscall_return (type);                                        \
}

#endif /* _syscall6() missing */

#elif defined(__s390x__)

/* S/390X only has five syscall parameters, and uses a structure for
   6-argument syscalls. */

#ifndef _syscall6

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,\
                  type4,arg4,type5,arg5,type6,arg6)          \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4,    \
          type5 arg5, type6 arg6) {			     \
	unsigned long  __arg[6] = {			     \
		(unsigned long) arg1, 			     \
		(unsigned long) arg2, 			     \
		(unsigned long) arg3, 			     \
		(unsigned long) arg4, 			     \
		(unsigned long) arg5,			     \
		(unsigned long) arg6 			     \
	};						     \
	register void *__argp asm("2") = &__arg;	     \
	long __res;					     \
	__asm__ __volatile__ (               	             \
                "    svc %b1\n"                              \
                "    lgr  %0,2"                              \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__argp)				     \
		: _svc_clobber);			     \
	__syscall_return(type, __res);			     \
}

#endif /* _syscall6() missing */

#elif defined(__s390__)

/* S/390 only has five syscall parameters, and uses a structure for
   6-argument syscalls. */

#ifndef _syscall6

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,\
                  type4,arg4,type5,arg5,type6,arg6)          \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4,    \
          type5 arg5, type6 arg6) {			     \
	unsigned long  __arg[6] = {			     \
		(unsigned long) arg1, 			     \
		(unsigned long) arg2, 			     \
		(unsigned long) arg3, 			     \
		(unsigned long) arg4, 			     \
		(unsigned long) arg5,			     \
		(unsigned long) arg6 			     \
	};						     \
	register void *__argp asm("2") = &__arg;	     \
	long __res;					     \
	__asm__ __volatile__ (               	             \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__argp)				     \
		: _svc_clobber);			     \
	__syscall_return(type, __res);			     \
}

#endif /* _syscall6() missing */

#endif

#endif /* SYSCALL_H */
