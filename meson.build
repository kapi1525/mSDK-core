project(
  'mSDK-core',
  'cpp',
  version: '0.1.0',
  default_options: [
    'warning_level=3',  # TODO: Cleanup Fusion headers so no warnings are shown when compiling with warning_level=3
    'cpp_std=c++17'
  ],
  license: 'MIT'        # i actualy dont know FusionSDK license
)



common_defines = [
  'HWABETA',
]

edittime_defines = [
  'EDITTIME',
]

runtime_defines = [
  'RUNTIME',
]

unicode_defines = [
  'UNICODE',
  '_UNICODE',
]



# Compiler arguments specific to MSVC
common_msvc_args = [
  '/wd4100',                # Supress 0 size array warning
  '/wd4200',                # Supress unused function parameter warning
  '/wd4201',                # Supress nameless enum and struct warnings
  '/Zc:wchar_t-',           # Dont treat wchar_t as builtin type
]


# Compiler arguments specific to clang-cl
common_clang_cl_args = [
  '-Wno-unused-parameter',  # Supress unused function parameter warning
  '-fms-compatibility',     # Enable microsoft comparibility
  '-fms-extensions',        # Enable microsoft language extensions
]
# Things currently breaking clang support:
# https://github.com/llvm/llvm-project/issues/30933



common = []
edittime = []
runtime = []
unicode = []

define_flag = []


compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'msvc'
  common += common_msvc_args
  define_flag = '/D'
elif compiler.get_id() == 'clang-cl'
  common += common_clang_cl_args
  define_flag = '-D'
  warning('clang-cl support is unfinished, expect errors.')
else
  warning('Unsuported compiler "' + compiler.get_id() + '", expect errors.')
endif


if host_machine.cpu_family() != 'x86'
  warning('Fusion requires extension to be x86 (32bit) DLL, current compiler is for ' + host_machine.cpu() + ' cpus, expect errors.')
endif



foreach define : common_defines
  common += define_flag + define
endforeach

foreach define : edittime_defines
  edittime += define_flag + define
endforeach

foreach define : runtime_defines
  runtime += define_flag + define
endforeach

foreach define : unicode_defines
  unicode += define_flag + define
endforeach



mmfs2 = meson.get_compiler('cpp').find_library(
  'mmfs2',
  dirs: meson.project_source_root() / 'lib',
  required: true
)


mSDK_include = include_directories('inc')

mSDK = declare_dependency(
  dependencies : mmfs2,
  include_directories : mSDK_include,
)

meson.override_dependency('mSDK', mSDK)
