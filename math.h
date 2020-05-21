//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#pragma once
#include <memory>
#include <cmath>

template <typename T> class Matrix;
template <typename T> struct Vec2;
template <typename T> struct Vec3;

#define PI 3.14159265359f
#define IS_UNSIGNED(x) (x >= 0)

template<typename T> inline T degree(T t){ return t * 180/PI; }
template<typename T> inline T radians(T t){ return t * PI/180; }

template<typename T> struct Vec3{
    T x = NULL, y = NULL, z = NULL;

    inline operator Matrix<T>() {
        Matrix<T> r(1, 3);
        r(0, 0) = x;
        r(0, 1) = y;
        r(0, 2) = z;
        return r;
    }

    inline float length() const noexcept {
        return std::sqrt(x*x + y*y + z*z);
    }

    inline Vec3<T> norm() const {
        auto len = length();
        if(len == 0)    throw "Cannot normalize Vec3 of length 0.";
        else/******/    return {x/len, y/len, z/len};
    }

    inline Vec3<T> operator-(const Vec3<T>& vec) const {
        return {x - vec.x, y - vec.y, z - vec.z};
    }

    inline Vec3<T> operator+(const Vec3<T>& vec) const {
        return {x + vec.x, y + vec.y, z + vec.z};
    }

    inline Vec3<T> operator* (const T t) const{
        return {x * t, y * t, z * t};
    }

    inline T dot(const Vec3<T>& vec) const {
        return vec.x * x + vec.y * y + vec.z * z;
    }

    inline Vec3<T> cross(const Vec3<T>& vec) const {
        return {
            y * vec.z - z * vec.y,
            z * vec.x - x * vec.z,
            x * vec.y - y * vec.x
        };
    }

    inline operator Vec2<T>(){
        return {x, y};
    }

    inline bool has_same_direction(const Vec3<T>& vec){
        return  IS_UNSIGNED(x) == IS_UNSIGNED(vec.x)
            &&  IS_UNSIGNED(y) == IS_UNSIGNED(vec.y)
            &&  IS_UNSIGNED(z) == IS_UNSIGNED(vec.z);
    }

};


template<typename T> struct Vec2{
    T x, y;
    inline operator Matrix<T>() {
        Matrix<T> r(1, 2);
        r(0, 0) = x;
        r(0, 1) = y;
        return r;
    }

    inline operator Vec3<T>() {
        return {x, y, 0};
    }
};


template<typename T> class Matrix {
protected:
    T* m_data;
    const size_t m_colums, m_rows;

public:
    Matrix() = delete;
    Matrix(const size_t rows, const size_t columns);
    Matrix(const Matrix& cpy);
    ~Matrix();

    inline size_t   columns()   noexcept { return m_colums; };
    inline size_t   rows()      noexcept { return m_rows; };
    inline const T* data()      noexcept { return m_data; } 

    inline T& operator()(const size_t row, const size_t column) const {
        return m_data[column + row * m_rows];
        //            ^column     ^row
    }

    inline Matrix<T> multiply(const Matrix<T>& m){
        if(m.m_rows == m_colums)
        Matrix<T> res(m.m_rows, m_colums);
    }

    inline Matrix<T> operator*(const Matrix<T>& m){
        return multiply(m);
    }
};

template <typename T> Matrix<T>::Matrix(const size_t rows, const size_t columns) 
: m_colums{columns}, m_rows{rows}
{
    auto size = columns * rows;
    if(size == 0) throw "Matrix can't be of size 0";
    m_data = new T[size];
}

template <typename T> Matrix<T>::~Matrix(){
    delete[] m_data;
}

template <typename T> Matrix<T>::Matrix(const Matrix& cpy)
: m_colums{cpy.m_colums}, m_rows{cpy.m_rows}, data{m_rows * m_colums}
{
    std::memcpy(m_data, cpy.data, m_colums * m_rows);
}

template <typename T> inline Vec3<T> rotateX(const Vec3<T>& vec, float degree){
    auto len = vec.length();
    auto rad = radians(degree);
    rad += std::atan(vec.z / vec.y);
    return {vec.x, cos(rad) * len, std::sin(rad) * len};
}

template <typename T> inline Vec3<T> rotateY(const Vec3<T>& vec, float degree){
    auto len = vec.length();
    auto rad = radians(degree);
    rad += std::atan(vec.z / vec.x);
    return {std::cos(rad) * len, vec.y, std::sin(rad) * len};
}

template <typename T> inline Vec3<T> rotateZ(const Vec3<T>& vec, float degree){
    auto len = vec.length();
    auto rad = radians(degree);
    rad += std::atan(vec.y / vec.x);
    return {std::cos(rad) * len, std::sin(rad) * len, vec.z};
}

template <typename T> inline Vec3<T> rotate(const Vec3<T>& vec, float x, float y, float z){
    return rotateX(rotateY(rotateZ(vec, z), y), x);
}

inline float angle(const Vec3<float>& a, const Vec3<float>& b){
    return std::acos(a.dot(b) / (a.length() * b.length()));
}

template <typename T> T clamp(T t, T _min, T _max){
    if(t > _max) t = _max;
    else if (t < _min) t = _min;
    return t;
}
