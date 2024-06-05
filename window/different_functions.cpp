#include "window.h"

//Функция для получения имени файла без пути (используется для именования вкладок)
QString* matrix_name_getter(QString& path){
    QString* name = new QString(path);
    while (name->contains("/")){
        name->removeFirst();
    }
    if (name->contains(".txt")){
        for (int i = 0; i < 4; i++){
            name->removeLast();
        }
    }
    return name;
}

void matrixDebug(qreal* matrix, int& rows, int& cols){
    int k = 0;
    for (int i = 0; i < rows*cols; i++){
        qDebug() << matrix[i];
        k++;
        if (k == cols){
            qDebug() << "\n";
            k = 0;
        }
    }
    qDebug() << '\n';
}

void matrixDebug(qreal** matrix, int& rows, int& cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            qDebug() << matrix[i][j];
        }
        qDebug() << '\n';
    }
    qDebug() << '\n';
}
