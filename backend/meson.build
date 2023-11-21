project(
    'raincloud',
    'cpp',
    version: '0.1.1',
    license: 'MPL-2.0',
    default_options: ['cpp_std=c++17'],
)

source_files = [
    'src/main.cpp',
]

dependencies = [
    dependency('oatpp'),
    dependency('libtorrent-rasterbar'),
]

executable(
    meson.project_name(),
    source_files,
    dependencies: dependencies,
)
