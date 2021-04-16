# rhc
A new approach as a C standard library.


## WARNING:
In active development

## Hints
Functions may either take str or const char*
valid system

## Components
- types
- error
  - set_error
  - get_error
  - valid style
  - assume
      - runtime asserts
- alloc
  - allocator
- str
  - size + not be zero terminated
  - view on a string
  - parse
- String
  - size + always zero terminated
  - holds a string with an allocator
- file
  - file to String
  - str to file
- log
- time
  - monotonic, etc.
- DynArray
- HashMap
- scope macros
- iterator?
- sdl option (OPTION_SDL)
