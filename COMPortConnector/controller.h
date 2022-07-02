#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "connector.h"

/*
    Класс, реализующий взаимодействие между графическим интерфейсом и логикой
*/

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    Controller(MainWindow *w);

private:
    MainWindow *window;
    Connector *connector;

signals:

public slots:
    void findPorts();
};

#endif // CONTROLLER_H
