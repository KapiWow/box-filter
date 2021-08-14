#include <gtest/gtest.h>
#include <simd.hh>

static const int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
static const int arr2[] = {8, 7, 6, 5, 4, 3, 2, 1};

TEST (simd, int_8_read)
{
    int_8 a;
    a.read(arr1);
    for (int i = 0; i != 8; i++) {
        ASSERT_EQ(a[i], i + 1);
    }
}

TEST (simd, int_8_write)
{
    int_8 a;
    a.read(arr1);
    int b[8];
    a.write(b);
    for (int i = 0; i != 8; i++) {
        ASSERT_EQ(b[i], i + 1);
    }
}

TEST (simd, int_8_add)
{
    int_8 a, b;
    a.read(arr1);
    b.read(arr2);
    int_8 c = a + b;
    for (int i = 0; i != 8; i++) {
        ASSERT_EQ(c[i], arr1[i] + arr2[i]);
    }
}

TEST (simd, int_8_add_scalar)
{
    int_8 a;
    a.read(arr1);
    int_8 b = a + 1;
    for (int i = 0; i != 8; i++) {
        ASSERT_EQ(b[i], arr1[i] + 1);
    }
}
