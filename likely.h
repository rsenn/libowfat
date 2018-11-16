#ifdef __dietlibc__
# include <sys/cdefs.h>
#else
# ifdef __GNUC__
#  if __GNUC__ < 3
#   define __expect(foo,bar) (foo)
#  else
#   define __expect(foo,bar) __builtin_expect((long)(foo),bar)
#  endif
#  define __likely(foo) __expect((foo),1)
#  define __unlikely(foo) __expect((foo),0)
# else
#  define __likely(foo) (foo)
#  define __unlikely(foo) (foo)
# endif
#endif
