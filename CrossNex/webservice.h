#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

enum URL_ID {
    ID_CPU_Info,
    ID_CPU_Time,
    ID_Process_Time_pid,
    ID_Disk_List,
    ID_Process_Memory_pid,
    ID_Machine_Memory,
    ID_Process_Info
};

class WebService : public QObject {
    Q_OBJECT

public:

    Q_ENUM(URL_ID);
    WebService(QObject *parent = nullptr);
    ~WebService();

    void FetchData(const QUrl &url, int controlIdentifier);

signals:
    void dataFetched(const QByteArray &data, int controlIdentifier);
    void errorOccurred(const QString &error, int controlIdentifier);

private slots:
    void HandleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // WEBSERVICE_H
