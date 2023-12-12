#include "webservice.h"

WebService::WebService(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this))
{
    connect(manager, &QNetworkAccessManager::finished, this, &WebService::HandleNetworkReply);
}

WebService::~WebService()
{
    if (manager != nullptr)
    {
        delete manager;
        manager = nullptr;
    }
}

void WebService::FetchData(const QUrl &url, int controlIdentifier)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    reply->setProperty("controlIdentifier", QVariant::fromValue(controlIdentifier));
}

void WebService::HandleNetworkReply(QNetworkReply *reply)
{
    int controlIdentifier = reply->property("controlIdentifier").toInt();

    if (reply->error() == QNetworkReply::NoError) {
        emit dataFetched(reply->readAll(), controlIdentifier);
    } else {
        emit errorOccurred(reply->errorString(), controlIdentifier);
    }

    reply->deleteLater();
}
