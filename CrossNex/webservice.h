#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WebService : public QObject
{
    Q_OBJECT
public:
    explicit WebService(QObject *parent = nullptr);

public slots:
    void FetchDataFromApi(const QUrl &url);

private slots:
    void OnReplyFinished(QNetworkReply *reply);

signals:
    void RequestFinished(const QByteArray &data);

private:
    QNetworkAccessManager *networkManager;
};

#endif // WEBSERVICE_H
