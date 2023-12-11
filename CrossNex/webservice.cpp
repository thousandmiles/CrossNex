#include "webservice.h"

WebService::WebService(QObject *parent)
    : QObject{parent}
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &WebService::OnReplyFinished);
}

void WebService::FetchDataFromApi(const QUrl &url)
{
    QNetworkRequest request(url);
    networkManager->get(request);
}

void WebService::OnReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        emit RequestFinished(data);
    }
    else
    {
        qDebug() << "Error: " << reply->errorString();
    }

    reply->deleteLater();
}

