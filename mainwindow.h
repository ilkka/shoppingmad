#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDeclarativeView>

class MainWindow : public QDeclarativeView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:

};

#endif // MAINWINDOW_H
