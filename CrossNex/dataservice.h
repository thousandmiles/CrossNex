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

    void FetchData(int controlIdentifier, int pid = -1);

    QString getIP() const;
    void setIP(const QString &ip);

    QString getURLPATH() const;

signals:
    void control_CPU_Info_DataReady(const QByteArray &data);
    void control_CPU_Time_DataReady(const QByteArray &data);
    void control_Process_Time_pid_DataReady(const QByteArray &data);
    void control_Disk_List_DataReady(const QByteArray &data);
    void control_Process_Memory_pid_DataReady(const QByteArray &data);
    void control_Machine_Memory_DataReady(const QByteArray &data);
    void control_Process_Info_DataReady(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void HandleWebServiceData(const QByteArray &data, int controlIdentifier);
    void HandleWebServiceError(const QString &error, int controlIdentifier);

private:
    QString IP;
    QString PORT;
    QString URLPATH;

private:
    WebService *webService;
};

#endif // DATASERVICE_H
