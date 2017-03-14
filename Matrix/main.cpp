#include <iostream>
#include <sstream>
#include <stdexcept>


struct Matrix {
    double *data;
    int rows, cols;

    Matrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        data = new double[rows*cols];
    }

    double get(int row, int col) {
        return data[row * cols + col];
    }

    double set(int row, int col, double val) {
        data[row * cols + col] = val;
    }

    Matrix& operator +(const Matrix& m2)
    {
        return sum(m2);
    }

    Matrix& sum(Matrix matrix) {
        Matrix *s = new Matrix(rows, cols);
        for (int i = 0; i < rows * cols; ++i) {
            s->data[i] = data[i] + matrix.data[i];
        }
        return *s;
    }

    Matrix& substract(Matrix matrix) {
        Matrix *s = new Matrix(rows, cols);
        for (int i = 0; i < rows * cols; ++i) {
            s->data[i] = data[i] - matrix.data[i];
        }
        return *s;
    }

    Matrix& multiply(Matrix matrix) {
        if (!(shape()[0] == matrix.shape()[1] && shape()[1] == matrix.shape()[0]))
            throw std::invalid_argument("shape does not fit each other");

        Matrix *s = new Matrix(rows, cols);
        for (int i = 0; i < rows * cols; ++i) {
            s->data[i] = data[i] - matrix.data[i];
        }
        return *s;
    }

    Matrix& T() {
        Matrix *s = new Matrix(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                s->set(j, i, get(i, j));
            }
        }
        return * s;
    }

    int* shape() {
        int * shape = new int[2];
        shape[0] = rows;
        shape[1] = cols;
        return shape;
    }

    std::string toString() {
        std::stringstream ss;
        ss << "[" << std::endl << " [";
        for (int i = 0; i < rows * cols; ++i) {
            if (i != 0 && i % cols == 0)
                ss << "]" << std::endl << " [";
            ss << " " << data[i] << " ";
        }
        ss << "]" << std::endl << "]";
        return ss.str();
    }
};


int main() {
    Matrix m1  = Matrix(2, 3);
    Matrix m2  = Matrix(2, 3);
    for (int i = 0; i < 6; ++i) {
        m1.set((int)i / 3, i % 3, i);
        m2.set((int)i / 3, i % 3, i);
    }

    m1.set(0, 1, 6);
    std::cout << m1.toString();
    std::cout << m2.toString();
    std::cout << (m1 + m2).toString();
    std::cout << m2.T().T().T().toString();
    return 0;
}