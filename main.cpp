#include <QtGui>
#include "mainwindow.h"

void main(int argc,char *argv[])
{
    QApplication app(argc,argv);

    MainWindow a;

    a.show();
//    a.showMaximized();

    app.exec();
}
