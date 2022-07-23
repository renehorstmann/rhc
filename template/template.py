import os
import re


#
# This file generates new namespaces for the rhc library
#


def regex_name_prefix(name_prefix: str):
    """selects a name with unknown end in a C file"""
    return r'(?<![a-zA-Z0-9_])' + name_prefix


def apply_replace(src_file, dst_file, template: dict):
    """reads in the src file, applies the replace list and saves the generated file under dst (and creates all dirs)"""
    file = open(src_file, 'r')
    text = file.read()
    file.close()

    for item in template:
        text = re.sub(item, template[item], text)

    os.makedirs(os.path.dirname(dst_file), exist_ok=True)
    file = open(dst_file, 'w')
    file.write(text)
    file.close()

#
# Templates
#

# the templates are defined as from to maps
# lib = library name (rhc dir + rhc_impl.h, ...)
# RHC = macro (RHC_ALLOCATOR_DEFAULT)
# rhc_ = (macro) function (rhc_new)
# rhc = datatypes (rhcsize, rhcf32)
# Rhc = classes (RhcString)

# changes nothing
RHC_lib = 'rhc'
RHC = {
    'RHC': 'RHC',
    'rhc_': 'rhc_',
    'rhc': 'rhc',
    'Rhc': 'Rhc',
}

# for the 's'ome framework
S_lib = 's'
S = {
    'RHC': 'S',
    'rhc_': 's_',
    'rhc': 's',
    'Rhc': 's',
}

if __name__ == '__main__':

    # set the template here
    lib = S_lib
    T = S

    print('applying template...')
    import shutil

    if os.path.exists('out'):
        shutil.rmtree('out')

    dir = 'out/' + lib + '/'
    dir_impl = dir + 'impl/'
    os.makedirs(dir_impl)

    shutil.copyfile('in/rhc/LICENSE', dir + 'LICENSE')

    def apply_impl(file):
        apply_replace('in/rhc/impl/'+file, dir_impl+file, T)
        
    def apply(file):
        apply_replace('in/rhc/'+file, dir+file, T)

    apply_impl('allocator_impl.h')
    apply_impl('assume_impl.h')
    apply_impl('error_impl.h')
    apply_impl('file_impl.h')
    apply_impl('log_impl.h')
    apply_impl('memory_impl.h')
    apply_impl('socket_impl.h')
    apply_impl('str_impl.h')
    apply_impl('stream_impl.h')
    apply_impl('string_impl.h')
    apply_impl('terminalcolor_impl.h')
    apply_impl('time_impl.h')
    
    apply('allocator.h')
    apply('assume.h')
    apply('common.h')
    apply('dynarray.h')
    apply('endian.h')
    apply('error.h')
    apply('file.h')
    apply('hashmap.h')
    apply('hashmap_string.h')
    apply('hashmap_trivial.h')
    apply('log.h')
    apply('memory.h')
    apply('socket.h')
    apply('str.h')
    apply('str_parse.h')
    apply('str_type.h')
    apply('stream.h')
    apply('string.h')
    apply('terminalcolor.h')
    apply('time.h')

    apply_replace('in/rhc/rhc.h', dir+lib+'.h', T)
    apply_replace('in/rhc/rhc_full.h', dir+lib+'_full.h', T)
    apply_replace('in/rhc/rhc_impl.h', dir+lib+'_impl.h', T)

    print('finish')