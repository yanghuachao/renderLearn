#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <iostream>
#include <vector>

template <class t>
struct Vec2 {
    t x, y;
    Vec2() : x(t()), y(t()) {}
    Vec2(t _x, t _y) : x(_x), y(_y) {}
    Vec2(const Vec2<t>& v) : x(v.x), y(v.y) {}

    Vec2<t>& operator =(const Vec2<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }

    Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(x + V.x, y + V.y); }
    Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(x - V.x, y - V.y); }
    Vec2<t> operator *(float f)          const { return Vec2<t>(x * f, y * f); }

    t& operator[](const int i) {
        if (i == 0) return x;
        else return y;
    }

    friend std::ostream& operator<<(std::ostream& s, const Vec2<t>& v) {
        s << "(" << v.x << ", " << v.y << ")";
        return s;
    }
};

template <class t>
struct Vec3 {
    t x, y, z;
    Vec3() : x(t()), y(t()), z(t()) {}
    Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}

    template <class u>
    Vec3(const Vec3<u>& v) : x(t(v.x)), y(t(v.y)), z(t(v.z)) {}

    Vec3(const Vec3<t>& v) : x(v.x), y(v.y), z(v.z) {}

    Vec3<t>& operator =(const Vec3<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }

    Vec3<t> operator ^(const Vec3<t>& v) const {
        return Vec3<t>(y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
    Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
    Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
    t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }

    float norm() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3<t>& normalize(t l = 1) {
        float n = norm();
        if (n != 0) *this = (*this) * (l / n);
        return *this;
    }

    t& operator[](const int i) {
        if (i == 0) return x;
        else if (i == 1) return y;
        else return z;
    }

    friend std::ostream& operator<<(std::ostream& s, const Vec3<t>& v) {
        s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return s;
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <>
template <>
inline Vec3<int>::Vec3(const Vec3<float>& v)
    : x(int(v.x + 0.5f)), y(int(v.y + 0.5f)), z(int(v.z + 0.5f)) {
}

template <>
template <>
inline Vec3<float>::Vec3(const Vec3<int>& v)
    : x(float(v.x)), y(float(v.y)), z(float(v.z)) {
}

//////////////////////////////////////////////////////////////////////////////////////////////

const int DEFAULT_ALLOC = 4;

class Matrix {
    std::vector<std::vector<float>> m;
    int rows, cols;

public:
    Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
    int nrows();
    int ncols();

    static Matrix identity(int dimensions);
    std::vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();

    friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

#endif //__GEOMETRY_H__
