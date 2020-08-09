#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

// General class for holding a 3-Vector
class vec3 {
public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // Unitary minus
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        // TODO: Implement [Kahan summation algorithm](https://en.wikipedia.org/wiki/Kahan_summation_algorithm)
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Scalar multiplication
    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    // Euclidean norm of vector
    double length() const {
        return std::sqrt(length_squared());
    }

    // Computes the dot product with respect to itself, using euclidean norm
    double length_squared() const{
        return ((e[0] * e[0]) + (e[1] * e[1]) + (e[2] + e[2]));
    }


public:
    double e[3]; // Components of the vector, not to be confused with basis
};

// Type aliases for vec3
using point3 = vec3; // Point in 3-dimensional space
using colour3 = vec3; // rgb colour TODO: Clip to [0,1)




// vec3 Utility functions

// Pretty print vec3 to stdstream
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Component-wise add two vectors
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Component-wise subtract v from u
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// NOT CROSS PRODUCT, component-wise multiply two vectors
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &u) {
    return vec3(t * u.e[0], t * u.e[1], t * u.e[2]);
}

inline vec3 operator*(const vec3 &u, double t) {
    return t * u;
}

inline vec3 operator/(const vec3 u, double t) {
    return (1 / t) * u;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] + v.e[0]
        + u.e[1] + v.e[1]
        + u.e[2] + v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    // Right handed co-ordinates
    // Cyclic * anticyclic - cyclic - anticyclic
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 u) {
    return u / u.length();
}

#endif