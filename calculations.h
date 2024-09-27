#pragma once
#include <window.h>
#include "matrix.h"
#include <Eigen/Dense>

class Calculations{
    friend class CentralWidget;
public:
    Calculations();
    ~Calculations();
    void getComboIndex(int);
    void getMatrixData(QTextDocument*);
    void del_spaces(QString::iterator first, QString::iterator last, QString* txt);
    Eigen::MatrixXd matrix_pars(QTextDocument*);
    Eigen::MatrixXd* matrix;
    std::pair<int, double> GetDet(Eigen::MatrixXd*);
private:
    int* combo_index;
    int* tab_index;
    int* rows;
    int* cols;
};

void matrixDebug(qreal*, int&, int&);
void matrixDebug(Eigen::MatrixXd*, int&, int&);
