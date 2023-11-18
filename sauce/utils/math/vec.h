#ifndef VEC_H_
#define VEC_H_

#include "core.h"

#include <stdexcept>

template <typename T>
class Vec3 {
public:
    // Constructors
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    // Getter methods
    T getX() const { return x; }
    T getY() const { return y; }
    T getZ() const { return z; }

    // Setter methods
    void setX(T _x) { x = _x; }
    void setY(T _y) { y = _y; }
    void setZ(T _z) { z = _z; }

    // Vector operations
    Vec3<T> operator+(const Vec3<T>& other) const {
        return Vec3<T>(x + other.x, y + other.y, z + other.z);
    }

    Vec3<T> operator-(const Vec3<T>& other) const {
        return Vec3<T>(x - other.x, y - other.y, z - other.z);
    }

    Vec3<T> operator*(T scalar) const {
        return Vec3<T>(x * scalar, y * scalar, z * scalar);
    }

    Vec3<T> operator/(T scalar) const {
        if (scalar != 0) {
            return Vec3<T>(x / scalar, y / scalar, z / scalar);
        } else {
            // You might want to handle division by zero differently
            throw std::runtime_error("Division by zero");
        }
    }

    // Dot product
    T dot(const Vec3<T>& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vec3<T> cross(const Vec3<T>& other) const {
        return Vec3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    // Magnitude (length) of the vector
    T magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    // Normalize the vector
    Vec3<T> normalize() const {
        T mag = magnitude();
        if (mag != 0) {
            return Vec3<T>(x / mag, y / mag, z / mag);
        } else {
            // You might want to handle the zero vector differently
            throw std::runtime_error("Normalization of zero vector");
        }
    }

private:
    T x, y, z;
};

#endif // VEC_H_
