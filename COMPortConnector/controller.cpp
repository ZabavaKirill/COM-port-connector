#include "controller.h"
#include <QMetaEnum>
#include <QSerialPort>

/*
    Класс, реализующий взаимодействие между графическим интерфейсом и логикой
*/

Controller::Controller(QObject *parent) : QObject(parent)
{

}

/*
    Конструктор
*/
Controller::Controller(MainWindow *w) {
    this->window = w;
    this->connector = new Connector();

    /*
        Соединяем сигналы и слоты
    */
    connect(window, SIGNAL(findPorts()), this, SLOT(findPorts()));
    connect(window, SIGNAL(connectPort(QString, QSerialPort::BaudRate, QSerialPort::DataBits,
                                       QSerialPort::Parity, QSerialPort::StopBits, QSerialPort::FlowControl)),
            connector, SLOT(connectPort(QString, QSerialPort::BaudRate, QSerialPort::DataBits,
                                        QSerialPort::Parity, QSerialPort::StopBits, QSerialPort::FlowControl)));
    connect(window, SIGNAL(disconnectPort()), connector, SLOT(disconnectPort()));
    connect(window, SIGNAL(sendText(QString)), connector, SLOT(writeMessageToPort(QString)));
    connect(window, SIGNAL(sendText(QString)), connector, SLOT(readMessageFromPort()));
    connect(connector, SIGNAL(notify(QString)), window, SLOT(addToLog(QString)));
    connect(connector, SIGNAL(errorOccured(QString)), window, SLOT(addErrorToLog(QString)));
    connect(connector, SIGNAL(temperatureChanged(QString)), window, SLOT(onTemperatureChanged(QString)));
    connect(connector, SIGNAL(pressureChanged(QString)), window, SLOT(onPressureChanged(QString)));

    /*
        Передаём в UI значения для выпадающих списков
    */
    findPorts();
    window->setBaudRates(QMetaEnum::fromType<QSerialPort::BaudRate>());
    window->setDataBits(QMetaEnum::fromType<QSerialPort::DataBits>());
    window->setParities(QMetaEnum::fromType<QSerialPort::Parity>());
    window->setStopBits(QMetaEnum::fromType<QSerialPort::StopBits>());
    window->setFlowControls(QMetaEnum::fromType<QSerialPort::FlowControl>());
}


void Controller::findPorts() {
    window->setPorts(connector->findPorts());
}
