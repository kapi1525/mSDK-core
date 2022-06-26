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
edititme_args = ['-DHWABETA']                 # TODO: Add EDITTIME macro
runtime_args  = ['-DHWABETA', '-DRUN_ONLY']     # TODO: Repleace RUN_ONLY with RUNTIME
unicode_args  = ['-DUNICODE', '-D_UNICODE']     # Both UNICODE and _UNICODE are required, first one is used by WinAPI and second is used by CRT (C Runtime Library)



mmfs2 = meson.get_compiler('cpp').find_library(
  'mmfs2',
  dirs: meson.project_source_root() / 'lib',
  required: true
)

SDK = declare_dependency(
  dependencies : mmfs2,
  include_directories : include_directories('inc'),
)

meson.override_dependency('SDK', SDK)