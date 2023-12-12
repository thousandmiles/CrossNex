#ifndef TESTCONTROL_H
#define TESTCONTROL_H

#include <QMainWindow>
#include "dataservice.h"
#include <QTextEdit>

class TestControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit TestControl(QMainWindow *parent = nullptr);
    ~TestControl();

private slots:
    void UpdateData(const QByteArray &data);
    void HandleError(const QString &error);


private:
    DataService *dataService;
};

#endif // TESTCONTROL_H
