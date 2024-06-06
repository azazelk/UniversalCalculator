#include "calculations.h"
#include "matrix.h"

Calculations::Calculations(){
    combo_index = new int{0};
    det = new qreal{0};
}

Calculations::~Calculations(){
    delete combo_index;
    delete matrix;
}

void Calculations::getComboIndex(int i){
    *combo_index = i;
}

//функция парсинга если пользователь совершает действие с матрицей
void Calculations::getMatrixData(QTextDocument* doc){
    txt = new QString(doc->toRawText());
    qDebug() << *txt;
    //Парсим строку с матрицей в массив
    QString buffer;
    //Узнаём колличество строк
    int rows = 0;
    for (auto j = txt->begin(); j != txt->end(); j++){
        if (*j == QChar(8233)){
            rows += 1;
        }
    }
    rows++;
    //узнаём колличество элементов
    int elements = 0;
    for (auto j = txt->begin(); j != txt->end(); j++){
        if (j->isDigit()){
            elements++;
        }
    }
    int cols = elements / rows; //узнаём колличество столбцов
    matrix_data_buffer = new qreal[rows*cols]; //создаём массив подходящего размера
    //парсим строку в массив
    int i = 0;
    for (auto j = txt->begin(); j != txt->end(); j++){
        if (j->isDigit()){
            buffer += *j;
        } else if (*j == QChar(8233) || *j == ' '){
            if (!buffer.isEmpty()){matrix_data_buffer[i] = buffer.toDouble(); i++;}
            buffer.clear();
        }
    }
    matrix_data_buffer[i] = buffer.toDouble();
    buffer.clear();
    //передаём массив объекту класса matrix
    matrix = new Matrix(matrix_data_buffer, rows, cols);
}
