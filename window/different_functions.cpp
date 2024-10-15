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

//функция для редактирования вывода
QString* show_edit(double* ed){
    QString* a = new QString;
    *a += std::to_string(*ed);
    auto i = a->end()-1;
    while (*i == '0' && (*i)!= '.'){
        a->erase(i);
        i--;
    }
    if (*i == '.'){
        a->erase(i);
    }
    return a;
}
