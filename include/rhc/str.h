#ifndef RHC_STR_H
#define RHC_STR_H

//
// functions working on a data view
//

#include "error.h"
#include "log.h"
#include "str_type.h"

struct RhcStream_i;

//
// Functions
//


// RhcStr_s from a c string
static RhcStr_s rhc_strc(const char *cstring) {
    return (RhcStr_s) {(char *) cstring, strlen(cstring)};
}

// returns true if the str is empty
static bool rhc_str_empty(RhcStr_s s) {
    return !s.data || s.size <= 0;
}

// prints the str to a stream
// as f.e.: "(RhcStr_s) {"hello world", 11}\n"
// returns the amount of printed characters
rhcsize rhc_str_stream_print(RhcStr_s str, struct RhcStream_i stream);

// prints the str to stdoud
// as f.e.: "(RhcStr_s) {"hello world", 11}\n"
// returns the amount of printed characters
rhcsize rhc_str_print(RhcStr_s s);

// copies from into cpy. cpy and from must have the same size
static void rhc_str_cpy(RhcStr_s cpy, RhcStr_s from) {
    if(!rhc_str_valid(cpy) || !rhc_str_valid(from) || cpy.size != from.size) {
        rhc_error_set("rhc_str_cpy failed");
        rhc_log_error("rhc_str_cpy failed: invalid or wrong sizes");
        return;
    }
    memcpy(cpy.data, from.data, cpy.size);
}

// copies from into cpy_buffer. cpy_buffer must have at least the size of from
// returns cpy_buffer with the copied size of from
static RhcStr_s rhc_str_cpy_into(RhcStr_s cpy_buffer, RhcStr_s from) {
    if(!rhc_str_valid(cpy_buffer) || !rhc_str_valid(from) || cpy_buffer.size < from.size) {
        rhc_error_set("rhc_str_cpy_info failed");
        rhc_log_error("rhc_str_cpy_into failed: invalid or wrong sizes");
        return (RhcStr_s) {cpy_buffer.data, 0};
    }
    cpy_buffer.size = from.size;
    rhc_str_cpy(cpy_buffer, from);
    return cpy_buffer;
}

// returns true if the strs are equal
static bool rhc_str_equals(RhcStr_s a, RhcStr_s b) {
    if(!rhc_str_valid(a) || !rhc_str_valid(b) || a.size != b.size)
        return false;
    if(rhc_str_empty(a) && rhc_str_empty(b))
        return true;
    return memcmp(a.data, b.data, a.size) == 0;
}

// out_c_string needs to be a buffer of at least str.size+1
static void rhc_str_as_c(char *out_c_string, RhcStr_s s) {
    if(!rhc_str_valid(s)) {
        out_c_string[0] = '\0';
        return;
    }
    memcpy(out_c_string, s.data, s.size);
    out_c_string[s.size] = '\0';
}

// returns a new allocated buffer with str as cstring
static char *rhc_str_as_new_c_a(RhcStr_s s, RhcAllocator_i a) {
    rhc_assume(rhc_allocator_valid(a), "a needs to be valid");
    if(!rhc_str_valid(s))
        return NULL;
    char *buffer = rhc_a_malloc(a, s.size + 1);
    if(!buffer)
        return NULL;
    rhc_str_as_c(buffer, s);
    return buffer;
}


// returns a new allocated buffer with str as cstring
static char *rhc_str_as_new_c(RhcStr_s s) {
    return rhc_str_as_new_c_a(s, RHC_ALLOCATOR_DEFAULT);
}


// returns true if str begins with cmp
static bool rhc_str_begins_with(RhcStr_s s, RhcStr_s cmp) {
    if(!rhc_str_valid(s) || rhc_str_empty(cmp) || s.size < cmp.size)
        return false;
    return rhc_str_equals(cmp, (RhcStr_s) {s.data, cmp.size});
}

// returns true if str ends with cmp
static bool rhc_str_ends_with(RhcStr_s s, RhcStr_s cmp) {
    if(!rhc_str_valid(s) || rhc_str_empty(cmp) || s.size < cmp.size)
        return false;
    return rhc_str_equals(cmp, (RhcStr_s) {s.data + s.size - cmp.size, cmp.size});
}

// returns a new str, based on s, but without the leading chars of strip (space -> isspace())
static RhcStr_s rhc_str_lstrip(RhcStr_s s, char strip) {
    // invalid safe
    if(strip==' ') {
        while (!rhc_str_empty(s) && isspace(*s.data)) {
            s.data++;
            s.size--;
        }
    } else {
        while (!rhc_str_empty(s) && *s.data == strip) {
            s.data++;
            s.size--;
        }
    }
    return s;
}

// returns a new str, based on s, but without the least chars of strip (space -> isspace())
static RhcStr_s rhc_str_rstrip(RhcStr_s s, char strip) {
    // invalid safe
    if(strip==' ') {
        while (!rhc_str_empty(s) && isspace(s.data[s.size-1])) {
            s.size--;
        }
    } else {
        while (!rhc_str_empty(s) && s.data[s.size-1] == strip) {
            s.size--;
        }
    }
    return s;
}

// returns a new str, based on s, but without the leading and least chars of strip (space -> isspace())
static RhcStr_s rhc_str_strip(RhcStr_s s, char strip) {
    return rhc_str_lstrip(rhc_str_rstrip(s, strip), strip);
}

// a new str, based on s, from the beginning until the first split, or the end
static RhcStr_s rhc_str_next_split(RhcStr_s s, char split) {
    if(rhc_str_empty(s))
        return rhc_str_new_invalid();
    RhcStr_s res = {s.data, 0};
    if (split==' ') {
        while (res.size < s.size && !isspace(res.data[res.size]))
            res.size++;
    } else {
        while (res.size < s.size && res.data[res.size] != split)
            res.size++;
    }
    return res;
}


// returns a str array, based on s, containing all non empty splits between each split (limited autotype version)
static int rhc_str_split(RhcStr_s *splits, int max, RhcStr_s s, char split) {
    // invalid safe
    int n = 0;
    for (;;) {
        s = rhc_str_lstrip(s, split);
        if (rhc_str_empty(s))
            return n;
        RhcStr_s item = rhc_str_next_split(s, split);
        s.data += item.size;
        s.size -= item.size;
        splits[n++] = item;
        if (n >= max) {
            rhc_log_warn("rhc_str_split: reached max: %d/%d", n, max);
            return n;
        }
    }
}

// returns a str array, based on s, containing all non empty splits between each split (allocated version)
static RhcStrArray rhc_str_split_allocated(RhcStr_s s, char split, RhcAllocator_i a) {
    RhcStrArray res = {.allocator = a};
    rhc_assume(rhc_allocator_valid(a), "a needs to be valid");
    for (;;) {
        s = rhc_str_lstrip(s, split);
        if (rhc_str_empty(s))
            return res;
        RhcStr_s item = rhc_str_next_split(s, split);
        s.data += item.size;
        s.size -= item.size;
        RhcStr_s *array = rhc_a_renew(a, RhcStr_s, res.array, res.size + 1);
        if(!array) {
            rhc_log_warn("rhc_str_split_allocated failed: to realloc");
            return res;
        }
        res.array = array;
        res.array[res.size++] = item;
    }
}

// returns the index of the first found char find in str, or -1 if nothing found
static rhcsize rhc_str_find_first(RhcStr_s s, char find) {
    if(rhc_str_empty(s))
        return -1;
    if (find==' ') {
        for(rhcsize i=0; i < s.size; i++) {
            if(isspace(s.data[i]))
                return (rhcsize) i;
        }
    } else {
        for(rhcsize i=0; i < s.size; i++) {
            if(s.data[i] == find)
                return (rhcsize) i;
        }
    }
    return -1;
}


// returns the index of the last found char find in str, or -1 if nothing found
static rhcsize rhc_str_find_last(RhcStr_s s, char find) {
    if(rhc_str_empty(s))
        return -1;
    if (find==' ') {
        for(rhcsize i=s.size; i > 0; i--) {
            if(isspace(s.data[i-1]))
                return (rhcsize) i - 1;
        }
    } else {
        for(rhcsize i=s.size; i > 0; i--) {
            if(s.data[i-1] == find)
                return (rhcsize) i - 1;
        }
    }
    return -1;
}

// returns the index of the first found str found in s, or -1 if nothing found
static rhcsize rhc_str_find_first_str(RhcStr_s s, RhcStr_s find) {
    // invalid safe
    if(find.size > s.size)
        return -1;
    for(rhcsize i=0; i < s.size - find.size; i++) {
        if(rhc_str_equals(find, (RhcStr_s) {s.data + i, find.size}))
            return (rhcsize) i;
    }
    return -1;
}

// returns the index of the last found str found in s, or -1 if nothing found
static rhcsize rhc_str_find_last_str(RhcStr_s s, RhcStr_s find) {
    // invalid safe
    if(find.size > s.size)
        return -1;
    for(rhcsize i= s.size - find.size; i > 0; i--) {
        if(rhc_str_equals(find, (RhcStr_s) {s.data + i - 1, find.size}))
            return (rhcsize) i;
    }
    return -1;
}

// returns the index of the first found char of multiple_chars in str, or -1 if nothing found
static rhcsize rhc_str_find_first_set(RhcStr_s s, const char *char_set) {
    for(rhcsize i=0; i < s.size; i++) {
        for (const char *c = char_set; *c != 0; c++) {
            if ((*c == ' ' && isspace(s.data[i])) || *c == s.data[i])
                return (rhcsize) i;
        }
    }
    return -1;
}
// returns the index of the first found char of multiple_chars in str, or -1 if nothing found
static rhcsize rhc_str_find_last_set(RhcStr_s s, const char *char_set) {
    for(rhcsize i=s.size; i > 0; i--) {
        for (const char *c = char_set; *c != 0; c++) {
            if ((*c == ' ' && isspace(s.data[i-1])) || *c == s.data[i-1])
                return (rhcsize) i - 1;
        }
    }
    return -1;
}

// returns new str, based on s, but missing the first n characters
static RhcStr_s rhc_str_eat(RhcStr_s s, rhcsize n) {
    if(n>=s.size) {
        s.data += s.size;
        s.size = 0;
    } else {
        s.data += n;
        s.size -= n;
    }
    return s;
}

// returns new str, based on s, but missing the last n characters
static RhcStr_s rhc_str_eat_back(RhcStr_s s, rhcsize n) {
    if(n>=s.size) {
        s.size = 0;
    } else {
        s.size -= n;
    }
    return s;
}

// checks if s begins with eat_begin and returns a new str, based on s, without eat_begin at the beginning
// if s does not begin with eat_begin, an invalid str is returned
static RhcStr_s rhc_str_eat_str(RhcStr_s s, RhcStr_s eat_begin) {
    if(!rhc_str_begins_with(s, eat_begin))
        return rhc_str_new_invalid();
    s.data+=eat_begin.size;
    s.size-=eat_begin.size;
    return s;
}

// checks if s ends with eat_back and returns a new str, based on s, without eat_back at the end
// if s does not end with eat_back, an invalid str is returned
static RhcStr_s rhc_str_eat_back_str(RhcStr_s s, RhcStr_s eat_back) {
    if(!rhc_str_ends_with(s, eat_back))
        return rhc_str_new_invalid();
    s.size-=eat_back.size;
    return s;
}

// returns new str, based on s, without every leading char until the char until.
// The cut is set into opt_get
static RhcStr_s rhc_str_eat_until(RhcStr_s s, char until, RhcStr_s *opt_get) {
    RhcStr_s get = s;
    rhcsize pos = rhc_str_find_first(s, until);
    if (pos >= 0) {
        s.data+=pos;
        s.size-=pos;
        get.size=pos;
    } else {
        s.data+=s.size;
        s.size = 0;
    }
    if(opt_get)
        *opt_get = get;
    return s;
}

// returns new str, based on s, without every least char until the char until.
// The cut is set into opt_get
static RhcStr_s rhc_str_eat_back_until(RhcStr_s s, char until, RhcStr_s *opt_get) {
    RhcStr_s get = s;
    rhcsize pos = rhc_str_find_last(s, until);
    if (pos >= 0) {
        s.size=pos;
        get.data+=pos;
        get.size-=pos;
    } else {
        s.size = 0;
    }
    if(opt_get)
        *opt_get = get;
    return s;
}

// returns new str, based on s, without every leading char until the str until.
// The cut is set into opt_get
static RhcStr_s rhc_str_eat_until_str(RhcStr_s s, RhcStr_s until, RhcStr_s *opt_get) {
    RhcStr_s get = s;
    rhcsize pos = rhc_str_find_first_str(s, until);
    if (pos >= 0) {
        s.data+=pos;
        s.size-=pos;
        get.size=pos;
    } else {
        s.data+=s.size;
        s.size = 0;
    }
    if(opt_get)
        *opt_get = get;
    return s;
}

// returns new str, based on s, without every least char until the str until.
// The cut is set into opt_get
static RhcStr_s rhc_str_eat_back_until_str(RhcStr_s s, RhcStr_s until, RhcStr_s *opt_get) {
    RhcStr_s get = s;
    rhcsize pos = rhc_str_find_last_str(s, until);
    if (pos >= 0) {
        s.size=pos;
        get.data+=pos;
        get.size-=pos;
    } else {
        s.size = 0;
    }
    if(opt_get)
        *opt_get = get;
    return s;
}

// returns new str, based on s, without every leading char until a char in set_until
// The cut is set into opt_get
static RhcStr_s rhc_str_eat_until_set(RhcStr_s s, const char *set_until, RhcStr_s *opt_get) {
    RhcStr_s get = s;
    rhcsize pos = rhc_str_find_first_set(s, set_until);
    if (pos >= 0) {
        s.data+=pos;
        s.size-=pos;
        get.size=pos;
    } else {
        s.data+=s.size;
        s.size = 0;
    }
    if(opt_get)
        *opt_get = get;
    return s;
}

// returns new str, based on s, without every least char until a char in set_until.
// The cut is set into opt_get
static RhcStr_s rhc_str_eat_back_until_set(RhcStr_s s, const char *set_until, RhcStr_s *opt_get) {
    RhcStr_s get = s;
    rhcsize pos = rhc_str_find_last_set(s, set_until);
    if (pos >= 0) {
        s.size=pos;
        get.data+=pos;
        get.size-=pos;
    } else {
        s.size = 0;
    }
    if(opt_get)
        *opt_get = get;
    return s;
}

// returns the number of char search, found in str
static rhcsize rhc_str_count(RhcStr_s s, char search) {
    if(rhc_str_empty(s))
        return 0;
    rhcsize cnt = 0;
    if(search==' ') {
        for(rhcsize i=0; i < s.size; i++) {
            if(isspace(s.data[i]))
                cnt++;
        }
    } else {
        for(rhcsize i=0; i < s.size; i++) {
            if(s.data[i] == search)
                cnt++;
        }
    }
    return cnt;
}

// returns the number of str search, found in s
static rhcsize rhc_str_count_str(RhcStr_s s, RhcStr_s search) {
    if(s.size < search.size)
        return 0;
    rhcsize cnt = 0;
    for(rhcsize i=0; i < s.size - search.size; i++) {
        if(rhc_str_equals(search, (RhcStr_s) {s.data + i, search.size}))
            cnt++;
    }
    return cnt;
}

// returns the number of a char in set_search, found in s
static rhcsize rhc_str_count_set(RhcStr_s s, const char *set_search) {
    rhcsize cnt = 0;
    while(*set_search) {
        cnt += rhc_str_count(s, *set_search++);
    }
    return cnt;
}

// replaces each char old with replacement in the str s
static void rhc_str_replace(RhcStr_s s, char old, char replacement) {
    if(rhc_str_empty(s))
        return;
    if(old==' ') {
        for(rhcsize i=0; i < s.size; i++) {
            if(isspace(s.data[i]))
                s.data[i] = replacement;
        }
    } else {
        for(rhcsize i=0; i < s.size; i++) {
            if(s.data[i] == old)
                s.data[i] = replacement;
        }
    }
}

// copies str s into buffer, with old -> replacement.
// the resulting str will be truncated to buffer.size
// returns the buffer with its new size
// have a look at string.h::rhc_string_new_replace_a for an allocated version
static RhcStr_s rhc_str_replace_str_into(RhcStr_s buffer, RhcStr_s s, RhcStr_s old, RhcStr_s replacement) {
    if(rhc_str_empty(buffer) || rhc_str_empty(s) || rhc_str_empty(old) || !rhc_str_valid(replacement))
        return (RhcStr_s) {buffer.data, 0};

    rhcsize b = 0;
    for(rhcsize i=0; i < s.size; i++) {
        if(s.size-i>=old.size && rhc_str_equals(old, (RhcStr_s) {s.data + i, old.size})) {
            rhcsize cpy_size = buffer.size - b >= replacement.size ? replacement.size : buffer.size - b;
            rhc_str_cpy((RhcStr_s) {buffer.data + b, cpy_size}, (RhcStr_s) {replacement.data, cpy_size});
            b += cpy_size;
        } else {
            if(b<buffer.size) {
                buffer.data[b] =s.data[i];
            }
            b++;
        }
    }
    return (RhcStr_s) {buffer.data, b};
}

// runs tolower on each char in s and returns s
static RhcStr_s rhc_str_tolower(RhcStr_s s) {
    if(rhc_str_empty(s)) return s;
    for(rhcsize i=0; i < s.size; i++) {
        s.data[i] = (char) tolower(s.data[i]);
    }
    return s;
}

// runs toupper on each char in s and returns s
static RhcStr_s rhc_str_toupper(RhcStr_s s) {
    if(rhc_str_empty(s)) return s;
    for(rhcsize i=0; i < s.size; i++) {
        s.data[i] = (char) toupper(s.data[i]);
    }
    return s;
}

#endif //RHC_STR_H
