#ifndef RHC_RHC_H
#define RHC_RHC_H

//
// includes the basics of rhc
//      define RHC_IMPL once in a single source file!
//      or include rhc_impl.h in a single source file
//

#include "common.h"
#include "export.h"
#include "error.h"
#include "assume.h"
#include "memory.h"
#include "log.h"

#ifdef RHC_IMPL
#include "impl/allocator_impl.h"
#include "impl/assume_impl.h"
#include "impl/error_impl.h"
#include "impl/file_impl.h"
#include "impl/log_impl.h"
#include "impl/memory_impl.h"
#include "impl/socket_impl.h"
#include "impl/str_impl.h"
#include "impl/stream_impl.h"
#include "impl/string_impl.h"
#include "impl/terminalcolor_impl.h"
#include "impl/time_impl.h"
#endif

#endif //RHC_RHC_H
