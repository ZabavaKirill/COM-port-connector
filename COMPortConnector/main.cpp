#include "mainwindow.h"
#include "controller.h"
#include <QApplication>

/*
    COMPortConnector
    Программа предназначена для подключения к Arduino UNO
    по последовательному порту, передачи текстового сообщения
    и считывания показателей температуры и давления с Arduino.
    Перед подключением к порту производится проверка параметров порта.
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *window = new MainWindow();
    Controller *controller = new Controller(window);
    window->show();

    return a.exec();
}
