#include "window.h"

//Конструктор виджета для редоктирования матрицы
MatrixEditor::MatrixEditor(QWidget* parrent): QPlainTextEdit(parrent){
    file_name = new QString;
    path = new QString;
}

//Деструктор виджета для редоктирования матрицы
MatrixEditor::~MatrixEditor(){
    delete file_name;
    delete path;
    delete matrix_file;
}

//Слот для сохранения файла
QString MatrixEditor::matrix_save(){
    if (path->isNull()){
        QList<QString>* spaths = new QList<QString>(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation));
        *path = spaths->first() + ("/"+(*file_name)+".txt");
        spaths->clear();
        delete spaths;
    }
    matrix_file = new QFile(*path, this);
    if (!matrix_file->open(QIODevice::WriteOnly | QIODevice::Text)){

    } else {
        QTextStream data(matrix_file);
        data << this->toPlainText();
        matrix_file->close();
        return *path;
    }
    matrix_file->close();
    return "error_file";
}

//Слот для сохранения файла с пользовательсим путём
QString MatrixEditor::matrix_save_as(){
    *path = QFileDialog::getSaveFileName(this, tr("Выберите путь для сохранения"), *file_name, "*.txt");
    matrix_file = new QFile(*path, this);
    if (!matrix_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "file open Error";
    } else {
        QTextStream data(matrix_file);
        data << this->toPlainText();
        matrix_file->close();
        return *path;
    }
    matrix_file->close();
    return "error_file";
}
