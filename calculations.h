#pragma once
#include <window.h>

class Calculations{
    friend class CentralWidget;
public:
    Calculations();
    ~Calculations();
    void getComboIndex(int);
    void getMatrixData(QTextDocument*);
    void del_spaces(QString::iterator first, QString::iterator last, QString* txt);
    Eigen::MatrixXd* matrix_pars(QTextDocument*);
    std::pair<bool, double> GetDet(Eigen::MatrixXd*);
    std::pair<bool, Eigen::MatrixXd*> fold(Eigen::MatrixXd*, Eigen::MatrixXd*);
    void show(QString*, Eigen::MatrixXd*);
private:
    int* combo_index;
    int* tab_index;
};

void matrixDebug(qreal*, int&, int&);
void matrixDebug(Eigen::MatrixXd*, int&, int&);
