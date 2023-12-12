#include "dataservice.h"
#include "webservice.h"

DataService::DataService(QObject *parent) : QObject(parent), webService(new WebService(this))
{
    connect(webService, &WebService::dataFetched, this, &DataService::HandleWebServiceData);
    connect(webService, &WebService::errorOccurred, this, &DataService::HandleWebServiceError);
}

DataService::~DataService()
{
    if (webService != nullptr)
    {
        delete webService;
        webService = nullptr;
    }
}

void DataService::FetchData(int controlIdentifier, int pid)
{
    QString url = URLPATH;

    switch (controlIdentifier) {
    case ID_CPU_Info:
    {
        url += "cpu-info";
        break;
    }
    case ID_CPU_Time:
    {
        url += "cpu-total-time";
        break;
    }
    case ID_Process_Time_pid:
    {
        if (pid == -1)
        {
            return;
        }
        url += "process-total-time/%1";
        url = url.arg(pid);
        break;
    }
    case ID_Disk_List:
    {
        url += "disk-info";
        break;
    }
    case ID_Process_Memory_pid:
    {
        if (pid == -1)
        {
            return;
        }
        url += "process-memory/%1";
        url = url.arg(pid);
        break;
    }
    case ID_Machine_Memory:
    {
        url += "machine-memory";
        break;
    }
    case ID_Process_Info:
    {
        url += "process-info";
        break;
    }
    default:
        qDebug() << "Unknown ID";
        return;
    }

    QUrl apipath(url);
    qDebug()<<url;
    webService->FetchData(apipath, controlIdentifier);
}

void DataService::HandleWebServiceData(const QByteArray &data, int controlIdentifier)
{
    switch (controlIdentifier) {
    case ID_CPU_Info:
        emit control_1_DataReady(data);
        break;
    case ID_CPU_Time:
        emit control_2_DataReady(data);
        break;
    case ID_Process_Time_pid:
        emit control_3_DataReady(data);
        break;
    case ID_Disk_List:
        emit control_3_DataReady(data);
        break;
    case ID_Process_Memory_pid:
        emit control_3_DataReady(data);
        break;
    case ID_Machine_Memory:
        emit control_3_DataReady(data);
        break;
    case ID_Process_Info:
        emit control_3_DataReady(data);
        break;
    default:
        qDebug() << "Unknown ID";
        break;
    }

}

void DataService::HandleWebServiceError(const QString &error, int controlIdentifier)
{
    emit errorOccurred(error);
}
