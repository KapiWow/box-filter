project('box-filter', 'c', 'cpp')

c = meson.get_compiler('c')

common = files([
  'box_filter.c',
  'box_filter_simd.cc',
])

src = files([
  'main.cc',
]) + common

gtest = dependency(
  'gtest',
  main: true
)

lib = [
  dependency('opencv'),
  dependency('openmp'),
]

cpp_arg = [
    '-O3',
    '-march=native',
    '-g',
    '-ffast-math',
]

c_arg = cpp_arg

cpp_arg += [
  '-std=c++17',
]

executable(
  'box_filter',
  sources: src,
  dependencies: lib,
  cpp_args: cpp_arg,
  c_args: c_arg,
)

test_name = [
  'box_filter_test',
  'simd_test',
]

test_src = files([
  'box_filter_test.cc',
  'simd_test.cc',
])

foreach name : test_name
  test(
    name,
    executable(
      name,
      sources: [name + '.cc'] + common,
      dependencies: [gtest] + lib,
      cpp_args: cpp_arg,
      c_args: c_arg,
    )
  )
endforeach
