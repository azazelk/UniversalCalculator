#pragma once

class Matrix{
    friend class Calculations;
public:
    Matrix(double**, int, int);
    Matrix(double*, int, int);
    Matrix();
    ~Matrix();
private:
    double** data;
    int rows;
    int cols;
};
