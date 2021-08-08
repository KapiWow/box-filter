project('box-filter', 'c', 'cpp')

c = meson.get_compiler('c')

src = files([
  'main.cc',
  'box_filter.c',
])

gtest = dependency(
  'gtest',
  main: true
)

lib = [
  dependency('opencv'),
]

cpp_arg = [
    '-O3',
    '-march=native',
    '-g',
    '-ffast-math',
]

c_arg = cpp_arg

executable(
  'box_filter',
  sources: src,
  dependencies: lib,
  cpp_args: cpp_arg,
  c_args: c_arg,
)

test_src = files([
  'box_filter_test.cc'
])

test(
  'box_filter_test',
  executable(
    'box_filter_test',
    sources: test_src,
    dependencies: [gtest]
  )
)
