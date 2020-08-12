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

    vec3 operator-() const {
        return vec3(-e[0], -e[1], -e[2]);
    } // Unitary minus
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        // TODO: Implement [Kahan summation
        // algorithm](https://en.wikipedia.org/wiki/Kahan_summation_algorithm)
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

    vec3& operator/=(const double t) { return *this *= 1 / t; }

    // Euclidean norm of vector
    double length() const { return sqrt(length_squared()); }

    // Computes the dot product with respect to itself, using euclidean norm
    double length_squared() const {
        return ((e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]));
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max),
                    random_double(min, max));
    }

public:
    double e[3]; // Components of the vector, not to be confused with basis
};

// Type aliases for vec3
using point3 = vec3;  // Point in 3-dimensional space
using colour3 = vec3; // rgb colour

// vec3 Utility functions

// Pretty print vec3 to stdstream
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline bool operator==(const vec3& u, const vec3& v) {
    return ((u.e[0] == v.e[0]) && (u.e[1] == v.e[1]) && (u.e[2] == v.e[2]));
}

// Component-wise add two vectors
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Component-wise subtract v from u
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// NOT CROSS PRODUCT, component-wise multiply two vectors
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& u) {
    return vec3(t * u.e[0], t * u.e[1], t * u.e[2]);
}

inline vec3 operator*(const vec3& u, double t) { return t * u; }

inline vec3 operator/(vec3 u, double t) { return (1 / t) * u; }

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    // Right handed co-ordinates
    // Cyclic * anticyclic - cyclic - anticyclic
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 u) { return u / u.length(); }

vec3 random_in_unit_disk() {
    // TODO: Implement a better algorithm
    while (true) {
        auto u = vec3::random(-1, 1);
        u.e[2] = 0;
        if (u.length_squared() > 1) {
            continue;
        }
        return u;
    }
}

vec3 random_in_unit_sphere() {
    // TODO: Implement a better algorithm
    while (true) {
        auto u = vec3::random(-1, 1);
        if (u.length_squared() > 1) {
            continue;
        }
        return u;
    }
}

// Returns a random unit vector with a lambertian distribution
vec3 lambertian_unit_vector() {
    auto a = random_double(0, 2 * M_PI);
    auto z = random_double(-1, 1);
    auto r = std::sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

// Reflects u about unit normal n
vec3 reflect(const vec3& u, const vec3& n) { return u - (2 * dot(u, n) * n); }

// Refracts about a normal
vec3 refract(const vec3& incident, const vec3& normal,
             double eta_i_over_eta_r) {
    auto cos_theta = fmin(dot(-incident, normal), 1.0);
    vec3 r_out_perp = eta_i_over_eta_r * (incident + cos_theta * normal);
    vec3 r_out_parallel =
        -sqrt(fabs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_perp + r_out_parallel;
}

#endif