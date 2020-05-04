#include "math.h"


template <typename T> Matrix<T>::Matrix(const size_t rows, const size_t columns) 
: m_colums{columns}, m_rows{rows}
{
    auto size = columns * rows;
    if(size == 0) throw "Matrix can't be of size 0";
    data = new T[size];
}

template <typename T> Matrix<T>::~Matrix(){
    delete[] data;
}

template <typename T> Matrix<T>::Matrix(const Matrix& cpy)
: m_colums{cpy.m_colums}, m_rows{cpy.m_rows}, data{m_rows * m_colums}
{
    std::memcpy(m_data, cpy.data, m_colums * m_rows);
}

template<typename T> Vec3<T>::Vec3(const Matrix<T>& m){
    _TRY_BEGIN x = m(0,0); _CATCH_ALL _CATCH_END
    _TRY_BEGIN y = m(1,0); _CATCH_ALL _CATCH_END
    _TRY_BEGIN z = m(2,0); _CATCH_ALL _CATCH_END
}

template<typename T> Vec2<T>::Vec2(const Matrix<T>& m){
    _TRY_BEGIN x = m(0,0); _CATCH_ALL _CATCH_END
    _TRY_BEGIN y = m(1,0); _CATCH_ALL _CATCH_END
}
