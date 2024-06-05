#include "calculations.h"
#include "matrix.h"

Calculations::Calculations(){
    combo_index = new int{0};
    crows = new int{0};
    ccols = new int{0};
    det = new qreal{0};
}

Calculations::~Calculations(){
    delete combo_index;
    delete crows;
    delete ccols;
}

void Calculations::getComboIndex(int i){
    *combo_index = i;
}

void Calculations::getData(QTextDocument* doc){
    ///////////////////////////////////////////Парсинг если пользователь совершает действие с матрицей//////////////////////////////////////////////////
    txt = new QString(doc->toRawText());
    qDebug() << *txt;
    //Парсим строку с матрицей в массив
    QString buffer;
    *ccols = 0;
    *crows = 0;
    //Узнаём колличество столбцов
    for (auto j = txt->begin(); *j != QChar(8233); j++){
        if (*j == ' '){
            *ccols += 1;
        }
    }
    *ccols += 1;
    //Узнаём колличество строк
    for (auto j = txt->begin(); j != txt->end(); j++){
        if (*j == QChar(8233)){
            *crows += 1;
        }
    }
    *crows += 1;
    matrix_data_buffer = new qreal[*ccols*(*crows)]; //создаём массив подходящего размера
    //парсим строку в массив
    int i = 0;
    for (auto j = txt->begin(); j != txt->end(); j++){
        if (j->isDigit()){
            buffer += *j;
        } else if (*j == QChar(8233) || *j == ' '){
            matrix_data_buffer[i] = buffer.toDouble();
            buffer.clear();
            i++;
        }
    }
    matrix_data_buffer[i] = buffer.toDouble();
    buffer.clear();
    
    matrix_data_1 = new qreal*[*crows];
    for (int m = 0; m < *crows; m++){
        matrix_data_1[m] = new qreal[*ccols];
    }
    i = 0;
    for (int m = 0; m < *crows; m++){
        for (int j = 0; j < *ccols; j++){
            matrix_data_1[m][j] = matrix_data_buffer[i];
            i++;
        }
    }
    delete[] matrix_data_buffer;
    matrixDebug(matrix_data_1, *crows, *ccols);

    /*//выбираем действие
    switch (*combo_index) {
    case 0:
        MatrixDet();
        break;
    default:
        break;
    }*/
}
