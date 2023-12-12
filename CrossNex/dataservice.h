#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QObject>
#include "webservice.h"

class DataService : public QObject
{
    Q_OBJECT

public:
    DataService(QObject *parent = nullptr);
    ~DataService();

    void FetchDataForControl1();
    void FetchDataForControl2();
    void FetchData(int controlIdentifier, int pid = -1);

signals:
    void control_1_DataReady(const QByteArray &data);
    void control_2_DataReady(const QByteArray &data);
    void control_3_DataReady(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void HandleWebServiceData(const QByteArray &data, int controlIdentifier);
    void HandleWebServiceError(const QString &error, int controlIdentifier);

private:
    QString IP = "192.168.136.128";
    QString PORT = "8888";
    QString URLPATH = "http://" + IP + ":" + PORT + "/";

private:
    WebService *webService;
};

#endif // DATASERVICE_H
