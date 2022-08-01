#ifndef RHC_ERROR_H
#define RHC_ERROR_H

//
// error managment
//      used for files and sockets for example.
//

// returns the current error from the calling thread, or NULL if no error occured
RHC_EXPORT
const char *rhc_error_get();

// clears the error of the calling thread
RHC_EXPORT
void rhc_error_clear();

// sets the error of the calling thread and logs it as error
// if rhc_error_get != NULL, the old error is logged as error before the reset
// the given error should be static data:
//      rhc_error_set("File not found");
RHC_EXPORT
void rhc_error_set(const char *static_data_error);

// does not overwrite an existing error
// the given error should be static data:
//      rhc_error_set_try("File not found");
static void rhc_error_set_try(const char *static_data_error) {
    if(rhc_error_get())
        return;
    rhc_error_set(static_data_error);
}

#endif //RHC_ERROR_H
