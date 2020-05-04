#pragma once
#include <memory>

template <typename T> class Matrix;

template<typename T> struct Vec3{
    T x, y, z;
    inline Matrix<T> Matrix<T>() {
        Matrix<T> r(1, 3);
        r(0, 0) = x;
        r(0, 1) = y;
        r(0, 2) = z;
        return r;
    }

    inline Vec2<T> Vec2<T>(){
        return {x, y};
    }

    Vec3(const Matrix<T>& m);
};

template<typename T> struct Vec2{
    T x, y;
    inline Matrix<T> Matrix<T>() {
        Matrix<T> r(1, 2);
        r(0, 0) = x;
        r(0, 1) = y;
        return r;
    }

    inline Vec3<T> Vec3<T>() {
        return {x, y, 0};
    }

    Vec2(const Matrix<T>& m);
};

template<typename T> class Matrix {
protected:
    const T* m_data;
    const size_t m_colums, m_rows;

private:
    

public:
    Matrix() = delete;
    Matrix(const size_t rows, const size_t columns);
    Matrix(const Matrix& cpy);
    ~Matrix();

    inline size_t   columns()   noexcept { return m_weight; };
    inline size_t   rows()      noexcept { return m_rows; };
    inline const T* data()      noexcept { return m_data; } 

    inline T& operator()(const size_t row, const size_t column) {
        return m_data[x + y * m_rows]
    }

    inline Matrix<T> multiply(const Matrix<T>& m){
        if(m.m_rows == m_colums)
        Matrix<T> res(m.m_rows, m_colums);
    }

    inline Matrix<T> operator*(const Matrix<T>& m){
        return multiply(m);
    }
};

template <typename T> inline Vec3<T> rotateX(Vec3<T> vec, float degree){

}

template <typename T> inline Vec3<T> rotateY(Vec3<T> vec, float degree){

}

template <typename T> inline Vec3<T> rotateZ(Vec3<T> vec, float degree){

}
