#include "mainwindow.h"

#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{
    setSource(QUrl("qrc:/shoppinglist/qml/main.qml"));
}

MainWindow::~MainWindow()
{

}
