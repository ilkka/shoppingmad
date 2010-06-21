#include "mainwindow.h"

#include "wantedmodel.h"
#include "logging.h"

#include <QUrl>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{
    setSource(QUrl("qrc:/shoppinglist/qml/main.qml"));
    rootContext()->setContextProperty("WantedItemsModel", WantedModel::create());
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    LOG_TRACE("resized to " << event->size());
    rootObject()->setProperty("width", event->size().width());
    rootObject()->setProperty("height", event->size().height());
}
