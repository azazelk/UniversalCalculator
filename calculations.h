#pragma once
#include "window.h"

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
    std::pair<bool, Eigen::MatrixXd*> subtraction(Eigen::MatrixXd*, Eigen::MatrixXd*);
    std::pair<bool, Eigen::MatrixXd*> multiplication(Eigen::MatrixXd*, Eigen::MatrixXd*);
    std::pair<int, std::vector<std::vector<int>>> mst_kruskal(std::vector<std::pair<int, std::vector<int>>>&&, size_t&);
    std::pair<int, std::vector<std::vector<int>>> mst_prim(Eigen::MatrixXd*, size_t&);
    std::vector<std::pair<int, std::vector<int>>> get_struct(Eigen::MatrixXd*, size_t&);
    void show(QString*, Eigen::MatrixXd*);
    void show(QString*, std::pair<int, std::vector<std::vector<int>>>&&);
private:
    int* combo_index;
    int* tab_index;
};

void matrixDebug(qreal*, int&, int&);
void matrixDebug(Eigen::MatrixXd*, int&, int&);
