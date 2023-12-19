#include "newinstancedialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>

const QRegularExpression NewInstanceDialog::ipv4Regex = QRegularExpression(
    QStringLiteral("^"
                   "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])\\."
                   "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])\\."
                   "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])\\."
                   "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])$"));

NewInstanceDialog::NewInstanceDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("新建实例");

    QFormLayout *formLayout = new QFormLayout(this);
    ipLineEdit = new QLineEdit(this);
    instanceLineEdit = new QLineEdit(this);
    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    formLayout->addRow("IP地址:", ipLineEdit);
    formLayout->addRow("实例名:", instanceLineEdit);
    formLayout->addRow(okButton, cancelButton);

    connect(okButton, &QPushButton::clicked, this, &NewInstanceDialog::acceptClicked);
    connect(cancelButton, &QPushButton::clicked, this, &NewInstanceDialog::reject);
}

QString NewInstanceDialog::getIpAddress() const
{
    return ipLineEdit->text();
}

QString NewInstanceDialog::getInstanceName() const
{
    return instanceLineEdit->text();
}

bool NewInstanceDialog::isValidIpAddress(const QString &ipAddress)
{
    QRegularExpressionMatch match = ipv4Regex.match(ipAddress);
    return match.hasMatch();
}

void NewInstanceDialog::setInstanceSet(const QSet<QString> &itemSet)
{
    instanceSet.clear();
    instanceSet = itemSet;
}

void NewInstanceDialog::acceptClicked()
{
    QString ip = ipLineEdit->text();
    QString instance = instanceLineEdit->text();
    if (ip.isEmpty() ||
        instance.isEmpty())
    {
        QMessageBox::warning(this, "Error", "IP地址和用户名不能为空，请重新输入。");
    }
    else if (!isValidIpAddress(ip))
    {
        QMessageBox::warning(this, "Error", "IP地址格式错误，请重新输入。");
    }
    else if (instanceSet.contains(instance))
    {
        QMessageBox::warning(this, "Error", "实例名已存在，请重新输入。");
    }
    else
    {
        accept(); // 用户输入有效，关闭对话框
    }
}
