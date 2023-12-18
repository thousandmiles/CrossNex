#include "mainwindow.h"
#include "navigationtree.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->resize(1200,900);
    this->setStyleSheet("background-color: #E0E0E0;");

    ui->menubar->setStyleSheet("QMenuBar {background-color: #E0E0E0;}"
                               "QMenuBar::item { padding: 8px; }"
                               "QMenuBar::item:selected { background-color: #555; color: white; }"
                               );

    ui->menubar->setFixedHeight(30);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 8);
    ui->treeWidget->setStyleSheet("background-color:0;");
    ui->statusbar->showMessage("CrossNex V-1.0");

}

MainWindow::~MainWindow()
{
    delete ui;
}
