#include "tab.h"
#include <QVBoxLayout>
#include <QJsonDocument>

Tab::Tab(const QString& ip, QWidget *parent)
    : QWidget{parent},
    ip(ip)
{
    dataService = new DataService(this);

    webEngineView = new QWebEngineView(this);
    webEngineView->load(QUrl("qrc:/echart/EChart/main-page.html"));

    webChannel = new QWebChannel(webEngineView);
    webTransport = new WebTransport(webEngineView->page());

    webChannel->registerObject(QStringLiteral("webTransport"), webTransport);
    webEngineView->page()->setWebChannel(webChannel);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(webEngineView);
    setLayout(layout);

    connect(&timer, &QTimer::timeout, this, &Tab::fetchData);

    connect(dataService, &DataService::control_CPU_Info_DataReady, this, &Tab::updateCPUData);
    connect(dataService, &DataService::control_CPU_Time_DataReady, this, &Tab::updateCPUTime);
    connect(dataService, &DataService::control_Process_Time_pid_DataReady, this, &Tab::updateProcessTimePid);
    connect(dataService, &DataService::control_Disk_List_DataReady, this, &Tab::updateDiskList);
    connect(dataService, &DataService::control_Process_Memory_pid_DataReady, this, &Tab::updateProcessMemoryPid);
    connect(dataService, &DataService::control_Machine_Memory_DataReady, this, &Tab::updateMachineMemory);
    connect(dataService, &DataService::control_Process_Info_DataReady, this, &Tab::updateProcessInfo);

    connect(dataService, &DataService::errorOccurred, this, &Tab::handleError);

    // 获取URL数据
    dataService->setIP(ip);

    timer.start(1000);
}


void Tab::updateCPUData(const QByteArray &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject cpuJson = jsonDoc.object();

    qDebug()<<"cpuJson: "<< cpuJson;
    webTransport->sendCPUDataToJavaScript(cpuJson);
}


void Tab::handleError(const QString &error)
{
    qDebug()<<"Error";
}


void Tab::fetchData() {

    dataService->FetchData(ID_CPU_Info);
}


Hello::Hello(QWidget *parent): QWidget{parent}
{
    webEngineView = new QWebEngineView(this);
    webEngineView->load(QUrl("qrc:/hello/EChart/hello.html"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(webEngineView);
    setLayout(layout);
}
