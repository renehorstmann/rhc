# rhc
A new approach as a C standard library addition.


## WARNING:
In active development


## Components
- error
  - set_error
  - get_error
  - valid style
  - assume
      - runtime asserts
- alloc
  - allocator
- Buffer
  - to return loaded data?
- str
  - view on a string
- String
  - holds a string with an allocator
- log
- time
  - monotonic, etc.
- test
  - calls exit(EXIT_FAILURE) for conditions
- DynArray
- HashMap
- scope macros
- iterator?
- sdl option (macrk def? OPTION_SDL)


## TODO:
- str -> Str_s
- String_* -> string_*
- String { union { Str_s str, struct { char *data, size}
- every function must handle not valid input
  - assert(allocator_valid)
  - if(!*_valid) return *_new_invalid()
- alloc?
  - alloc_new_empty
  - alloc_new_zero
  - alloc_renew
  - alloc_delete?
- log sdl
- file write (bool ascii)
- file append
- str from Utilc/strviu & strviu_parse
