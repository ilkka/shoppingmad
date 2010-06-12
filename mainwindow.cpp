#include "mainwindow.h"

#include "wantedmodel.h"

#include <QUrl>
#include <QDeclarativeContext>
#include <QGraphicsObject>

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{
    setSource(QUrl("qrc:/shoppinglist/qml/main.qml"));
    rootContext()->setContextProperty("WantedItemsModel", new WantedModel);
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
}
