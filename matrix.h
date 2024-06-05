#pragma once

class Matrix{
public:
    Matrix(double**, int, int);
    ~Matrix();
    int get_det();
private:
    double** data;
    int rows;
    int cols;
};
