#include "calculations.h"

Calculations::Calculations(){
    combo_index = new int{0};
}

Calculations::~Calculations(){
    delete combo_index;
    delete tab_index;
}

void Calculations::getComboIndex(int i){
    *combo_index = i;
}

void Calculations::del_spaces(QString::iterator begin, QString::iterator end, QString* txt){
    while (begin != end){
        if (*txt->begin() == ' '){
            txt->erase(txt->begin());
        } else if ((*begin == ' ' && *(begin + 1) == ' ') || ((*begin == ' ') && !(begin + 1)->isDigit())){
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
        if (it->isDigit()){
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

void Calculations::show(QString* s, std::pair<int, std::vector<std::vector<int>>>&& st){
    for (size_t i = 0; i < st.second.size(); i++){
        for (size_t j = 0; j < 2; j++){
            *s += std::to_string(st.second[i][j]) + " ";
        }
        *s += "\n";
    }
    *s += "sum: " + std::to_string(st.first);
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

std::pair<bool, Eigen::MatrixXd*> Calculations::multiplication(Eigen::MatrixXd* m1, Eigen::MatrixXd* m2){
    Eigen::MatrixXd* result;
    if (m1->cols() == m2->rows() || m1->rows() == m2->cols()){
        result = new Eigen::MatrixXd(*m1 * *m2);
        return {1, result};
    } else {
        return {0, result};
    }
}

std::vector<std::pair<int, std::vector<int>>> Calculations::get_struct(Eigen::MatrixXd* arr, size_t& size){
    std::vector<std::pair<int, std::vector<int>>> weigths(size + 50);
    int c = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if ((*arr)(i,j) != 0) {
                weigths[c].first = (*arr)(i,j);
                weigths[c].second.push_back(i);
                weigths[c].second.push_back(j);
                (*arr)(i,j) = 0;
                (*arr)(j,i) = 0;
                c++;
            }
        }
    }
    for (size_t i = 0; i < weigths.size(); i++) {
        if (weigths[i].first == 0) {
            weigths.erase(weigths.begin() + i);
            i -= 1;
        }
    }
    for (size_t i = 0; i < weigths.size(); i++) {
        for (size_t j = 0; j < weigths.size(); j++) {
            if (weigths[i].second.at(0) == weigths[j].second.at(1) &&
                weigths[i].second.at(1) == weigths[j].second.at(0)) {
                weigths.erase(weigths.begin() + j);
            }
        }
    }
    for (size_t i = 0; i < weigths.size(); i++) {
        for (size_t j = 0; j < weigths.size(); j++) {
            if (weigths[i].first < weigths[j].first) {
                weigths[i].swap(weigths[j]);
            }
        }
    }
    return weigths;
}

std::pair<int, std::vector<std::vector<int>>> Calculations::mst_kruskal(std::vector<std::pair<int, std::vector<int>>>&& weigths, size_t& size){
    std::set<int> s;
    std::pair<int, std::vector<std::vector<int>>> v_ans;
    v_ans.second.resize(size-1);
    s.emplace(weigths[0].second[0]); s.emplace(weigths[0].second[1]);
    bool visited[size];
    for(size_t c = 0; c < size; c++){
        visited[c] = 0;
    }
    visited[weigths[0].second[0]] = true; visited[weigths[0].second[1]] = true;
    v_ans.second[0].push_back(weigths[0].second[0]);
    v_ans.second[0].push_back(weigths[0].second[1]);
    v_ans.first = weigths[0].first;
    int i = 0;
    int counter = 1;
    while(s.size() < size){
        if ((s.find(weigths[i].second[0]) != s.end()) && !visited[weigths[i].second[1]]){
            s.emplace(weigths[i].second[1]);
            visited[weigths[i].second[1]] = true;
            v_ans.second[counter].push_back(weigths[i].second[0]);
            v_ans.second[counter].push_back(weigths[i].second[1]);
            v_ans.first += weigths[i].first;
            i = 0;
            counter++;
        } else if ((s.find(weigths[i].second[1]) != s.end()) && !visited[weigths[i].second[0]]){
            s.emplace(weigths[i].second[0]);
            visited[weigths[i].second[0]] = true;
            v_ans.second[counter].push_back(weigths[i].second[1]);
            v_ans.second[counter].push_back(weigths[i].second[0]);
            v_ans.first += weigths[i].first;
            i = 0;
            counter++;
        } else {
            i++;
        }
    }
    return v_ans;
}

#define INF 999999

std::pair<int, std::vector<std::vector<int>>> Calculations::mst_prim(Eigen::MatrixXd* graph, size_t& size){
    std::vector<int> visited;
    std::vector<int> unvisited;
    std::pair<int, std::vector<std::vector<int>>> v_ans;
    v_ans.second.resize(size-1);
    v_ans.first = 0;
    for (size_t i = 1; i < size; i++){
        unvisited.push_back(i);
    }
    visited.push_back(0);
    int minimum = INF;
    int a = 0;
    int counter = 0;
    v_ans.second[counter].push_back(a);
    for (size_t i = 0; i < size; i++){
        if ((*graph)(0,i) != 0 && (*graph)(0,i) < minimum){
            minimum = (*graph)(0,i);
            a = i;
        }
    }
    v_ans.first += minimum;
    minimum = INF;
    visited.push_back(a);
    unvisited.erase(find(unvisited.begin(), unvisited.end(), a));
    v_ans.second[counter].push_back(a);
    counter++;
    int b = 0;
    while (!unvisited.empty()){
        for (size_t i : visited){
            for (size_t j = 0; j < size; j++){
                if (((*graph)(i,j) != 0 && find(visited.begin(), visited.end(), j) == visited.end()) && (*graph)(i,j) < minimum){
                    minimum = (*graph)(i,j);
                    a = j;
                    b = i;
                }
            }
        }
        visited.push_back(a);
        unvisited.erase(find(unvisited.begin(), unvisited.end(), a));
        v_ans.first += minimum;
        minimum = INF;
        v_ans.second[counter].push_back(b);
        v_ans.second[counter].push_back(a);
        counter++;
    }
    return v_ans;
}
