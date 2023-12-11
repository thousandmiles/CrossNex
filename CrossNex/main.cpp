#include "mainwindow.h"
#include "dataservice.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    DataService *data = new DataService();
    data->StartService();

    return a.exec();
}
