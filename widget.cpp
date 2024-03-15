#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Widget::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Widget::disconnectFromServer);

    ipLineEdit = new QLineEdit();
    portLineEdit = new QLineEdit();
    connectButton = new QPushButton("Connect");
    logTextEdit = new QTextEdit;
    messageLineEdit = new QLineEdit();
    sendButton = new QPushButton("Send");

    ipLineEdit->setPlaceholderText("ip adress");
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ipLineEdit->setValidator(ipValidator);
    portLineEdit->setPlaceholderText("port");
    portLineEdit->setValidator(new QDoubleValidator(portLineEdit));
    messageLineEdit->setPlaceholderText("Enter a message:");
    connectButton->setCheckable(true);
    logTextEdit->setReadOnly(true);

    topLayout = new QHBoxLayout();
    topLayout->addWidget(ipLineEdit);
    topLayout->addWidget(portLineEdit);
    topLayout->addWidget(connectButton);

    bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(messageLineEdit);
    bottomLayout->addWidget(sendButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(logTextEdit);
    mainLayout->addLayout(bottomLayout);

    connect(sendButton, &QPushButton::clicked, this, &Widget::ClickedSlot);
    connect(connectButton, &QPushButton::toggled, this, &Widget::ToggleSlot);
}

Widget::~Widget()
{

}

void Widget::SendToServer(QString str)
{
    if(socket->state() != 0)
    {
        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out << str;
        socket->write(Data);
        printTE(str);
    }
    else
    {
        QString str1 = "Not connected to server";
        printTE(str1);
    }
}

void Widget::connectToS(QString ip, int port)
{
    socket->connectToHost(ip, port);
    if(socket->waitForConnected(1000))
    {
        QString str = "Connected to server";
        printTE(str);
        QString str1 = "Hello, server!";
        SendToServer(str1);
        qDebug() << "connected";
    }
    else
    {
        QString str2 = "Failed to connect server";
        printTE(str2);
        connectButton->toggle();
    }
}

void Widget::printTE(QString str)
{
    QString time_format = "dd.MM.yyyy HH:mm:ss";
    QDateTime cdt = QDateTime::currentDateTime();
    QString dt = cdt.toString(time_format);
    QString ipAddr = socket->localAddress().toString() + ":";
    QString portS = QString::number(socket->localPort());
    QString str1 = dt + " | " + ipAddr + portS + " | " + str;
    logTextEdit->append(str1);
}

void Widget::printFromServer(QString str)
{
    QString time_format = "dd.MM.yyyy HH:mm:ss";
    QDateTime cdt = QDateTime::currentDateTime();
    QString dt = cdt.toString(time_format);
    QString ipAddr = socket->peerAddress().toString() + ":";
    QString portS = QString::number(socket->peerPort());
    QString str1 = dt + " | " + ipAddr + portS + " | " + str;
    logTextEdit->append(str1);
}

void Widget::slotReadyRead()
{
    QDataStream in(socket);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        printFromServer(str);
    }
    else
    {
        QString str = "DataStream error";
        printTE(str);
    }
}

void Widget::disconnectFromServer()
{
    socket->close();
}

void Widget::ClickedSlot()
{
    SendToServer(messageLineEdit->text());
    messageLineEdit->clear();
}

void Widget::ToggleSlot(bool checked)
{
    if(checked)
    {
        QString ip = ipLineEdit->text();
        int port = portLineEdit->text().toInt();
        if(port > 65535)
        {
            qDebug() << "too big number";
            QString str1 = "Error, too big number";
            printTE(str1);
            connectButton->toggle();
        }
        else if(port < 1)
        {
            qDebug() << "port cannot be negative";
            QString str1 = "Error, port cannot be negative";
            printTE(str1);
            connectButton->toggle();
        }
        else
        {
            connectToS(ip, port);
        }
    }
    else
    {
        if(socket->state() != 0)
        {
            socket->disconnectFromHost();
            QString str = "Disconnected from server";
            printTE(str);
        }
    }
}
