#ifndef SIMD_HH
#define SIMD_HH

#include <iostream>
#include <omp.h>

template<class T, int N>
class alignas(N * sizeof(T)) Simd {
public:
    Simd(T *in) {
        read(in);
    }

    Simd() {
        for (int i = 0; i < N; i++) {
            data[i] = T{0};
        }
    }
    
    Simd(const Simd& s) {
        *this = s;
    }

    Simd(const Simd&& s) {
        *this = s;
    }

    Simd& operator=(const Simd& rhs) {
		Simd temp(rhs);
		this->swap(temp);
		return *this;
    }

    Simd& operator=(Simd&& rhs) {
        this->swap(rhs);
        return *this;
    }

    T& operator[] (int n) { return data[n]; }
    const T& operator[] (int n) const { return data[n]; }

    Simd& operator+=(Simd&& rhs) {
#pragma omp simd
        for (int i = 0; i < N; i++) {
            data[i] = data[i] + rhs[i];
        }
        return *this;
    }

    Simd& operator+=(const Simd& rhs) {
#pragma omp simd
        for (int i = 0; i < N; i++) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    Simd& operator-=(const Simd& rhs) {
#pragma omp simd
        for (int i = 0; i < N; i++) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }
    
    Simd operator-(const Simd& rhs) const {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] - rhs[i];
        }
        return temp;
    }
    
    Simd&& operator-(Simd&& rhs) {
#pragma omp simd
        for (int i = 0; i < N; i++) {
            rhs[i] = data[i] - rhs[i];
        }
        return std::move(rhs);
    }

    Simd operator+(const Simd& rhs) const {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] + rhs.data[i];
        }
        return temp;
    }
    
    Simd operator+(const int rhs) const {
        Simd temp;
#pragma omp simd
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] + rhs;
        }
        return temp;
    }
    
    Simd operator* (const Simd& rhs) const {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] * rhs.data[i];
        }
        return temp;
    }
    
    Simd& operator/=(const int rhs) {
#pragma omp simd
        for (int i = 0; i < N; i++) {
            data[i] /= rhs;
        }
        return *this;
    }
    
    void read(const T* in) {
        std::copy(in, in + N, data);
    }

    void write(T* out) {
        std::copy(data, data + N, out);
    }

    void zero() {
        for (int i = 0; i < N; i++) {
            data[i] = 0;
        }
    }

    T data[N * sizeof(T)];

private:

    void swap( Simd& s) {
        std::swap(data, s.data);
    }

};

typedef Simd<int, 8> int_8;
typedef Simd<float, 8> float_8;
typedef Simd<int, 4> int_4;
typedef Simd<float, 4> float_4;
typedef Simd<uint8_t, 32> uint8_t_32;
typedef Simd<uint8_t, 16> uint8_t_16;
typedef Simd<uint16_t, 16> uint16_t_16;
typedef Simd<uint32_t, 8> uint32_t_8;
typedef Simd<uint32_t, 16> uint32_t_16;

inline void convert_u16_u8_16(const uint16_t_16& from, uint8_t_16& to) {
#pragma omp simd
    for (int i = 0; i < 16; i++) {
        to[i] = (uint8_t)from[i];
    }
}

inline void convert_u8_u16_16(const uint8_t_16& from, uint16_t_16& to) {
#pragma omp simd
    for (int i = 0; i < 16; i++) {
        to[i] = (uint16_t)from[i];
    }
}

#endif
