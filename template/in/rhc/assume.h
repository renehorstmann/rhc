#ifndef RHC_ASSUME_H
#define RHC_ASSUME_H

#include "export.h"

// assert like function, that also uses formatting print to stderr.
// Calls raise(SIG_ABRT).
// If NDEBUG is defined, only msg will get displayed (without expression, file and line infos)
#define rhc_assume(EX, ...) \
(void)((EX) || (rhc__rhc_assume_impl (#EX, __FILE__, __LINE__, __VA_ARGS__),0))

RHC_EXPORT
void rhc__rhc_assume_impl(const char *expression, const char *file, int line, const char *format, ...);

#endif //RHC_ASSUME_H
