#include "calculations.h"

Calculations::Calculations(){
    combo_index = new int{0};
}

Calculations::~Calculations(){
    delete combo_index;
}

void Calculations::getComboIndex(int i){
    *combo_index = i;
}

void Calculations::del_spaces(QString::iterator begin, QString::iterator end, QString* txt){
    while (begin != end){
        if (*txt->begin() == ' '){
            txt->erase(txt->begin());
        } else if (*begin == ' ' && *(begin + 1) == ' '){
            txt->erase(begin);
            begin--;
        } else if (*begin == ' ' && *(begin + 1) == '\n'){
            txt->erase(begin);
            begin--;
        }
        if (*(end-1) == ' ' || *(end-1) == '\n'){
            txt->erase(end-1);
            end--;
        }
        begin++;
    }
}

//функция для парсинга если пользователь совершает действие с матрицей
Eigen::MatrixXd* Calculations::matrix_pars(QTextDocument* doc){
    QString* buffer = new QString(doc->toPlainText());
    //Узнаём колличество строк
    del_spaces(buffer->begin(), buffer->end(), buffer);
    auto ti = buffer->begin();
    int* rows = new int(0);
    int* elements = new int(0);
    while(ti != buffer->end()){
        if (*ti == '\n'){
            (*rows)++;
            (*elements)++;
        } if (*ti == ' '){
            (*elements)++;
        }
        ti++;
    }
    (*rows)++;
    (*elements)++;
    //Собираем элементы в матрицу
    int* cols = new int(*elements/(*rows));
    Eigen::MatrixXd* matrix = new Eigen::MatrixXd(*rows, *cols);
    QString* bufstr = new QString("");
    int* col_counter = new int(0);
    int* row_counter = new int(0);
    for (auto it = buffer->begin(); it != buffer->end(); it++){
        if (it->isDigit() || *it == ',' || *it == '.'){
            *bufstr += *it;
        } else if (*it == ' '){
            (*matrix)(*row_counter,*col_counter) = bufstr->toDouble();
            *bufstr = "";
            (*col_counter)++;
        } else if (*it == '\n'){
            (*matrix)(*row_counter,*col_counter) = bufstr->toDouble();
            *bufstr = "";
            *col_counter = 0;
            (*row_counter)++;
        }
    }
    (*matrix)(*row_counter,*col_counter) = bufstr->toDouble();
    delete bufstr;
    delete col_counter;
    delete elements;
    return matrix;
}

std::pair<bool, double> Calculations::GetDet(Eigen::MatrixXd* matrix){
    if (matrix->rows() == matrix->cols()){
        return {1, matrix->determinant()};
    } else {
        return {0, 0};
    }
}

std::pair<bool, Eigen::MatrixXd*> Calculations::fold(Eigen::MatrixXd* m1, Eigen::MatrixXd* m2){
    Eigen::MatrixXd* result;
    if ((m1->rows() != 0 && m2->rows() != 0) && (m1->rows() == m2->rows() && m1->cols() == m2->cols())){
        result = new Eigen::MatrixXd(*m1 + *m2);
        return {1, result};
    } else {
        return {0, result};
    }
}

void Calculations::show(QString* s, Eigen::MatrixXd* m){
    for (int i = 0; i < m->rows(); i++){
        for (int j = 0; j < m->cols(); j++){
            QString* edited = show_edit(new double((*m)(i,j)));
            *s += *edited;
            delete edited;
            *s += " ";
        }
        *s += '\n';
    }
}

std::pair<bool, Eigen::MatrixXd*> Calculations::subtraction(Eigen::MatrixXd* m1, Eigen::MatrixXd* m2){
    Eigen::MatrixXd* result;
    if ((m1->rows() != 0 && m2->rows() != 0) && (m1->rows() == m2->rows() && m1->cols() == m2->cols())){
        result = new Eigen::MatrixXd(*m1 - *m2);
        return {1, result};
    } else {
        return {0, result};
    }
}
