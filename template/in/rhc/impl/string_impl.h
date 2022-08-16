#ifndef RHC_STRING_IMPL_H
#define RHC_STRING_IMPL_H
#ifdef RHC_IMPL

static rhcsize rhc__string_stream_read(struct RhcStream_i *stream, void *data, rhcsize len);
static rhcsize rhc__string_stream_write(struct RhcStream_i *stream, const void *data, rhcsize len);
static bool rhc__string_stream_valid(struct RhcStream_i *stream);



// allocated start_capacity + 1 (null)
RhcString *rhc_string_new_a(rhcsize start_capacity, RhcAllocator_i a) {
    rhc_assume(rhc_allocator_valid(a), "a needs to be valid");
    RhcString *self = rhc_a_new0(a, RhcString, 1);
    // at least 8 bytes to be allocated
    start_capacity = rhc_max(7, start_capacity);
    self->data = rhc_a_malloc0(a, start_capacity + 1);
    self->capacity = start_capacity;
    self->allocator = a;
    self->stream = (RhcStream_i) {
            .impl = self,
            .opt_read_try = rhc__string_stream_read,
            .opt_write_try = rhc__string_stream_write,
            .valid = rhc__string_stream_valid
    };
    return self;
}

RhcString *rhc_string_new_clone_a(RhcStr_s to_clone, RhcAllocator_i a) {
    RhcString *sb = rhc_string_new_a(to_clone.size, a);
    if (!rhc_string_valid(sb))
        return sb;
    memcpy(sb->data, to_clone.data, to_clone.size);
    sb->size = to_clone.size;
    return sb;
}

RhcString *rhc_string_new_replace_a(RhcStr_s to_replace, RhcStr_s old, RhcStr_s replacement, RhcAllocator_i a) {
    if (rhc_str_empty(to_replace) || rhc_str_empty(old) || !rhc_str_valid(replacement)) {
        return rhc_string_new_invalid();
    }
    rhcsize cnt = rhc_str_count_str(to_replace, old);
    if (cnt <= 0) {
        return rhc_string_new_clone_a(to_replace, a);
    }
    rhcsize size = to_replace.size - cnt * old.size + cnt * replacement.size;
    RhcString *res = rhc_string_new_a(size, a);
    res->size = size;
    RhcStr_s str = rhc_string_get_str(res);
    str = rhc_str_replace_str_into(str, to_replace, old, replacement);
    res->data = str.data;
    res->size = str.size;
    return res;
}

RhcString *rhc_string_new_cat_a(RhcStr_s *strs, int n, RhcAllocator_i a) {
    rhcsize size = 0;
    for (int i = 0; i < n; i++) {
        size += rhc_str_empty(strs[i]) ? 0 : strs[i].size;
    }
    RhcString *res = rhc_string_new_a(size, a);
    if (!rhc_string_valid(res))
        return res;

    for (int i = 0; i < n; i++) {
        if (!rhc_str_empty(strs[i])) {
            rhc_str_cpy(strs[i], (RhcStr_s) {res->data + res->size, strs[i].size});
            res->size += strs[i].size;
        }
    }
    return res;
}

void rhc_string_kill(RhcString **self_ptr) {
    RhcString *self = *self_ptr;
    if(!rhc_string_valid(self))
        return;
    rhc_a_free(self->allocator, self->data);
    rhc_a_free(self->allocator, self);
    *self_ptr = NULL;
}

void rhc_string_set_capacity(RhcString *self, rhcsize capacity) {
    if (!rhc_string_valid(self))
        return;

    void *data = rhc_a_realloc(self->allocator, self->data, capacity + 1);
    if (!data) {
        rhc_string_kill(&self);
        return;
    }
    self->data = data;
    self->capacity = capacity;
    memset(&self->data[self->size], 0, self->capacity + 1 - self->size);
}

void rhc_string_resize(RhcString *self, rhcsize size) {
    if (size > self->capacity) {
        rhc_string_set_capacity(self, size * 2);
    }
    if (!rhc_string_valid(self))
        return;
    self->size = size;
    self->data[self->size] = '\0';  //just to be sure
}

void rhc_string_push(RhcString *self, char push) {
    if (!rhc_string_valid(self))
        return;
    rhc_string_resize(self, self->size + 1);
    self->data[self->size - 1] = push;
    self->data[self->size] = '\0';  //just to be sure
}

void rhc_string_append(RhcString *self, RhcStr_s append) {
    if (!rhc_string_valid(self))
        return;
    rhcsize prev_size = self->size;
    rhc_string_resize(self, self->size + append.size);
    memcpy(self->data + prev_size, append.data, append.size);
    self->data[self->size] = '\0';  //just to be sure
}



static rhcsize rhc__string_stream_read(struct RhcStream_i *stream, void *data, rhcsize len) {
    RhcString *self = stream->impl;
    if(!rhc_string_valid(self) || len <=0)
        return 0;
    rhcsize s = rhc_min(self->size - self->stream_pos, len);
    memcpy(data, self->data+self->stream_pos, s);
    self->stream_pos+=s;
    return s;
}
static rhcsize rhc__string_stream_write(struct RhcStream_i *stream, const void *data, rhcsize len) {
    RhcString *self = stream->impl;
    if(!rhc_string_valid(self) || len <=0)
        return 0;
    rhc_string_append(self, (RhcStr_s) {(char *) data, len});
    return len;
}
static bool rhc__string_stream_valid(struct RhcStream_i *stream) {
    RhcString *self = stream->impl;
    return rhc_string_valid(self);
}

#endif //RHC_IMPL
#endif //RHC_STRING_IMPL_H
