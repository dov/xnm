# meson --buildtype debug build_debug
project('libxnm','c',
        version: '0.1.3',
        default_options : [])

glib_dep = dependency('glib-2.0')
bison = find_program('bison', 'yacc', 'byacc')
parse = generator(bison,
    output : [ '@BASENAME@_bison.c', '@BASENAME@_bison.h' ],
    arguments : [
        '-d', '@INPUT@', '-v',
        '--output=@OUTPUT0@',
        '--defines=@OUTPUT1@'
    ]
)

subdir('src')

libxnm_dep = declare_dependency(
  include_directories : 'src',
  link_with : libxnm_lib)

subdir('tests')

# xnm.pc
pkg = import('pkgconfig')
if not meson.is_subproject()
  pkg.generate(libxnm_lib,
    name: 'libxnm',
    filebase: 'libxnm',
    description: 'A XNM parsing library',
    version: meson.project_version())
endif
