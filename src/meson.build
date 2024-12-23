bison_output = parse.process('xnm-parser.y')

libxnm_sources = files(
  'xnm-struct.c',
  'xnm-retrieve.c'
)

deps = [glib_dep]
libxnm_inc = ['.']

libxnm_lib = library('xnm',
                     libxnm_sources,
                     bison_output,
                     include_directories: libxnm_inc,
                     dependencies : deps,    
                     install: not meson.is_subproject())

install_headers('xnm.h', subdir: 'libxnm')

