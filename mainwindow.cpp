#include "mainwindow.h"

#include "wantedmodel.h"

#include <QUrl>
#include <QDeclarativeContext>

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{
    setSource(QUrl("qrc:/shoppinglist/qml/main.qml"));
    rootContext()->setContextProperty("WantedItemsModel", new WantedModel);
}

MainWindow::~MainWindow()
{

}
