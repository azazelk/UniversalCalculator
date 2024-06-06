#pragma once
#include <window.h>
#include "matrix.h"

class Calculations{
public:
    Calculations();
    ~Calculations();
    void getComboIndex(int);
    void getMatrixData(QTextDocument*);
private:
    Matrix* matrix = nullptr;
    qreal* matrix_data_buffer;
    qreal** matrix_data_1;
    qreal* det;
    QString* txt;
    void calculationSwitch();
    int* combo_index;
    int* tab_index;
};

void matrixDebug(qreal*, int&, int&);
void matrixDebug(qreal**, int&, int&);
