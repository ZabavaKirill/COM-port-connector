#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

/*
    Графический интерфейс приложения
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setPorts(QStringList);
    void setBaudRates(QMetaEnum);
    void setDataBits(QMetaEnum);
    void setParities(QMetaEnum);
    void setStopBits(QMetaEnum);
    void setFlowControls(QMetaEnum);

private:
    Ui::MainWindow *ui;

signals:
    void findPorts();
    void connectPort(QString, QSerialPort::BaudRate, QSerialPort::DataBits, QSerialPort::Parity, QSerialPort::StopBits, QSerialPort::FlowControl);
    void disconnectPort();
    void sendText(QString);

public slots:
    void addToLog(QString);
    void addErrorToLog(QString);
    void onTemperatureChanged(QString);
    void onPressureChanged(QString);

private slots:
    void on_portSearchButton_pressed();
    void on_connectButton_pressed();
    void on_disconnectButton_pressed();
    void on_sendButton_pressed();
};

#endif // MAINWINDOW_H
