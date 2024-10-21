#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile style(":/style.css");
    style.open(QFile::ReadOnly);
    app.setStyleSheet(style.readAll());
    app.setWindowIcon(QPixmap(":/img/icons/main_logo.png"));
    MainWindow* mw = new MainWindow;
    mw->setMinimumSize(600, 500);
    mw->show();
    return app.exec();
}
