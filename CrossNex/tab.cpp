#include "tab.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>

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
    // connect(dataService, &DataService::control_CPU_Time_DataReady, this, &Tab::updateCPUTime);
    // connect(dataService, &DataService::control_Process_Time_pid_DataReady, this, &Tab::updateProcessTimePid);
    connect(dataService, &DataService::control_Disk_List_DataReady, this, &Tab::updateDiskList);
    // connect(dataService, &DataService::control_Process_Memory_pid_DataReady, this, &Tab::updateProcessMemoryPid);
    connect(dataService, &DataService::control_Machine_Memory_DataReady, this, &Tab::updateMachineMemory);
    connect(dataService, &DataService::control_Process_Info_DataReady, this, &Tab::updateProcessInfo);

    connect(dataService, &DataService::errorOccurred, this, &Tab::handleError);

    // 获取URL数据
    dataService->setIP(ip);

    timer.start(1000);
}

// return "** KB"
qint64 WebTransport::GetDiskSize(const QString &diskInfo)
{
    QString sizeStr = diskInfo;

    if (diskInfo.endsWith("G"))
    {
        return sizeStr.remove("G").toDouble() * 1024 * 1024;
    }
    else if (diskInfo.endsWith("M"))
    {
        return sizeStr.remove("M").toDouble() * 1024;
    }
    else if (diskInfo.endsWith("K"))
    {
        return sizeStr.remove("K").toDouble();
    }

    return 0;
}


void Tab::updateCPUData(const QByteArray &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject cpuJson = jsonDoc.object();
        webTransport->sendCPUDataToJavaScript(cpuJson);
    }
}

void Tab::updateDiskList(const QByteArray &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        webTransport->sendDiskDataToJavaScript(jsonObj);
    }
}

void Tab::updateMachineMemory(const QByteArray &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject memoryJson = jsonDoc.object();
        webTransport->sendMemoryDataToJavaScript(memoryJson);
    }
}

void Tab::updateProcessInfo(const QByteArray &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject processJson = jsonDoc.object();
        webTransport->sendProcessDataToJavaScript(processJson);
    }
}


void Tab::handleError(const QString &error)
{
    qDebug()<<"Error";
}


void Tab::fetchData() {

    dataService->FetchData(ID_CPU_Info);
    dataService->FetchData(ID_Machine_Memory);
    dataService->FetchData(ID_Disk_List);
    dataService->FetchData(ID_Process_Info);
    timer.stop();
}


Hello::Hello(QWidget *parent): QWidget{parent}
{
    webEngineView = new QWebEngineView(this);
    webEngineView->load(QUrl("qrc:/hello/EChart/hello.html"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(webEngineView);
    setLayout(layout);
}

void WebTransport::sendCPUDataToJavaScript(const QJsonObject &data) {
    // 将数据从Qt传递到JavaScript
    setProperty("CPUJsonData", data);
}

void WebTransport::sendMemoryDataToJavaScript(const QJsonObject &data)
{
    qint64 totalMemory = data["total_memory"].toInt();
    qint64 freeMemory = data["free_memory"].toInt();
    qint64 availableMemory = data["available_memory"].toInt();
    qint64 buffers = data["buffers"].toInt();
    qint64 cached = data["cached"].toInt();

    QJsonObject newJsonObj;
    newJsonObj["total_memory"] = double(totalMemory) / (1024 * 1024);
    newJsonObj["free_memory"] = double(freeMemory) / (1024 * 1024);
    newJsonObj["available_memory"] = double(availableMemory) / (1024 * 1024);
    newJsonObj["buffers"] = double(buffers) / 1024;
    newJsonObj["cached"] = double(cached) / 1024;
    newJsonObj["used_percent"] = (double(totalMemory - availableMemory) / totalMemory) * 100;
    qDebug()<<newJsonObj;
    setProperty("MemoryJsonData", newJsonObj);

}

void WebTransport::sendDiskDataToJavaScript(const QJsonObject &data)
{
    setProperty("DiskJsonData", data);
    double used_percent = 0;

    if (data.contains("disk") && data["disk"].isArray())
    {
        qint64 totalSize = 0;   // KB
        qint64 usedSize = 0;    // KB
        QJsonArray diskArray = data["disk"].toArray();

        foreach (const QJsonValue &diskValue, diskArray)
        {
            if (diskValue.isObject())
            {
                QJsonObject diskObj = diskValue.toObject();
                if (diskObj.contains("size") && diskObj.contains("used"))
                {
                    QString sizeStr = diskObj["size"].toString();
                    QString usedStr = diskObj["used"].toString();

                    qint64 currentTotalSize = GetDiskSize(sizeStr);
                    qint64 currentUsedSize = GetDiskSize(usedStr);

                    totalSize += currentTotalSize;
                    usedSize += currentUsedSize;
                }
            }
        }

        used_percent = ((double)usedSize / (totalSize + 1)) * 100;

        qDebug()<<"usedSize: "<<usedSize<<"totalSize: "<<totalSize<<"used_percent: "<<used_percent;

        emit DiskUsedPercentIsObtained(used_percent);
    }
}

void WebTransport::sendProcessDataToJavaScript(const QJsonObject &data)
{
    setProperty("ProcessJsonData", data);
}
