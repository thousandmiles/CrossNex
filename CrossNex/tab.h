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
    Q_PROPERTY(QJsonObject CPUJsonData MEMBER CPUJsonData NOTIFY dataChanged)

public:
    explicit WebTransport (QWebEnginePage *page, QObject *parent = nullptr)
        : QObject(parent), webPage(page) {}

public slots:
    void sendCPUDataToJavaScript(const QJsonObject &data) {
        // 将数据从Qt传递到JavaScript
        setProperty("CPUJsonData", data);
    }
signals:
    void dataChanged(const QJsonObject &CPUJsonData);

private:
    QWebEnginePage *webPage;
    QJsonObject CPUJsonData;
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
    void updateCPUTime(const QByteArray &data);
    void updateProcessTimePid(const QByteArray &data);
    void updateDiskList(const QByteArray &data);
    void updateProcessMemoryPid(const QByteArray &data);
    void updateMachineMemory(const QByteArray &data);
    void updateProcessInfo(const QByteArray &data);

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
