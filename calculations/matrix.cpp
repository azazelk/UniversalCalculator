#include "matrix.h"

Matrix::Matrix(): rows(0), cols(0){};

Matrix::Matrix(double** tdata, int trows, int tcols): rows(trows), cols(tcols){
    data = new double*[rows];
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            data[i][j] = tdata[i][j];
        }
    }
}

Matrix::Matrix(double* tdata, int trows, int tcols): rows(trows), cols(tcols){
    data = new double*[rows];
    //выделяем память под вложенные массивы
    for (int i = 0; i < rows; i++){
        data[i] = new double[cols];
    }
    //копируем одномерный массив
    int k = 0;
    int r = 0;
    for (int i = 0; i < rows*cols; i++){
        data[r][k] = tdata[i];
        k++;
        if (k == cols){
            k = 0;
            r += 1;
        }
    }
}

Matrix::~Matrix(){}

