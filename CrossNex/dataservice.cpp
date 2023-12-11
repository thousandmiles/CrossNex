#include "dataservice.h"
#include "webservice.h"

DataService::DataService(QObject *parent):
    QObject{parent},
    m_isRunning{false},
    m_urlIndex{0}
{
    m_apiRequester = new WebService();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &DataService::FetchDataFromApi);
    connect(m_apiRequester, &WebService::RequestFinished, this, &DataService::GetNext);
}

const QStringList DataService::m_urlList =
{
    "cpu-info",
    "cpu-total-time",
    //"process-total-time/pid",
    "disk-info",
    //"process-memory/pid",
    "machine-memory",
    "process-info"
};

DataService::~DataService()
{
    m_apiRequester->deleteLater();
    StopService();
}

void DataService::StartService()
{
    if (!m_isRunning)
    {
        m_timer->start(5000);
        m_isRunning = true;
    }
}

void DataService::StopService()
{
    if (m_isRunning)
    {
        m_timer->stop();
        m_isRunning = false;
    }
}

void DataService::GetNext()
{
    m_urlIndex++;
    if (m_urlIndex >= m_urlList.size())
    {
        m_urlIndex = 0;
        return;
    }
}

void DataService::FetchDataFromApi()
{
    // QUrl apiUrl("http://192.168.136.128:8888/cpu-info");
    // m_apiRequester->FetchDataFromApi(apiUrl);

    QUrl apiUrl = URLPATH + m_urlList[m_urlIndex];
    m_apiRequester->FetchDataFromApi(apiUrl);

    qDebug()<<"---"<<apiUrl.toString();
    m_timer->start(5000);
}



