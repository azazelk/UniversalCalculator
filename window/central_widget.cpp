#include "window.h"

//Конструктор центрального виджета
CentralWidget::CentralWidget(QWidget* parrent): QWidget(parrent){
    tab_counter = 0;
    tab = new QTabWidget(this);
    tab->setTabsClosable(true);
    hlayout = new QHBoxLayout(this);
    second_editor = new MatrixEditor;
    second_editor->setVisible(false);
    hlayout->addWidget(tab);
    hlayout->addWidget(second_editor);
    tab_list = new QList<MatrixEditor*>;
    opened_files_paths = new QList<QString>;
    this->setLayout(hlayout);
    calc = new Calculations;
    answer_doc = new QDockWidget;
    answer = new QLabel;
    matrix1 = new Eigen::MatrixXd;
    matrix2 = new Eigen::MatrixXd;
    result = new Eigen::MatrixXd;
}

//Деструктор центрального виджета
CentralWidget::~CentralWidget(){
    delete[] opened_files_paths;
}

//Функция для открытия второго редактора
inline void CentralWidget::open_second_editor(){
    if (!tab_list->isEmpty() && ((opnum1 == 0 && opnum2 == 1) || (opnum1 == 0 && opnum2 == 2)
        || (opnum1 == 0 && opnum2 == 3))){
        second_editor->setVisible(true);
    } else {
        second_editor->setVisible(false);
    }
}

//Слот для получения файла с пользовательским путём (создаёт экземпляр виджета для редактирования матрицы и добавляет в список)
void CentralWidget::matrix_from_file_to_dlg(){
    tab_counter = 0;
    tab_list->append(new MatrixEditor(this));
    *tab_list->last()->path = QFileDialog::getOpenFileName(this, "Выберите файл с матрицей", "", "*.txt");
    if (!tab_list->last()->path->isNull() && !opened_files_paths->contains(*tab_list->last()->path)){
        *tab_list->last()->file_name = *(matrix_name_getter(*tab_list->last()->path));
        tab_list->last()->matrix_file = new QFile(*tab_list->last()->path, this);
        if (!tab_list->last()->matrix_file->open(QIODevice::ReadOnly)){
            qDebug() << "file open Error";
            tab_list->removeLast();
        } else {
            opened_files_paths->append(*tab_list->last()->path);
            QTextStream* data = new QTextStream(tab_list->last()->matrix_file);
            tab_list->last()->setPlainText(QString(data->readAll()));
            tab->addTab(tab_list->last(), *(tab_list->last()->file_name));
            tab_list->last()->matrix_file->close();
            delete data;
            open_second_editor();
        }
    }
    else{
        qDebug() << "file is opened";
        tab_list->removeLast();
    }
}

//Слот для создания нового файла (создаёт экземпляр виджета для редактирования матрицы и добавляет в список)
void CentralWidget::new_matrix_file_edit(){
    tab_list->append(new MatrixEditor(this));
    *tab_list->last()->file_name = "matrix_"+QString::number(QRandomGenerator::global()->generate());
    opened_files_paths->append("*");
    tab->addTab(tab_list->last(), *(tab_list->last()->file_name));
    open_second_editor();
}

/*Слот для передачи сигнала "сохранить" открытому в данный момент файлу с матрицей (добавляет в список путей значение которое вернёт функция
matrix_save() объекта класса MatrixEditor)*/
void CentralWidget::matrix_save_reciver(){
    if (!tab_list->isEmpty()){
        if(!opened_files_paths->contains(*tab_list->at(tab_counter)->path) && opened_files_paths->at(tab_counter) != "*"){
            opened_files_paths->append(tab_list->at(tab_counter)->matrix_save());
        } else if(opened_files_paths->at(tab_counter) == "*"){
            opened_files_paths->replace(tab_counter, tab_list->at(tab_counter)->matrix_save());
        }
        else {
            tab_list->at(tab_counter)->matrix_save();
        }
    }
}

//Слот для передачи сигнала "сохранить как..." открытому в данный момент файлу с матрицей
void CentralWidget::matrix_save_as_reciver(){
    if (!tab_list->isEmpty()){
        if(!opened_files_paths->contains(*tab_list->at(tab_counter)->path) && opened_files_paths->at(tab_counter) != "*"){
            opened_files_paths->append(tab_list->at(tab_counter)->matrix_save_as());
        } else if(opened_files_paths->at(tab_counter) == "*"){
            opened_files_paths->replace(tab_counter, tab_list->at(tab_counter)->matrix_save_as());
        }
        else {
            tab_list->at(tab_counter)->matrix_save_as();
        }
        QString* names = new QString(*matrix_name_getter(*tab_list->at(tab_counter)->path));
        tab->setTabText(tab_counter, *names);
        *tab_list->at(tab_counter)->file_name = *names;
        opened_files_paths->replace(tab_counter, *tab_list->at(tab_counter)->path);
        delete names;
    }
}

//Слот для получения номера вкладки из объекта tab класса QTabWidget
void CentralWidget::tab_num_get(int num){
    tab_counter = num;
}

//Слот для получения номера вкладки из объекта tab класса QTabWidget и её закрытия
void CentralWidget::tab_close(int num){
    tab->removeTab(num);
    delete tab_list->at(num);
    tab_list->removeAt(num);
    opened_files_paths->removeAt(num);
    if (tab_list->isEmpty()){
        second_editor->hide();
    }
}

void CentralWidget::calculation_resiver(int i){
    calc->getComboIndex(i);
}

void CentralWidget::start(){
    QString* ans = new QString();
    if (opnum1 == 0 && !this->tab_list->empty()){
        matrix1 = (calc->matrix_pars(this->tab_list->at(tab_counter)->document()));
        if (opnum2 == 0){
            *ans += "Determinant: ";
            if (std::pair<bool, double> i = calc->GetDet(matrix1); i.first == true){
                if (i.second < 1*exp(-10)){
                    i.second = abs(i.second);
                }
                (*ans) += std::to_string(i.second);
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            } else {
                (*ans) = "Невозможно вычислить определитель";
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            }
        } else if (opnum2 == 1){
            matrix2 = (calc->matrix_pars(second_editor->document()));
            if (std::pair<bool, Eigen::MatrixXd*> i = calc->fold(matrix1, matrix2); i.first == true){
                result = (i.second);
                calc->show(ans, result);
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            } else {
                *ans = "Неверные данные: размеры матриц должны быть равны";
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            }
        } else if (opnum2 == 2){
            matrix2 = (calc->matrix_pars(second_editor->document()));
            if (std::pair<bool, Eigen::MatrixXd*> i = calc->subtraction(matrix1, matrix2); i.first == true){
                result = (i.second);
                calc->show(ans, result);
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            } else {
                *ans = "Неверные данные: размеры матриц должны быть равны";
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            }
        } else if (opnum2 == 3){
            matrix2 = (calc->matrix_pars(second_editor->document()));
            if (std::pair<bool, Eigen::MatrixXd*> i = calc->multiplication(matrix1, matrix2); i.first == true){
                result = (i.second);
                calc->show(ans, result);
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            } else {
                *ans = "Неверные данные: колличество столбцов первого сомножителя долны быть равны колличеству строк второго сомножителя";
                answer->setText(*ans);
                answer_doc->setWidget(answer);
            }
        }
    }
    delete ans;
}

void CentralWidget::input_num_get(int i){
    opnum1 = i;
    qDebug() << "operation1: " << opnum1;
}

void CentralWidget::get_op_num(int i){
    opnum2 = i;
    open_second_editor();
    qDebug() << "operation1: " << opnum2;
}
