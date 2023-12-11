#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "webservice.h"

class DataService : public QObject
{
    Q_OBJECT
public:
    explicit DataService(QObject *parent = nullptr);
    ~DataService();

public slots:
    void StartService();
    void StopService();
    void GetNext();


signals:
    void ApiDataReceived(const QByteArray &data);

private:
    QTimer *m_timer;
    WebService *m_apiRequester;
    bool m_isRunning;
    bool Get;
    unsigned int m_urlIndex;
    static const QStringList m_urlList;

    QString IP = "192.168.136.128";
    QString PORT = "8888";
    QString URLPATH = "http://" + IP + ":" + PORT + "/";

    void FetchDataFromApi();
    void SetUrlList();
};

#endif // DATASERVICE_H
