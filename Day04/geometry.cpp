#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "geometry.h"




// =============================
// Matrix 类实现部分
// =============================

Matrix::Matrix(int r, int c)
    : m(std::vector<std::vector<float>>(r, std::vector<float>(c, 0.f))), rows(r), cols(c) {
}

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i = 0; i < dimensions; ++i)
        E[i][i] = 1.f;
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert(i >= 0 && i < rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < a.cols; ++j)
            for (int k = 0; k < cols; ++k)
                result[i][j] += m[i][k] * a.m[k][j];
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows == cols);
    Matrix result(rows, cols * 2);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            result[i][j] = m[i][j];
        result[i][i + cols] = 1.f;
    }

    // Forward elimination
    for (int i = 0; i < rows; ++i) {
        float diag = result[i][i];
        for (int j = 0; j < result.cols; ++j)
            result[i][j] /= diag;

        for (int k = i + 1; k < rows; ++k) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; ++j)
                result[k][j] -= result[i][j] * coeff;
        }
    }

    // Backward substitution
    for (int i = rows - 1; i >= 0; --i) {
        for (int k = i - 1; k >= 0; --k) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; ++j)
                result[k][j] -= result[i][j] * coeff;
        }
    }

    // Extract inverse from augmented matrix
    Matrix truncate(rows, cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            truncate[i][j] = result[i][j + cols];
    return truncate;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i = 0; i < m.nrows(); ++i) {
        for (int j = 0; j < m.ncols(); ++j) {
            s << m[i][j];
            if (j < m.ncols() - 1) s << '\t';
        }
        s << '\n';
    }
    return s;
}
