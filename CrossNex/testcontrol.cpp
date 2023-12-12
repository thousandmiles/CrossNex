#include "testcontrol.h"
#include <QJsonDocument>
#include <QTextEdit>

TestControl::TestControl(QMainWindow *parent)
    : QMainWindow(parent), dataService(new DataService(this))
{

    connect(dataService, &DataService::control_1_DataReady, this, &TestControl::UpdateData);
    connect(dataService, &DataService::errorOccurred, this, &TestControl::HandleError);

    dataService->FetchData(ID_CPU_Info);

}

TestControl::~TestControl()
{
    if (dataService != nullptr)
    {
        delete dataService;
        dataService = nullptr;
    }
}

void TestControl::UpdateData(const QByteArray &data)
{
    QString dataAsString = QString::fromUtf8(data);
    qDebug()<<dataAsString;
}

void TestControl::HandleError(const QString &error)
{
    qDebug()<<"Error";
}
