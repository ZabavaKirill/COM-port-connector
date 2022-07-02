#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>

/*
    Графический интерфейс приложения
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sendingTextEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z,.\\s]+"), this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
    Добавление доступных портов в выпадающий список
*/
void MainWindow::setPorts(QStringList ports) {
    ui->portComboBox->clear();
    ui->portComboBox->addItems(ports);
}

/*
    Заполнение выпадающих списков, содержащих параметры порта
*/
void MainWindow::setBaudRates(QMetaEnum baudRates) {
    ui->baudComboBox->clear();
    for (int i = 0; i < baudRates.keyCount(); i++) {
        if(baudRates.value(i) >= 0)
            ui->baudComboBox->addItem(baudRates.key(i), QVariant(baudRates.value(i)));
    }
}

void MainWindow::setDataBits(QMetaEnum dataBits) {
    ui->dataBitsComboBox->clear();
    for (int i = 0; i < dataBits.keyCount(); i++) {
        if(dataBits.value(i) >= 0)
            ui->dataBitsComboBox->addItem(dataBits.key(i), QVariant(dataBits.value(i)));
    }
}

void MainWindow::setParities(QMetaEnum parities) {
    ui->parityComboBox->clear();
    for (int i = 0; i < parities.keyCount(); i++) {
        if(parities.value(i) >= 0)
            ui->parityComboBox->addItem(parities.key(i), QVariant(parities.value(i)));
    }
}

void MainWindow::setStopBits(QMetaEnum stopBits) {
    ui->stopBitsComboBox->clear();
    for (int i = 0; i < stopBits.keyCount(); i++) {
        if(stopBits.value(i) >= 0)
            ui->stopBitsComboBox->addItem(stopBits.key(i), QVariant(stopBits.value(i)));
    }
}

void MainWindow::setFlowControls(QMetaEnum flowControls) {
    ui->flowControlComboBox->clear();
    for (int i = 0; i < flowControls.keyCount(); i++) {
        if(flowControls.value(i) >= 0)
            ui->flowControlComboBox->addItem(flowControls.key(i), QVariant(flowControls.value(i)));
    }
}

/*
    Вывести сообщение в лог
*/
void MainWindow::addToLog(QString message) {
    ui->log->setTextColor(QColor("black"));
    ui->log->append(message);
}

/*
    Вывести сообщение об ошибке в лог
*/
void MainWindow::addErrorToLog(QString errorMessage) {
    ui->log->setTextColor(QColor("red"));
    ui->log->append('\n' + errorMessage + '\n');
}

void MainWindow::on_portSearchButton_pressed()
{
    emit findPorts();
}

void MainWindow::on_connectButton_pressed()
{
    QString portName = ui->portComboBox->currentText();
    QSerialPort::BaudRate baudRate = QSerialPort::BaudRate(ui->baudComboBox->currentData().toInt());
    QSerialPort::DataBits dataBits = QSerialPort::DataBits(ui->dataBitsComboBox->currentData().toInt());
    QSerialPort::Parity parity = QSerialPort::Parity(ui->parityComboBox->currentData().toInt());
    QSerialPort::StopBits stopBits = QSerialPort::StopBits(ui->stopBitsComboBox->currentData().toInt());
    QSerialPort::FlowControl flowControl = QSerialPort::FlowControl(ui->flowControlComboBox->currentData().toInt());

    emit connectPort(portName, baudRate, dataBits, parity, stopBits, flowControl);
}

void MainWindow::onTemperatureChanged(QString temperature) {
    ui->temperatureEdit->setText(temperature);
}

void MainWindow::onPressureChanged(QString pressure) {
    ui->pressureEdit->setText(pressure);
}

void MainWindow::on_disconnectButton_pressed()
{
    ui->temperatureEdit->clear();
    ui->pressureEdit->clear();

    emit disconnectPort();
}

void MainWindow::on_sendButton_pressed()
{
    emit sendText(ui->sendingTextEdit->text());
}
