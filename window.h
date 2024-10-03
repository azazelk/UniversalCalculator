#pragma once
#include <QMainWindow>
#include <QApplication>
#include "QtWidgets"
#include "calculations.h"

class MainWindow; //Класс главного окна
class CentralWidget; //Класс центрального окна
class MatrixEditor; //Класс редактора матриц
class Calculations;
class FlexibleBox;
QString* matrix_name_getter(QString&); //Функция принимает путь к файлу и обрезает дирректории так, что остаётстя только имя файла без расширения

class MainWindow: public QMainWindow{
    friend class CentralWidget;
public:
    MainWindow(QWidget* parrent = 0);
private:
    FlexibleBox* calculate_select;
    QMenuBar* menubar;
    QMenu* file_menu;
    QAction* file_open_action; //действие открытия файла
    QAction* save; //действие для сохранения файла
    QAction* save_as; //действие для сохраниния файла с пользовательским путём
    QAction* file_new; //действие для создания нового файла
    CentralWidget* mainCentralWgt; //объект класса центрального виджета
    QStatusBar* status_bar;
    QPushButton* start_button;
    QComboBox* input_select;
};

class MatrixEditor: public QPlainTextEdit{
    Q_OBJECT
    friend class CentralWidget;
public:
    MatrixEditor(QWidget *parrent = 0);
    ~MatrixEditor();
    QString matrix_save(); //Функция для сохранения файла (возвращает путь к файлу)
    QString matrix_save_as(); //Функция для сохранения файла с пользовательским путём (возвращает путь к файлу)
private:
    QString* file_name; //Хранит имя файла
    QString* path; //Хранит полный путь к файлу
    QFile* matrix_file; //Хранит файл
};

class CentralWidget: public QWidget{
    friend class MainWindow;
    Q_OBJECT
public:
    CentralWidget(QWidget* parrent = 0);
    ~CentralWidget();
private:
    int tab_counter; //хранит число вкладок
    QList<MatrixEditor*>* tab_list; //хранит в себе объекты класса MatrixEditor
    QList<MatrixEditor*>* second_editor;
    QTabWidget* tab;
    QVBoxLayout* vlayout;
    QList<QString>* opened_files_paths; //хранит в себе все пути открытых файлов
    Calculations* calc;
    QDockWidget* answer_doc;
    QLabel* answer;
    int opnum1 = 0; //хранит в себе первый номер операции
    int opnum2 = 0; //хранит в себе второй номер операции
public slots:
    void matrix_from_file_to_dlg(); //слот для открытия файла
    void matrix_save_reciver(); //слот вызывает функцию matrix_save() у конкретного объекта класса MatrixEditor
    void matrix_save_as_reciver(); //слот вызвает функцию matrix_save_as() у конкретного объекта класса MatrixEditor
    void new_matrix_file_edit(); //слот создаёт новый объект класса MatrixEditor
    void tab_num_get(int); //слот получает номер открытой в данный момент вкладки
    void tab_close(int); //слот получает сигнал о закрытии вкладки с файлом
    void calculation_resiver(int); //передаёт индекс математической операции объекту calc из calculate_select
    void start(); //запускает процесс парсинга и вычислений
    void input_num_get(int); //слот для получения номера вводимых данных
    void get_op_num(int); //слот для получения номера операции
};

class FlexibleBox: public QComboBox{
    friend class CentralWidget;
    Q_OBJECT;
public:
    FlexibleBox(QWidget* parrent = 0);
public slots:
    void create_action(int);
signals:
    void drop_input(int);
};
