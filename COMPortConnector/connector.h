#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

/*
    Класс, реализующий подключение к последовательному порту, проверку настроек порта и чтение/запись с порта
*/

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);

private:
    QSerialPort *port;
    QTimer *portCheckTimer, *portReadTimer;
    QByteArray serialData;
    QString receivedStr, expectedStr = "test";
    const int DELAY_MS = 1000,
              MAX_CHECK_TIME = 5000;
    int elapsedTime;

public:
    QStringList findPorts();

signals:
    void errorOccured(QString);
    void notify(QString);
    void temperatureChanged(QString);
    void pressureChanged(QString);

public slots:
    void connectPort(QString, QSerialPort::BaudRate, QSerialPort::DataBits, QSerialPort::Parity, QSerialPort::StopBits, QSerialPort::FlowControl);
    void disconnectPort();
    void writeMessageToPort(QString);
    void readMessageFromPort();

private slots:
    void checkPortSettings();
    void readSerial();
};

#endif // CONNECTOR_H
