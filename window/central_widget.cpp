#include "window.h"

//Конструктор центрального виджета
CentralWidget::CentralWidget(QWidget* parrent): QWidget(parrent){
    tab_counter = 0;
    tab = new QTabWidget(this);
    tab->setTabsClosable(true);
    vlayout = new QVBoxLayout(this);
    vlayout->addWidget(tab);
    tab_list = new QList<MatrixEditor*>;
    opened_files_paths = new QList<QString>;
    this->setLayout(vlayout);
    calc = new Calculations;
    answer_doc = new QDockWidget;
    answer = new QLabel;
}

//Деструктор центрального виджета
CentralWidget::~CentralWidget(){
    delete[] opened_files_paths;
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
        }
    }
    else{
        qDebug() << "file is opened";
        tab_list->removeLast();
    }
    /*qDebug() << "tab_list" << tab_list->size();
    qDebug() << "opened_files_paths" << opened_files_paths->size();*/
}

//Слот для создания нового файла (создаёт экземпляр виджета для редактирования матрицы и добавляет в список)
void CentralWidget::new_matrix_file_edit(){
    tab_list->append(new MatrixEditor(this));
    *tab_list->last()->file_name = "matrix_"+QString::number(QRandomGenerator::global()->generate());
    opened_files_paths->append("*");
    tab->addTab(tab_list->last(), *(tab_list->last()->file_name));
    /*qDebug() << "tab_list" << tab_list->size();
    qDebug() << "opened_files_paths" << opened_files_paths->size();*/
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
    /*qDebug() << "tab_counter" << tab_counter;
    qDebug() << "tab_list" << tab_list->size();
    qDebug() << "opened_files_paths" << opened_files_paths->size();*/
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
    /*qDebug() << "tab_counter" << tab_counter;
    qDebug() << "tab_list" << tab_list->size();
    qDebug() << "opened_files_paths" << opened_files_paths->size();*/
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
    /*qDebug() << num;
    qDebug() << "delete 1 object from tab_list" << tab_list->size();
    qDebug() << "delete 1 object from opened_files_paths" << opened_files_paths->size();*/
}

void CentralWidget::calculation_resiver(int i){
    calc->getComboIndex(i);
}

void CentralWidget::start(){
    if (opnum2 == 0 && !this->tab_list->empty()){
        if (opnum1 == 0){
            calc->matrix_pars(this->tab_list->at(tab_counter)->document());
            qDebug() << "Matrix data get succes";
            QString det("Determinant: ");
            if (std::pair<int, double> i = calc->GetDet(this->calc->matrix); i.first == 1){
                det += std::to_string(i.second);
                answer->setText(det);
                answer_doc->setWidget(answer);
            } else {
                det = "Error, please set matrix, where rows = cols";
                answer->setText(det);
                answer_doc->setWidget(answer);
            }
        }
    }
    else if (opnum2 == 1 && !this->tab_list->empty()){
        if (opnum1 == 0){
            qDebug() << "this op empty";
        }
    }
}

void CentralWidget::operation_num_get(int i){
    opnum1 = i;
    qDebug() << "operation: " << opnum1;
}

void CentralWidget::get_op_num(int i){
    opnum2 = i;
}
