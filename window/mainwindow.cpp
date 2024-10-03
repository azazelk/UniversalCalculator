#include "window.h"

//Конструктор главного окна
MainWindow::MainWindow(QWidget* parrent): QMainWindow(parrent){
    //####################################### Меню и центральный виджет #######################################
    menubar = new QMenuBar;
    this->setMenuWidget(menubar);
    file_menu = new QMenu("Файл", menubar);
    menubar->addMenu(file_menu);
    file_open_action = new QAction("Открыть файл", menubar);
    save = new QAction("Сохранить", menubar);
    save_as = new QAction("Сохранить как...", menubar);
    file_new = new QAction("Новый", menubar);
    file_menu->addAction(file_new);
    file_menu->addAction(file_open_action);
    file_menu->addAction(save);
    file_menu->addAction(save_as);
    mainCentralWgt = new CentralWidget;
    setCentralWidget(mainCentralWgt);

    //####################################### Статус бар #######################################
    status_bar = new QStatusBar;
    start_button = new QPushButton;
    start_button->setIcon(QPixmap(":/img/icons/play.png"));
    start_button->setMinimumSize(35, 32);
    start_button->setIconSize(QSize(25, 25));
    calculate_select = new FlexibleBox;
    calculate_select->insertItem(0, "Вычислить детерминант");
    calculate_select->insertItem(1, "Сложить");
    input_select = new QComboBox;
    input_select->insertItem(0, "Матрица");
    input_select->insertItem(1, "Уравнение");
    status_bar->insertWidget(0, input_select, 1);
    status_bar->insertWidget(1, calculate_select, 2);
    status_bar->insertWidget(2, start_button, 0);
    this->setStatusBar(status_bar);

    //####################################### Консоль с ответом ##################################
    mainCentralWgt->answer_doc = new QDockWidget("Answer console");
    mainCentralWgt->answer_doc->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->addDockWidget(Qt::BottomDockWidgetArea, mainCentralWgt->answer_doc);

    //####################################### Подключения #######################################
    //Подключение сигнала объекта tab QTabWidget к слоту mainCentralWgt для закрытия вкладки с файлом
    connect(mainCentralWgt->tab, SIGNAL(tabCloseRequested(int)), mainCentralWgt, SLOT(tab_close(int)));
    //Подключение сигнала объекта tab QTabWidget к слоту mainCentralWgt для полученя номера вкладки
    connect(mainCentralWgt->tab, SIGNAL(tabBarClicked(int)), mainCentralWgt, SLOT(tab_num_get(int)));
    //Подключение сигнала (об изменеиях во вкладке) объекта tab QTabWidget к слоту mainCentralWgt для полученя номера вкладки
    connect(mainCentralWgt->tab, SIGNAL(currentChanged(int)), mainCentralWgt, SLOT(tab_num_get(int)));
    //Подключение сигнала (открыть файл) к слоту mainCentralWgt для открытя файла
    connect(file_open_action, SIGNAL(triggered(bool)), mainCentralWgt, SLOT(matrix_from_file_to_dlg()));
    /*Подключение сигнала (сохранить) к слоту mainCentralWgt для передачи этого сигнала конкретному объекту класса
    MatrixEditor и последующего вызова функции для сохранения файла*/
    connect(save, SIGNAL(triggered(bool)), mainCentralWgt, SLOT(matrix_save_reciver()));
    /*Подключение сигнала (сохранить как...) к слоту mainCentralWgt для передачи этого сигнала конкретному объекту класса
    MatrixEditor и последующего вызова функции для сохранения файла с указанием пользовательского пути и имени*/
    connect(save_as, SIGNAL(triggered(bool)), mainCentralWgt, SLOT(matrix_save_as_reciver()));
    //Подключение сигнала (новый файл) к слоту mainCentralWgt для создания нового файла
    connect(file_new, SIGNAL(triggered(bool)), mainCentralWgt, SLOT(new_matrix_file_edit()));
    //Подключение сигнала кнопки старт
    connect(start_button, SIGNAL(clicked(bool)), mainCentralWgt, SLOT(start()));
    //Подключени сигнала для добавления действий в объект calculate_select
    connect(input_select, SIGNAL(activated(int)), calculate_select, SLOT(create_action(int)));
    //Подключени сигнала для передачи номера действия объекту mainCentralWgt
    connect(calculate_select, SIGNAL(activated(int)), mainCentralWgt, SLOT(get_op_num(int)));
    //Поключение сигнала для передачи номера вводимых данных объекту mainCentralWgt
    connect(calculate_select, SIGNAL(drop_input(int)), mainCentralWgt, SLOT(input_num_get(int)));
}

//QCombobox со слотом для выбора действий с входными данными
FlexibleBox::FlexibleBox(QWidget *parrent): QComboBox(parrent){}

void FlexibleBox::create_action(int i){
    switch (i) {
    case 0:
        this->clear();
        this->addItems(QList<QString>("Вычислить детеримнант"));
        this->addItems(QList<QString>("Сложить"));
        emit drop_input(i);
        break;
    case 1:
        this->clear();
        this->addItems(QList<QString>("Найти корни"));
        emit drop_input(i);
        break;
    default:
        break;
    }
}
