#include "connector.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <memory>
#include <QDebug>
/*
    Класс, реализующий подключение к последовательному порту, проверку настроек порта и чтение/запись с порта
*/

Connector::Connector(QObject *parent) : QObject(parent)
{
    receivedStr = "";
    port = new QSerialPort();
    portCheckTimer = new QTimer();
    portReadTimer = new QTimer();

    //проверка завершения чтения с порта
    connect(portReadTimer, &QTimer::timeout,
            [=]() {
        if(receivedStr.length() < expectedStr.length()) {
            emit notify("Получение текста...");
        } else {
            emit notify("Полученный текст: " + receivedStr);
            portReadTimer->stop();
        }
    });

    connect(port, SIGNAL(readyRead()), this, SLOT(readSerial()));
}

/*
    Поиск доступных для подключения портов
*/
QStringList Connector::findPorts() {
    QStringList *ports = new QStringList();
    foreach (QSerialPortInfo portInfo, QSerialPortInfo::availablePorts()) {
        ports->append(portInfo.portName());
    }
    return *ports;
}

/*
    Подключение к порту
*/
void Connector::connectPort(QString portName, QSerialPort::BaudRate baudRate,
                            QSerialPort::DataBits dataBits, QSerialPort::Parity parity,
                            QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl) {
    disconnectPort();
    port->setPortName(portName);

    if(port->open(QIODevice::ReadWrite)) {
        if(port->setBaudRate(baudRate) && port->setDataBits(dataBits) &&
                port->setParity(parity) && port->setStopBits(stopBits) &&
                port->setFlowControl(flowControl)) {
            if(port->isOpen()) {
                emit notify("Открыт порт " + port->portName() + "\n");
                checkPortSettings();
            }
        } else {
            port->close();
            emit errorOccured("Возникла ошибка: не удалось установить параметры для подключения к порту.");
        }
    } else {
        emit errorOccured("Возникла ошибка: невозможно подключиться к порту.");
    }

}

/*
    Отключение от порта
*/
void Connector::disconnectPort() {
    if(port->isOpen()) {
        port->close();
        emit notify("\nЗакрыт порт " + port->portName() + '\n');
    }
}

/*
    Проверка параметров порта
*/
void Connector::checkPortSettings() {
    elapsedTime = 0;
    expectedStr = "test";
    receivedStr = "";

    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(portCheckTimer, &QTimer::timeout, this,                 //будет выполнятся по сигналу таймера проверки параметров порта
            [this, conn]() {                                                //*conn содержит указатель на соединение между таймером и функцией
        //qDebug() << elapsedTime << receivedStr << expectedStr;              //и используется чтобы разорвать его при завершении проверки
        if(receivedStr == expectedStr) {                                    //если просто останавливать таймер, происходит лишнее срабатывание функции
            emit notify("Проверка завершена: порт настроен корректно.\n");
            disconnect(*conn);
            portCheckTimer->stop();
        }
        else if(receivedStr.length() > expectedStr.length() || elapsedTime > MAX_CHECK_TIME) {
            emit errorOccured("Возникла ошибка: выберите другие настройки порта.");
            disconnect(*conn);
            disconnectPort();
            portCheckTimer->stop();
        }
        else {
            emit notify("Проверка параметров порта...");
            elapsedTime += DELAY_MS;
        }
    });

    if(port->isWritable()) {            //запускаем таймер для проверки параметров порта
        emit notify("Проверка параметров порта...");
        port->write(expectedStr.toLocal8Bit());
        portCheckTimer->start(DELAY_MS);
    } else {
        emit errorOccured("Возникла ошибка: порт открыт только для чтения.");
    }
}

/*
    Запись в порт
*/
void Connector::writeMessageToPort(QString str) {
    expectedStr = str;

    if(port->isOpen()) {
        if(port->isWritable()) {
            emit notify("Отправленный текст: " + str);
            port->write(str.toLocal8Bit());
        } else {
            emit errorOccured("Возникла ошибка: порт открыт только для чтения.");
        }
   } else {
        emit errorOccured("Возникла ошибка: порт закрыт.");
    }
}

/*
    Чтение сообщения из порта
*/
void Connector::readMessageFromPort() {
    receivedStr = "";

    if(port->isOpen()) {
        if(port->isReadable()) {
            emit notify("Получение текста...");
            portReadTimer->start(DELAY_MS);
        } else {
            emit errorOccured("Возникла ошибка: порт открыт только для записи.");
        }
    } else {
        emit errorOccured("Возникла ошибка: порт закрыт.");
    }
}

/*
    Функция, выполняемая если появилась новая информация, доступная к считыванию из порта
*/
void Connector::readSerial() {
    serialData = port->readAll();

    if(serialData != "\r") {
        QString str = QString(serialData);  //преобразуем QByteArray в QString

        QStringList data = str.split(":"); //данные поступают в формате "строка:температура:давление"

        receivedStr += data[0];

        emit temperatureChanged(data[1]);
        emit pressureChanged(data[2]);

        /*if(str.contains(QRegExp("[A-Za-z,.\\s]"))) {     //если в полученных данных есть буквы, добавляем их в переменную receivedStr
            receivedStr += (new QString(str))->remove(QRegExp("[0-9:]"));
        }

        if(!portCheckTimer->isActive()) {           //считываем температуру и давление только в том случае, если завершена проверка параметров порта
            QStringList numericalData = str.remove(QRegExp("[A-Za-z,.\\s]")).split(":");
            emit temperatureChanged(numericalData[0]);
            emit pressureChanged(numericalData[1]);
        }*/
    }
}
