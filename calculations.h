#pragma once
#include <window.h>

class Calculations{
public:
    Calculations();
    ~Calculations();
    void getComboIndex(int);
    void getData(QTextDocument*);
private:
    qreal* matrix_data_buffer;
    qreal** matrix_data_1;
    qreal* det;
    QString* txt;
    void calculationSwitch();
    int* combo_index;
    int* tab_index;
    int* crows;
    int* ccols;
};

void matrixDebug(qreal*, int&, int&);
void matrixDebug(qreal**, int&, int&);
