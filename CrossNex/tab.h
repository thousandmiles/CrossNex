#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QMessageBox>
#include <QJsonObject>
#include <QTimer>
#include "dataservice.h"


class WebTransport : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject CPUJsonData MEMBER CPUJsonData NOTIFY CPUJsonDataChanged)
    Q_PROPERTY(QJsonObject MemoryJsonData MEMBER MemoryJsonData NOTIFY MemoryJsonDataChanged)
    Q_PROPERTY(QJsonObject DiskJsonData MEMBER DiskJsonData NOTIFY DiskJsonDataChanged)

public:
    explicit WebTransport (QWebEnginePage *page, QObject *parent = nullptr)
        : QObject(parent), webPage(page) {}
    qint64 GetDiskSize(const QString& diskInfo);

public slots:
    void sendCPUDataToJavaScript(const QJsonObject &data);
    void sendMemoryDataToJavaScript(const QJsonObject &data);
    void sendDiskDataToJavaScript(const QJsonObject &data);

signals:
    void CPUJsonDataChanged(const QJsonObject &JsonData);
    void MemoryJsonDataChanged(const QJsonObject &JsonData);
    void DiskJsonDataChanged(const QJsonObject &JsonData);
    void DiskUsedPercentIsObtained(const int& usedValue);

private:
    QWebEnginePage *webPage;
    QJsonObject CPUJsonData;
    QJsonObject MemoryJsonData;
    QJsonObject DiskJsonData;
};



//-------------------------
//  Hello Page Tab class
//-------------------------
class Hello: public QWidget
{
    Q_OBJECT

public:
    Hello(QWidget *parent = nullptr);
private:
    QWebEngineView *webEngineView;
};



//-------------------------
//  Normal Tab class
//-------------------------

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(const QString& ip, QWidget *parent = nullptr);

public slots:
    void fetchData();          // test function

private slots:
    void updateCPUData(const QByteArray &data);
    // void updateCPUTime(const QByteArray &data);
    // void updateProcessTimePid(const QByteArray &data);
    void updateDiskList(const QByteArray &data);
    // void updateProcessMemoryPid(const QByteArray &data);
    void updateMachineMemory(const QByteArray &data);
    // void updateProcessInfo(const QByteArray &data);

    void handleError(const QString &error);

signals:

private:
    QWebEngineView *webEngineView;
    QWebChannel *webChannel;
    WebTransport* webTransport;
    QString ip;
    QTimer timer;
    DataService *dataService;
};

#endif // TAB_H
