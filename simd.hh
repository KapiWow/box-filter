#ifndef SIMD_HH
#define SIMD_HH

#include <iostream>

template<class T, int N>
class alignas(N * sizeof(T)) Simd {
public:
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

    Simd operator+ (const Simd& rhs) {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] + rhs.data[i];
        }
        return temp;
    }
    
    Simd operator+ (const int rhs) {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] + rhs;
        }
        return temp;
    }
    
    Simd operator* (const Simd& rhs) {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] * rhs.data[i];
        }
        return temp;
    }
    
    Simd operator* (const int rhs) {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] * rhs;
        }
        return temp;
    }

    Simd operator>> (int n) {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] >> n;
        }
        return temp;
    }

    Simd operator<< (int n) {
        Simd temp;
        for (int i = 0; i < N; i++) {
            temp[i] = data[i] << n;
        }
        return temp;
    }

    void read(const T* in) {
        std::copy(in, in + N, data);
    }

    void write(T* out) {
        std::copy(data, data + N, out);
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
typedef Simd<uint16_t, 16> uint16_t_16;

#endif
