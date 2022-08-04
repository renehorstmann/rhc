#ifndef RHC_EXPORT_H
#define RHC_EXPORT_H

//
// to mark functions as library export function
//

#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define RHC_EXPORT __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define RHC_EXPORT __attribute__ ((visibility ("default")))
#else
#define RHC_EXPORT
#endif

#endif //RHC_EXPORT_H
