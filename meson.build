project(
  'mSDK-core',
  'cpp',
  version: '0.1.0',
  default_options: [
    'warning_level=0',  # TODO: Cleanup Fusion headers so no warnings are shown when compiling with warning_level=3
    'cpp_std=c++17'
  ],
  # license: 'MIT' i actualy dont know FusionSDK license
)




if meson.get_compiler('cpp').get_id() != 'msvc'
  warning('Compilers other than MSVC are currently unsupported, Fusion may not like that you\'re not using MSVC.')
endif

if build_machine.cpu() != 'x86'
  warning('Fusion requires extension to be x86 (32bit) DLL, current compiler is for ' + build_machine.cpu() + ' cpus.')
endif



# Compiler arguments for each target
common   = ['-DHWABETA']
edittime = common + ['-DEDITTIME']
runtime  = common + ['-DRUNTIME']
unicode  = ['-DUNICODE', '-D_UNICODE']


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
