#pragma once

class Matrix{
    friend class Calculations;
public:
    Matrix(double**, int, int);
    Matrix(double*, int, int);
    Matrix();
    ~Matrix();
    int get_det();
private:
    double** data;
    int rows;
    int cols;
};
